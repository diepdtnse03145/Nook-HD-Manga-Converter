#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "book.h"

#include <QtCore>
#include <QtWidgets>
#include <QtGui>
#include <QtConcurrent>

#define OP_DIR "E:/Documents/Manga"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    stateMachine(new QStateMachine(this)),
    lastSelectDir(OP_DIR)
{
    ui->setupUi(this);
    ui->listWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
    //Setting states
    QState *runningState = new QState();
    QState *stopState = new QState();

    stopState->addTransition(this,SIGNAL(started()),runningState);
    runningState->addTransition(this,SIGNAL(finished()),stopState);

    runningState->assignProperty(ui->addButton,"enabled","false");
    runningState->assignProperty(ui->clearAllButton,"enabled","false");
    runningState->assignProperty(ui->removeButton,"enabled","false");


    stopState->assignProperty(ui->addButton,"enabled","true");
    stopState->assignProperty(ui->clearAllButton,"enabled","true");
    stopState->assignProperty(ui->removeButton,"enabled","true");

    connect(runningState,&QState::entered,[&]{
        qDebug()<<QTime::currentTime()<<"start convert";
    });
    connect(runningState,&QState::exited,[&]{
        qDebug()<<QTime::currentTime()<<"stop convert";
    });
    connect(runningState,&QState::exited,[&]{
        ui->progressBar->setValue(100);
        QMessageBox::information(
                    this,
                    tr("Nook HD+ Manga Converter"),
                    tr("All job completed!") );
    });

    stateMachine->addState(stopState);
    stateMachine->addState(runningState);
    stateMachine->setInitialState(stopState);

    connect(ui->convertButton,&QPushButton::clicked,this,&MainWindow::convert);
    connect(ui->addButton,&QPushButton::clicked,this,&MainWindow::addBook);
    connect(ui->clearAllButton,&QPushButton::clicked,ui->listWidget,&QListWidget::clear);
    connect(ui->removeButton,&QPushButton::clicked,[&]{qDeleteAll(ui->listWidget->selectedItems());});
    connect(this,&MainWindow::completed,ui->progressBar,&QProgressBar::setValue);
    stateMachine->start();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete stateMachine;
}

void MainWindow::addBook()
{
    //Multiselection file dialog
    //http://www.qtcentre.org/threads/34226-QFileDialog-select-multiple-directories?p=220108#post220108
    QFileDialog getBookListDialog(this,"Choose Book to Convert",lastSelectDir);
    getBookListDialog.setFileMode(QFileDialog::DirectoryOnly);
    getBookListDialog.setOption(QFileDialog::DontUseNativeDialog,true);

    QListView *l = getBookListDialog.findChild<QListView*>("listView");
    if (l)
    {
        l->setSelectionMode(QAbstractItemView::ExtendedSelection);
    }

    QTreeView *t = getBookListDialog.findChild<QTreeView*>("treeView");
    if (t)
    {
        t->setSelectionMode(QAbstractItemView::ExtendedSelection);
    }
    //

    if(getBookListDialog.exec() == QDialog::Accepted)
    {
        ui->listWidget->addItems(getBookListDialog.selectedFiles());
    }
    this->lastSelectDir = getBookListDialog.directory().absolutePath();
}

void MainWindow::convert()
{
    emit this->started();

    QtConcurrent::run([&]{
        Book currentBook;
        for(int i = 0; i < ui->listWidget->count(); ++i)
        {
            currentBook.setSource(ui->listWidget->item(i)->text());
            currentBook.convert();
            emit completed((i*100)/ui->listWidget->count());
        }

        emit this->finished();
    });

}

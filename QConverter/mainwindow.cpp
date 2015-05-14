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
    stateMachine(new QStateMachine(this))
{
    ui->setupUi(this);
    ui->listWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);

    QState *runningState = new QState();
    QState *stopState = new QState();

    stopState->addTransition(ui->convertButton,SIGNAL(clicked()),runningState);
    runningState->addTransition(this,SIGNAL(finished()),stopState);

    connect(stopState,&QState::entered,[&]{ui->statusBar->showMessage("Stop");});
    connect(runningState,&QState::entered,[&]{ui->statusBar->showMessage("Running");});

    connect(runningState,&QState::entered,this,&MainWindow::convert);

    connect(ui->addButton,SIGNAL(clicked()),this,SLOT(addBook()));
    connect(ui->clearAllButton,SIGNAL(clicked()),ui->listWidget,SLOT(clear()));
    connect(ui->removeButton,&QPushButton::clicked,[&]{qDeleteAll(ui->listWidget->selectedItems());});

    stateMachine->addState(stopState);
    stateMachine->addState(runningState);
    stateMachine->setInitialState(stopState);

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
    qDebug()<<QTime::currentTime();
    //convert all book
    QtConcurrent::run([&]{
        for(int i = 0; i < ui->listWidget->count(); ++i)
        {
            Book currentBook;
            currentBook.setSource(ui->listWidget->item(i)->text());
            currentBook.convert();
            qDebug()<<ui->listWidget->item(i)->text();
        }

        emit this->finished();
    });

    qDebug()<<QTime::currentTime();
    //    QMessageBox::information(
    //                this,
    //                tr("Nook HD+ Manga Converter"),
    //                tr("All job completed!") );


}

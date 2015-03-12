#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "book.h"

#include <QFileDialog>
#include <QList>
#include <QDebug>
#include <QTime>
#include <QMessageBox>
#include <QTreeView>

#define OP_DIR "D:/Documents/Manga"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    currentBook(new Book(this))
{
    ui->setupUi(this);
    ui->listWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);

    connect(ui->convertButton,SIGNAL(clicked()),this,SLOT(convert()));
    connect(ui->addButton,SIGNAL(clicked()),this,SLOT(addBook()));
    connect(ui->clearAllButton,SIGNAL(clicked()),ui->listWidget,SLOT(clear()));
    connect(ui->removeButton,SIGNAL(clicked()),this,SLOT(remove()));

}

MainWindow::~MainWindow()
{
    delete ui;
    delete currentBook;
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

    for(int i = 0; i < ui->listWidget->count(); ++i)
    {
        currentBook->setSource(ui->listWidget->item(i)->text());
        currentBook->convert();
    }  
    qDebug()<<QTime::currentTime();
    QMessageBox::information(
                this,
                tr("Nook HD+ Manga Converter"),
                tr("All job completed!") );
}

void MainWindow::remove()
{
    qDeleteAll(ui->listWidget->selectedItems());
}


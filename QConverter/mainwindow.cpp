#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "book.h"

#include <QFileDialog>
#include <QList>
#include <QUrl>
#include <QDebug>
//#include <QTreeView>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->listWidget->setSelectionMode(QAbstractItemView::MultiSelection);

    connect(ui->convertButton,SIGNAL(clicked()),this,SLOT(convert()));
    connect(ui->addButton,SIGNAL(clicked()),this,SLOT(addBook()));
    connect(ui->clearAllButton,SIGNAL(clicked()),ui->listWidget,SLOT(clear()));
    connect(ui->removeButton,SIGNAL(clicked()),this,SLOT(remove()));


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addBook()
{
    QFileDialog getBookListDialog(this,"Choose Book to Convert","/home");
    getBookListDialog.setFileMode(QFileDialog::DirectoryOnly);
    getBookListDialog.setOption(QFileDialog::DontUseNativeDialog,true);
    QListView *l = getBookListDialog.findChild<QListView*>("listView");
    if (l) {
    l->setSelectionMode(QAbstractItemView::MultiSelection);
    }

    /*QTreeView *t = getBookListDialog.findChild<QTreeView*>();
    if (t) {
    t->setSelectionMode(QAbstractItemView::MultiSelection);
    }*/


    if(getBookListDialog.exec() == QDialog::Accepted)
    {
        ui->listWidget->addItems(getBookListDialog.selectedFiles());
        //delete(ui->listWidget->takeItem(0));
    }

}

void MainWindow::convert()
{
    for(int i = 0; i < ui->listWidget->count(); ++i)
    {
        currentBook.setSource(ui->listWidget->item(i)->text());
        currentBook.convert();
    }
}

void MainWindow::remove()
{
    qDeleteAll(ui->listWidget->selectedItems());
}


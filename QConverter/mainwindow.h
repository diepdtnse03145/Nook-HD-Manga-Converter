#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "book.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void addBook();
    void convert();
    void remove();

private:
    Ui::MainWindow *ui;
    Book *currentBook;
    QString lastSelectDir;
};

#endif // MAINWINDOW_H


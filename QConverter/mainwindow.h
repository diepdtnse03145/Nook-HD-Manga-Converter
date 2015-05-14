#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "book.h"

namespace Ui {
class MainWindow;
}

class QStateMachine;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void addBook();
    void convert();

signals:
    void finished();
    void started();
    void completed(int percent);

private:
    Ui::MainWindow *ui;
    QString lastSelectDir;
    QStateMachine *stateMachine;
};

#endif // MAINWINDOW_H


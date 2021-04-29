#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTcpSocket>
#include <QHostAddress>
#include <QDebug>
#ifdef Q_OS_WIN
#include <Windows.h>
#endif

#define HOST "127.0.0.1"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QVBoxLayout *vl;
    QHBoxLayout *hl[8];
    QTcpSocket *socket;

private slots:
    void OnSocketRead();
};

#endif // MAINWINDOW_H

#include "mainwindow.h"
#include "ui_mainwindow.h"

const char* keys[8][5] = {
    {"1","2","3","4","5"},
    {"Q","W","E","R","T"},
    {"A","S","D","F","G"},
    {"0","9","8","7","6"},
    {"P","O","I","U","Y"},
    {"LEFTSHIFT","Z","X","C","V"},
    {"ENTER","L","K","J","H"},
    {"SPACE","LEFTCTRL","M","N","B"}
};

QPushButton *buttons[8][5];

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    // Setup window
    vl = new QVBoxLayout(this);
    for(int i=0;i<8;i++) {
        hl[i] = new QHBoxLayout(this);
        for(int j=0;j<5;j++) {
            buttons[i][j] = new QPushButton(this);
            buttons[i][j]->setText(keys[i][j]);
            hl[i]->addWidget(buttons[i][j]);
        }
        vl->addLayout(hl[i]);
    }
    ui->centralWidget->setLayout(vl);

    // Setup socket and connect straight away
    socket = new QTcpSocket(this);
    connect(socket, SIGNAL(readyRead()), this, SLOT(OnSocketRead()));

    socket->connectToHost(QHostAddress(HOST), 16384);
    // If we connected, send a hello
    if(socket->isOpen()) {
        socket->write("hello",5);
    }

}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::OnSocketRead() {
    QByteArray result;
    result = socket->read(160);
    int c = 0;
    for(int i=0;i<8;i++) {
        for(int j=0;j<5;j++) {
          if(result.at(c) == 0) buttons[i][j]->setText(keys[i][j]);
          else buttons[i][j]->setText("*");
          c++;
        }
    }
}



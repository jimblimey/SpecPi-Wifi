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

#ifdef Q_OS_WIN
const int vkeys[8][5] = {
    {0x31,0x32,0x33,0x34,0x35},
    {0x51,0x57,0x45,0x52,0x54},
    {0x41,0x53,0x44,0x46,0x47},
    {0x30,0x39,0x38,0x37,0x36},
    {0x50,0x4F,0x49,0x55,0x59},
    {VK_LSHIFT,0x5A,0x58,0x43,0x56},
    {VK_RETURN,0x4C,0x4B,0x4A,0x48},
    {VK_SPACE,VK_LCONTROL,0x4D,0x4E,0x42}
};
#endif

int keytrack[8][5] = {
    {0,0,0,0,0},
    {0,0,0,0,0},
    {0,0,0,0,0},
    {0,0,0,0,0},
    {0,0,0,0,0},
    {0,0,0,0,0},
    {0,0,0,0,0},
    {0,0,0,0,0}
};

QPushButton *buttons[8][5];

#ifdef Q_OS_WIN
UINT sendkeypress(int key, int keyup) {
    INPUT inputs[1] = {};
    inputs[0].type = INPUT_KEYBOARD;
    inputs[0].ki.wVk = key;
    if(keyup == KEYEVENTF_KEYUP) inputs[0].ki.dwFlags = KEYEVENTF_KEYUP;
    UINT uSent = SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
    return uSent;
}
#endif

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
    if(socket->isWritable()) {
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
    int isFree;
    for(int i=0;i<8;i++) {
        for(int j=0;j<5;j++) {
            isFree = result.at(c);
            if(result.at(c) == 0) buttons[i][j]->setText(keys[i][j]);
            else buttons[i][j]->setText("*");
            // Key down
            if(isFree == 1 && keytrack[i][j] == 0) {
                keytrack[i][j] = 1;
                sendkeypress(vkeys[i][j], 0);
            }
            // Key up
            else if(isFree == 0 && keytrack[i][j] == 1) {
                keytrack[i][j] = 0;
                sendkeypress(vkeys[i][j], KEYEVENTF_KEYUP);
            }
            c++;
        }
    }
}



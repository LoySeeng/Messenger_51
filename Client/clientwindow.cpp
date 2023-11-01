#include "clientwindow.h"
#include "ui_clientwindow.h"

ClientWindow::ClientWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ClientWindow)
{
    ui->setupUi(this);

    client_socket = new QTcpSocket(this);

    connect(client_socket, &QTcpSocket::disconnected, client_socket, &QTcpSocket::deleteLater);

    client_socket->connectToHost("127.0.0.1", 3333);
}



ClientWindow::~ClientWindow()
{
    delete ui;
}


void ClientWindow::on_Enter_pushButton_clicked()
{
    CheckID = new Enter_Form(client_socket, nullptr);
    CheckID->show();
}


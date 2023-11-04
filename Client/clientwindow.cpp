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

    ui->welcome_label->setText(tr("Welcome to 'Messenger 51'"));
    ui->welcome_label->setAlignment(Qt::AlignCenter);
    ui->Enter_pushButton->setText(tr("Enter the Area"));
}

void ClientWindow::on_Enter_pushButton_clicked()
{
    CheckID = new Enter_Form(client_socket, nullptr);
    CheckID->setWindowTitle("Messenger 51");
    CheckID->show();
}



ClientWindow::~ClientWindow()
{
    delete ui;
}

std::shared_ptr<QTcpSocket *> ClientWindow::get_socket() const
{

}


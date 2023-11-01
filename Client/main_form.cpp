#include "main_form.h"
#include "ui_main_form.h"

Main_Form::Main_Form(QTcpSocket * socket, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Main_Form)
{
    ui->setupUi(this);

    this->socket = socket;

    connect(socket, &QTcpSocket::readyRead, this, &Main_Form::onSocketReadyRead);
    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);

}

Main_Form::~Main_Form()
{
    delete ui;
}

void Main_Form::onSocketReadyRead()
{
    Data.clear();
    Data = socket->read(socket->bytesAvailable());
    QString incom(Data);

    QStringList lists;

    QRegularExpression regExp("^code:(\\d{0,2})");
    QRegularExpressionMatchIterator code_match = regExp.globalMatch(incom);
    while(code_match.hasNext()) {
        QRegularExpressionMatch match = code_match.next();
        QString list = match.captured(1);
        lists << list;
    }

    switch (lists.at(1).toInt()) {
    case 1: {
        // получение сообщений
    }
    }
}

void Main_Form::on_send_all_pushButton_clicked()
{
    QString message = ui->mes_lineEdit->text();
    QString sender = "Alex";
    QString mess_toServer = "code:03login_send:" + sender + "login_to:ALLmessage:" + message;
    SendtoServer(mess_toServer);
}


void Main_Form::on_send_private_pushButton_clicked()
{
    QString message = ui->mes_lineEdit->text();
    if (message != "") {
        QString sender = "Alex";
        QString receiver = "XmaX";
        QString mess_toServer = "code:03login_send:" + sender + "login_to:" + receiver + "message:" + message;
        SendtoServer(mess_toServer);
    }
}


void Main_Form::on_refresh_pushButton_clicked()
{
    QString mess_toServer = "code:04";
    SendtoServer(mess_toServer);
}

void Main_Form::SendtoServer(QString mes_out)
{
    Data.clear();
    QDataStream out(&Data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_6);
    out << mes_out;
    socket->write(Data);
}


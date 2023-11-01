#include "enter_form.h"
#include "ui_enter_form.h"

Enter_Form::Enter_Form(QTcpSocket * socket, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Enter_Form)
{
    ui->setupUi(this);

    this->socket = socket;

    connect(socket, &QTcpSocket::readyRead, this, &Enter_Form::onSocketReadyRead);
    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);

}

Enter_Form::~Enter_Form()
{
    delete ui;
}

void Enter_Form::SendtoServer(QString mes_out)
{
    Data.clear();
    QDataStream out(&Data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_6);
    out << mes_out;
    socket->write(Data);
}

void Enter_Form::onSocketReadyRead()
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
        QStringList lists_ID;
        QRegularExpression regExpID("login:(.+)password:(.+)$");
        QRegularExpressionMatchIterator ID_match = regExpID.globalMatch(incom);
        while(ID_match.hasNext()) {
            QRegularExpressionMatch match = ID_match.next();
            QString list = match.captured(1);
            lists_ID << list;
        }
        if (lists_ID.at(1).toInt() != 0) {
            userID = lists.at(1).toInt();
            Start_Chat = new Main_Form(socket, nullptr);
            Start_Chat->show();
        }
        else {
            qDebug() << tr("login failed");
        }
        break;
    }
    }
}


void Enter_Form::on_login_OK_pushButton_clicked()
{
    if (socket->state() == QTcpSocket::ConnectedState) {
        if(ui->toRegist_pushButton->isChecked()) {
            QString login = ui->login_lineEdit->text();
            QString pass = ui->pass_lineEdit->text();
            QString request = "code:02login" + login + "password:" + pass;
            SendtoServer(request);
            }
        else {
            QString login = ui->login_lineEdit->text();
            QString pass = ui->pass_lineEdit->text();
            QString request = "code:01login" + login + "password:" + pass;
            SendtoServer(request);
        }
    }
}



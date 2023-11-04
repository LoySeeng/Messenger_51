#include "enter_form.h"
#include "ui_enter_form.h"

Enter_Form::Enter_Form(QTcpSocket * socket, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Enter_Form)
{
    ui->setupUi(this);

    this->e_socket = socket;

    connect(e_socket, &QTcpSocket::readyRead, this, &Enter_Form::onSocketReadyRead);
    connect(e_socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);

    ui->welcome_label->setText(tr("Trying to enter secret facility..."));
    ui->welcome_label->setAlignment(Qt::AlignCenter);
    ui->toRegist_pushButton->setText(tr("I'm kinda new here"));
    ui->toRegist_pushButton->setToolTip(tr("Сlick this button to register"));
    ui->login_OK_pushButton->setText(tr("Let's go!"));
    ui->login_NO_pushButton->setText(tr("Turn away"));
}

int Enter_Form::UserID() const
{
    return e_userID;
}

QString Enter_Form::username() const
{
    return e_login;
}

void Enter_Form::SendtoServer(QString mes_out)
{
    Data.clear();
    QDataStream out(&Data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_6);
    out << mes_out;
    e_socket->write(Data);
}

void Enter_Form::onSocketReadyRead()
{
    Data.clear();
    QString incom = "";
    QDataStream in(e_socket);
    in.setVersion(QDataStream::Qt_6_6);
    if(in.status() == QDataStream::Ok) {
        in >> incom;

        QStringList lists, lists_ID;

        // finding out what type of response came from the Server: 01 - responce about (un)successful try to log in
        QRegularExpression regExp("^code:(\\d{0,2})");
        QRegularExpressionMatchIterator code_match = regExp.globalMatch(incom);
        while(code_match.hasNext()) {
            QRegularExpressionMatch match1 = code_match.next();
            QString list1 = match1.captured(1);
            lists << list1;
        }

        // isolating ID and login from the response fron Server
        QRegularExpression regExpID("ID:(.+)login:(.+)$");
        QRegularExpressionMatchIterator ID_match = regExpID.globalMatch(incom);
        while(ID_match.hasNext()) {
            QRegularExpressionMatch match2 = ID_match.next();
            QString list2 = match2.captured(1);
            lists_ID << list2;
        }

        switch (lists.at(1).toInt()) {
        // code:01 - information about was the user able to log in and if he could, returns his ID and login
        case 1: {
            // 0 instead of ID means that the attempt was unsuccessful
            if (lists_ID.at(1).toInt() != 0) {
                e_userID = lists.at(1).toInt();
                e_login = lists.at(2);
                // if everything is okay, create the main window of chat
                Start_Chat = new Main_Form(e_socket, this->UserID(), this->username(), nullptr);
                Start_Chat->setWindowTitle("Messenger 51");
                Start_Chat->show();
            }
            else {
                // just a little SCP reference
                QMessageBox::warning(this, "Fall to login or register", tr("[REDACTED] prevented the entrance"));
            }
            break;
        }
        }
    }
    else {
        QMessageBox::critical(this, "DataStream error", tr("Connection with the outside world is cut off!"));
    }
}

void Enter_Form::on_toRegist_pushButton_clicked()
{
    if(ui->toRegist_pushButton->isChecked()) {
        ui->toRegist_pushButton->setStyleSheet("color: rgb(159, 80, 239)");
    }
    else if(!ui->toRegist_pushButton->isChecked()) {
        ui->toRegist_pushButton->setStyleSheet("color: green");
    }
}

void Enter_Form::on_login_OK_pushButton_clicked()
{
    if (e_socket->state() == QTcpSocket::ConnectedState) {
        if(ui->toRegist_pushButton->isChecked()) {
            QString login = ui->login_lineEdit->text();
            QString pass = ui->pass_lineEdit->text();
            // sending a message to the server with the operation code: 02 - registration attempt
            QString request = "code:02login" + login + "password:" + pass;
            SendtoServer(request);
            }
        else {
            QString login = ui->login_lineEdit->text();
            QString pass = ui->pass_lineEdit->text();
            // sending a message to the server with the operation code: 01 - login attempt
            QString request = "code:01login" + login + "password:" + pass;
            SendtoServer(request);
        }
    }
    else {
        QMessageBox::critical(this, "Server is not powered up", "Seems like no one is at work yet");
    }
}

void Enter_Form::on_login_NO_pushButton_clicked()
{
    ui->login_lineEdit->clear();
    ui->pass_lineEdit->clear();
}

Enter_Form::~Enter_Form()
{
    delete ui;
}


#include "main_form.h"
#include "ui_main_form.h"

Main_Form::Main_Form(QTcpSocket * socket, int userID, QString username, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Main_Form),
    m_userID(userID),
    m_username(username)
{
    ui->setupUi(this);

    this->m_socket = socket;

    connect(m_socket, &QTcpSocket::readyRead, this, &Main_Form::onSocketReadyRead);
    connect(m_socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);

    ui->your_mes_label->setText(tr("Your message:"));
    ui->send_all_pushButton->setText(tr("Send to all"));
    ui->send_private_pushButton->setText(tr("Send Private"));
    ui->refresh_pushButton->setText(tr("Refresh"));

}

void Main_Form::SendtoServer(QString mes_out)
{
    Data.clear();
    QDataStream out(&Data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_6);
    out << mes_out;
    m_socket->write(Data);
}

void Main_Form::onSocketReadyRead()
{
    Data.clear();
    QString incom = "";
    QDataStream in(m_socket);
    in.setVersion(QDataStream::Qt_6_6);
    if(in.status() == QDataStream::Ok) {
        in >> incom;

        QStringList lists, lists_group_mess, lists_privat_mess, lists_All_Users;

        // finding out what type of response came from the Server: 05 - responce with group message, 06 - responce with private message
        QRegularExpression regExp("^code:(\\d{0,2})");
        QRegularExpressionMatchIterator code_match = regExp.globalMatch(incom);
        while(code_match.hasNext()) {
            QRegularExpressionMatch match1 = code_match.next();
            QString list = match1.captured(1);
            lists << list;
        }

        // isolating sender's of group message login and the message itself from the response fron Server
        QRegularExpression regExpGrMes("login_from:(.+)message:(.+)$");
        QRegularExpressionMatchIterator GM_match = regExpGrMes.globalMatch(incom);
        while(GM_match.hasNext()) {
            QRegularExpressionMatch match2 = GM_match.next();
            QString list2 = match2.captured(1);
            lists_group_mess << list2;
        }

        // isolating sender's of private message login and the message itself from the response fron Server
        QRegularExpression regExpPrMes("login_from_private:(.+)message:(.+)$");
        QRegularExpressionMatchIterator PM_match = regExpPrMes.globalMatch(incom);
        while(PM_match.hasNext()) {
            QRegularExpressionMatch match3 = PM_match.next();
            QString list3 = match3.captured(1);
            lists_privat_mess << list3;
        }

        // isolating usernames from the response fron Server
        QRegularExpression regExpUser("user:(.+)$");
        QRegularExpressionMatchIterator user_match = regExpUser.globalMatch(incom);
        while(user_match.hasNext()) {
            QRegularExpressionMatch match4 = PM_match.next();
            QString list4 = match4.captured(1);
            lists_All_Users << list4;
        }


        switch (lists.at(1).toInt()) {
        // receiving group messages from the Server
        case 5: {
            QString sender = lists_group_mess.at(1);
            QString mes = lists_group_mess.at(2);
            QString group_mes = "<" + sender + ">: '" + mes + "'";
            ui->group_textBrowser->append(group_mes);
            break;
        }
        // receiving private messages from the Server
        case 6: {
            QString sender = lists_privat_mess.at(1);
            QString mes = lists_privat_mess.at(2);
            if (sender == m_username) {
                QString private_mes = "self message: '" + mes + "'";
                ui->private_textBrowser->append(private_mes);
            }
            else {
                QString private_mes = "<" + sender + ">: '" + mes + "'";
                ui->private_textBrowser->append(private_mes);
            }
            break;
        }
        // getting a list of users from the server
        case 7:
            QString user = lists_All_Users.at(1);
            All_users.insert(user);
            break;
        }
    }
    else {
        QMessageBox::critical(this, "DataStream error", tr("Connection with the outside world is cut off!"));
    }
}

void Main_Form::on_send_all_pushButton_clicked()
{
    QString message = ui->mes_lineEdit->text();
    if(!message.isEmpty()) {
        QString sender = m_username;
        QString mess_toServer = "code:03login_send:" + sender + "login_to:ALLmessage:" + message;
        SendtoServer(mess_toServer);
    }
    else {
        QMessageBox::question(this, "Message can not be empty", tr("Are you trying to send wordless side eye or something?"));
    }
}


void Main_Form::on_send_private_pushButton_clicked()
{
    QString message = ui->mes_lineEdit->text();
    if (!message.isEmpty()) {
        QString mess_toServer1 = "code:05";
        SendtoServer(mess_toServer1);

        QString sender = m_username;

        QDialog dial(this);
        dial.setModal(true);
        auto l = new QVBoxLayout();
        dial.setLayout(l);
        auto userListWgt = new QListWidget(&dial);
        l->addWidget(userListWgt);
        auto buttonBox = new  QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dial);
        l->addWidget(buttonBox);
        connect(buttonBox, &QDialogButtonBox::accepted, &dial, &QDialog::accept);
        connect(buttonBox, &QDialogButtonBox::rejected, &dial, &QDialog::reject);
        for (const auto &value : All_users) {
            userListWgt->addItem(value);
        }
        userListWgt->setCurrentRow(0);
        auto result = dial.exec();
        if(result == QDialog::Accepted && userListWgt->currentItem()) {
            QString receiver = userListWgt->currentItem()->text();
            QString mess_toServer2 = "code:03login_send:" + sender + "login_to:" + receiver + "message:" + message;
            SendtoServer(mess_toServer2);
        }
    }
    else {
        QMessageBox::question(this, "Message can not be empty", tr("Are you trying to send wordless side eye or something?"));
    }
}


void Main_Form::on_refresh_pushButton_clicked()
{
    ui->group_textBrowser->clear();
    ui->private_textBrowser->clear();
    QString mess_toServer = "code:04for:" + m_userID;
    SendtoServer(mess_toServer);
}


Main_Form::~Main_Form()
{
    delete ui;
}


#include "serverwindow.h"
#include "ui_serverwindow.h"


ServerWindow::ServerWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ServerWindow)
{
    ui->setupUi(this);

}

void ServerWindow::power_server()
{
    server = new QTcpServer(this);
    server->setMaxPendingConnections(10);

    if(server->listen(QHostAddress::Any, 3333)) {
        qDebug() << tr("Server power up");
    }
    else {
        qDebug() << tr("Can not power up");
    }

    DBase = QSqlDatabase::addDatabase("QSQLITE");
    DBase.setDatabaseName("D:/Programming/QT_projects/Server/MESSENGER_51.db");

    if(DBase.open()) {
        qDebug() << "Yes";
    }
    else {
        qDebug() << "No :(";
    }
}

int ServerWindow::getIDbyName(QString name)
{
    QSqlQuery query;
    int ID = -1;
    query.prepare("SELECT id_user FROM users WHERE login = (:_login)");
    query.bindValue(":_login", name);
    if (query.exec()) {
        if(query.next()) {
            ID = query.value(0).toInt();
            return ID;
        }
    }
    return ID;
}

void ServerWindow::SendtoClient(QString mes_to_client)
{
    Data.clear();
    QDataStream out(&Data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_6);
    out << mes_to_client;
    socket->write(Data);
}

QString ServerWindow::getNamebyID(int user_ID)
{
    QSqlQuery query;
    QString username = "";
    query.prepare("SELECT login FROM users WHERE id_user = (:_id)");
    query.bindValue(":_id", user_ID);
    if (query.exec()) {
        if(query.next()) {
            username = query.value(0).toString();
            return username;
        }
    }
    return username;
}

void ServerWindow::addGroupMessage(QString sender, QString mes)
{
    int ID = getIDbyName(sender);
    if (ID != -1) {
        QSqlQuery query;
        query.prepare("INSERT INTO group_messages (id_sender, message) VALUES (?, ?)");
        query.addBindValue(ID);
        query.addBindValue(mes);
        query.exec();
    }
}

void ServerWindow::addPrivateMessage(QString sender, QString receiever, QString mes)
{
    int ID_Sender = getIDbyName(sender);
    int ID_Receiever = getIDbyName(receiever);
    if ((ID_Sender != -1) && (ID_Receiever != -1)) {
        QSqlQuery query;
        query.prepare("INSERT INTO private_messages (id_sender, id_receiver, message) VALUES (?, ?, ?)");
        query.addBindValue(ID_Sender);
        query.addBindValue(ID_Receiever);
        query.addBindValue(mes);
        query.exec();
    }
}

QVector<QString> ServerWindow::getGroupMessClient()
{
        QVector<QString> G_mes;

        if(DBase.open()) {
            qDebug() << "Yes";
            int ID_sender;
            QString mes, name_sender;
            QSqlQuery query;
            query.exec("SELECT id_sender, message FROM group_messages");
            while (query.next()) {
                ID_sender = query.value(0).toInt();
                mes = query.value(1).toString();
                name_sender = getNamebyID(ID_sender);
                G_mes.push_back("code:05login_from:" + name_sender + "message:" + mes);
            }
            return G_mes;
        }
        else {
            qDebug() << "No :(";
        }
        return G_mes;
}

void ServerWindow::incomingConnection(qintptr socketDescriptor)
{
    socket = new QTcpSocket;
    socket->setSocketDescriptor(socketDescriptor);
    connect(socket, &QTcpSocket::readyRead, this, &ServerWindow::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);

    Users_Sockets.push_back(socket);
    qDebug() << tr("Client connected ") << socketDescriptor;
}

void ServerWindow::slotReadyRead()
{
    QTcpSocket * obj = qobject_cast<QTcpSocket*>(sender());
    QByteArray data = obj->read(obj->bytesAvailable());

     qDebug() << tr("Reading...");

    QString incom(data);

        QString in_login, pass, message, inscriptionLogin, inscriptionPass, sender, receiver;

        QStringList lists, lists_login, lists_messages;

        QRegularExpression regExp("^code:(\\d{0,2})");
        QRegularExpressionMatchIterator code_match = regExp.globalMatch(incom);
        while(code_match.hasNext()) {
            QRegularExpressionMatch match = code_match.next();
            QString list = match.captured(1);
            lists << list;
        }

        QRegularExpression regExpLog("login:(.+)password:(.+)$");
        QRegularExpressionMatchIterator login_match = regExpLog.globalMatch(incom);
        while(login_match.hasNext()) {
            QRegularExpressionMatch match = login_match.next();
            QString list = match.captured(1);
            lists_login << list;
        }

        QRegularExpression regExpMes("login_send:(.+)login_to:(.+)message:(.+)$");
        QRegularExpressionMatchIterator mes_match = regExpMes.globalMatch(incom);
        while(mes_match.hasNext()) {
            QRegularExpressionMatch match = mes_match.next();
            QString mes = match.captured(1);
            lists_messages << mes;
        }

        switch(lists.at(1).toInt()) {
        case 1: {
            in_login = lists_login.at(1);
            pass = lists_login.at(2);

            QString response = LOG_IN(in_login, pass);

            if(response != "0") {
                SendtoClient(response);
            }
            else {
                SendtoClient("code:01ID:0");
            }
            break;
        }
        case 2: {
            inscriptionLogin = lists_login.at(1);
            inscriptionPass = lists_login.at(2);
            QString responce = addUser(inscriptionLogin, inscriptionPass);

            SendtoClient(responce);

            break;
        }
        case 3: {
            receiver = lists_messages.at(2);
            sender = lists_messages.at(1);
            message = lists_messages.at(3);
            if (receiver == "ALL") {
                addGroupMessage(sender, message);
            }
            else {
                addPrivateMessage(sender, receiver, message);
            }
            break;
        }
        case 4: {
            QVector<QString> from_group = getGroupMessClient();
            for (int q; q <from_group.size(); q++) {
                SendtoClient(from_group[q]);
                // добавить личные сообщения
            }
            break;
        }
    }
}


QString ServerWindow::LOG_IN(QString login, QString pass)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM users WHERE login = '(:_login)' AND password = '(:_pass)' limit 1");
    query.bindValue(":_login", login);
    query.bindValue(":_pass", pass);
    if(query.exec()) {
        while(query.next()) {
            QString ID = query.value(0).toString().toUtf8();
            QString answer = "code01ID:" + ID + "login:" + login;
            return answer;
        }
    }
    else {
        return "0";
    }
    return "0";
}

QString ServerWindow::addUser(QString login, QString pass)
{
    if (LOG_IN(login, pass) == "0") {
        QSqlQuery query_in;
        QSqlQuery query_out;
        query_in.prepare("INSERT INTO users (login, password) VALUES (?, ?)");
        query_in.addBindValue(login);
        query_in.addBindValue(pass);
        if(query_in.exec()) {
            query_out.prepare("SELECT id_user FROM users WHERE login = '(:_login)' AND password = '(:_pass)'");
            query_out.bindValue(":_login", login);
            query_out.bindValue(":_pass", pass);
            query_out.exec();
            while(query_out.next()) {
                QString ID = query_out.value(0).toString();
                QString answer = "code:01ID:" + ID + "login:" + login;
                return answer;
            }
        }
        else {
            return "code:01ID:0";
        }
    }
    return "code:01ID:0";
}

void ServerWindow::on_power_server_pushButton_clicked()
{
    power_server();
    ui->power_server_pushButton->setEnabled(false);
}

void ServerWindow::on_enter_moder_pushButton_clicked()
{
    BasementEnter = new Login_moder_Form();
    BasementEnter->show();
}

ServerWindow::~ServerWindow()
{
    delete ui;
}







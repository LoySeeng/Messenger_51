#include "serverwindow.h"
#include "ui_serverwindow.h"


ServerWindow::ServerWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ServerWindow)
{
    ui->setupUi(this);

    ui->welcome_label->setText(tr("Welcome to 'Messenger 51'"));
    ui->welcome_label->setAlignment(Qt::AlignCenter);
    ui->power_server_pushButton->setText(tr("Power up Server"));
    ui->enter_moder_pushButton->setText(tr("Show moderator pass"));

}

void ServerWindow::power_server()
{
    server = new QTcpServer(this);
    server->setMaxPendingConnections(10);

    if(server->listen(QHostAddress::Any, 3333)) {
        qDebug() << tr("Server is powered up");
    }
    else {
        // just a little SCP reference
        QMessageBox::critical(this, "Can not power up Server", tr("[REDACTED] is messing up Server again..."));
    }

    DBase = QSqlDatabase::addDatabase("QSQLITE");
    // !! Before the app startup make sure to specify your FULL path to the database !!
    DBase.setDatabaseName("D:/Programming/QT_projects/Server/MESSENGER_51.db");

    if(DBase.open()) {
        qDebug() << "Database connection is opened";
    }
    else {
        // just a little SCP reference
        QMessageBox::critical(this, "Can not open up Database", tr("Seems like Archivist never came"));
    }
}

void ServerWindow::incomingConnection(qintptr socketDescriptor)
{
    socket = new QTcpSocket;
    socket->setSocketDescriptor(socketDescriptor);
    connect(socket, &QTcpSocket::readyRead, this, &ServerWindow::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);

    Users_Sockets.push_back(socket);
    qDebug() << tr("Client connected: ") << socketDescriptor;
}

void ServerWindow::SendtoClient(QString mes_to_client)
{
    Data.clear();
    QDataStream out(&Data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_6);
    out << mes_to_client;
    socket->write(Data);
}


// functions for accessing the database

int ServerWindow::getIDbyName(QString name) // on success return user ID otherwise -1
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
    else {
        qDebug() << "Something is wrong with 'getIDbyName' function";
    }
    return ID;
}

QString ServerWindow::getNamebyID(int user_ID) // return user login otherwise empty string
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
    else {
        qDebug() << "Something is wrong with 'getNamebyID' function";
    }
    return username;
}

// checks whether the user was able to log in
QString ServerWindow::LOG_IN(QString login, QString pass) // return 'code:01ID:_login:_' otherwise 0
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

// adds a user when registering is required
QString ServerWindow::addUser(QString login, QString pass) // return 'code:01ID:_login:_' otherwise '01ID:0'
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
                QString ID = query_out.value(0).toString().toUtf8();
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

// adds a group message to the database
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
    else {
        qDebug() << "Something is wrong with 'addGroupMessage' function";
    }
}

// adds a private message to the database
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
    else {
        qDebug() << "Something is wrong with 'addPrivateMessage' function";
    }
}

// retrieves for client all group messages from the database
QVector<QString> ServerWindow::getGroupMessClient()
{
    QVector<QString> G_mes;

    if(DBase.open()) {
        qDebug() << "Database is still running";
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
        qDebug() << "We lost connection to Database";
    }
    return G_mes;
}

// retrieves for client all their private messages from the database
QVector<QString> ServerWindow::getPrivateMessClient(uint ID_user_to)
{
    QVector<QString> P_mes;

    if(DBase.open()) {
        qDebug() << "Database is still running";
        int ID_sender;
        QString mes, name_sender;
        QSqlQuery query;
        query.exec("SELECT id_sender, message FROM private_messages WHERE id_receiver = (:_id)");
        query.bindValue(":_id", ID_user_to);
        while (query.next()) {
            ID_sender = query.value(0).toInt();
            mes = query.value(1).toString();
            name_sender = getNamebyID(ID_sender);
            P_mes.push_back("code:06login_from_private:" + name_sender + "message:" + mes);
        }
        return P_mes;
    }
    else {
        qDebug() << "We lost connection to Database";
    }
    return P_mes;
}

// retrieves for client list of all users from the database
QVector<QString> ServerWindow::getAllUsers()
{
    QVector<QString> USERS;
    if(DBase.open()) {
        qDebug() << "Database is still running";
        QSqlQuery query;
        query.exec("SELECT login FROM users");
        while (query.next()) {
            QString name = query.value(0).toString();
            USERS.push_back("code:07user:" + name);
        }
        return USERS;
    }
    else {
        qDebug() << "We lost connection to Database";
    }
    return USERS;
}

void ServerWindow::slotReadyRead()
{
    Data.clear();
    QString incom = "";
    socket = (QTcpSocket*)sender();
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_6_6);
    if(in.status() == QDataStream::Ok) {
         qDebug() << tr("Server is reading...");

        in >> incom;

        QString in_login, pass, message, inscriptionLogin, inscriptionPass, sender, receiver;

        QStringList lists, lists_login, lists_messages, lists_get_mes, lists_get_users;

        // finding out what type of response came from the Client:
        // 01 - responce from client with login attempt, 02 - responce with registration attempt, 03 - responce with message from client
        // 04 - responce with request to send all messages, 05 - responce with request to send lists of all users
        QRegularExpression regExp("^code:(\\d{0,2})");
        QRegularExpressionMatchIterator code_match = regExp.globalMatch(incom);
        while(code_match.hasNext()) {
            QRegularExpressionMatch match1 = code_match.next();
            QString list1 = match1.captured(1);
            lists << list1;
        }

        // isolating login and password from the response from Client (for login / registration)
        QRegularExpression regExpLog("login:(.+)password:(.+)$");
        QRegularExpressionMatchIterator login_match = regExpLog.globalMatch(incom);
        while(login_match.hasNext()) {
            QRegularExpressionMatch match2 = login_match.next();
            QString list2 = match2.captured(1);
            lists_login << list2;
        }

        // isolating login of sender and receiver, message from the response from Client (for adding message to database)
        QRegularExpression regExpMes("login_send:(.+)login_to:(.+)message:(.+)$");
        QRegularExpressionMatchIterator mes_match = regExpMes.globalMatch(incom);
        while(mes_match.hasNext()) {
            QRegularExpressionMatch match3 = mes_match.next();
            QString mes1 = match3.captured(1);
            lists_messages << mes1;
        }

        // isolating ID of user who requested all messages from the response from Client (for sending all messages from database)
        QRegularExpression regExpGetMes("for:(.+)$");
        QRegularExpressionMatchIterator get_mes_match = regExpGetMes.globalMatch(incom);
        while(get_mes_match.hasNext()) {
            QRegularExpressionMatch match4 = get_mes_match.next();
            QString mes2 = match4.captured(1);
            lists_get_mes << mes2;
        }


        switch(lists.at(1).toInt()) {
        // code:01 - sendin back information about was the user able to log in and if they could, returns their ID and login
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
        // code:02 - sendin back information about was the user able to register and if they could, returns their ID and login
        case 2: {
            inscriptionLogin = lists_login.at(1);
            inscriptionPass = lists_login.at(2);

            QString responce = addUser(inscriptionLogin, inscriptionPass);

            SendtoClient(responce);

            break;
        }
        // code:03 - adding group or privatr message from Client to database
        case 3: {
            sender = lists_messages.at(1);
            receiver = lists_messages.at(2);
            message = lists_messages.at(3);
            if (receiver == "ALL") {
                addGroupMessage(sender, message);
            }
            else {
                addPrivateMessage(sender, receiver, message);
            }
            break;
        }
        // code:04 - sending back group and private messages received from the database
        case 4: {
            QVector<QString> from_group = getGroupMessClient();
            for (int g = 0; g <from_group.size(); g++) {
                SendtoClient(from_group[g]);
            }

            uint priv_mes_receiver_ID = lists_get_mes.at(1).toInt();
            QVector<QString> from_private = getPrivateMessClient(priv_mes_receiver_ID);
            for (int p = 0; p <from_private.size(); p++) {
                SendtoClient(from_private[p]);
            }
            break;
        }
        // code:05 - sending back lists of all users received from the database
        case 5: {
            QVector<QString> all_users = getAllUsers();
            for (int i = 0; i < all_users.size(); i++) {
                SendtoClient(all_users[i]);
            }
            break;
        }
        }
    }
    else {
        QMessageBox::critical(this, "DataStream error", tr("Connection with the outside world is cut off!"));
    }
}


void ServerWindow::on_power_server_pushButton_clicked()
{
    power_server();
    ui->power_server_pushButton->setEnabled(false);
    ui->power_server_pushButton->setStyleSheet("color: rgb(159, 80, 239)");
}

void ServerWindow::on_enter_moder_pushButton_clicked()
{
    if(ui->power_server_pushButton->isEnabled()) {
        QMessageBox::critical(this, "Server is not powered up", tr("You need to power up Server first!"));
    }
    else if (!ui->power_server_pushButton->isEnabled()) {
        BasementEnter = new Login_moder_Form();
        BasementEnter->setWindowTitle("Messenger 51");
        BasementEnter->show();
    }
}

ServerWindow::~ServerWindow()
{
    delete ui;
}







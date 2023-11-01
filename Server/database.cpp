#include "database.h"

DataBase::DataBase()
{
    DBase = QSqlDatabase::addDatabase("QSQLITE");
    DBase.setDatabaseName("./MESSENGER_51.db");

    if(DBase.open()) {
        qDebug() << "Yes";
    }
    else {
        qDebug() << "No :(";
    }
}

int DataBase::checkPasswordUser(QString login, QString pass)
{
    int result = -1;
    QString check_pass = "";
    if(UserExist(login)) {
        QSqlQuery query1;
        query1.prepare("SELECT password FROM users WHERE login = (:_login)");
        query1.bindValue(":_login", login);
        if(query1.exec()) {
            while(query1.next()) {
                check_pass = query1.value(0).toString();
            }
        }
        if(pass == check_pass) {
            QSqlQuery query2;
            query2.prepare("SELECT id_user FROM users WHERE login = (:_login)");
            query2.bindValue(":_login", login);
            if(query2.exec()) {
                while(query2.next()) {
                    int ID = query2.value(0).toInt();
                    result = ID;
                    return result;
                }
            }
        }
        else {
            result = -2;
        }
    }
    return result;
}

int DataBase::checkPasswordModer(QString login, QString pass)
{
    int result = -1;
    QString check_pass = "";
    if(ModerExist(login)) {
        QSqlQuery query1;
        query1.prepare("SELECT password FROM moderators WHERE login = (:_login)");
        query1.bindValue(":_login", login);
        if(query1.exec()) {
            while(query1.next()) {
                check_pass = query1.value(0).toString();
            }
        }
        if(pass == check_pass) {
            QSqlQuery query2;
            query2.prepare("SELECT id_moder FROM moderators WHERE login = (:_login)");
            query2.bindValue(":_login", login);
            if(query2.exec()) {
                while(query2.next()) {
                    int ID = query2.value(0).toInt();
                    result = ID;
                    return result;
                }
            }
        }
        else {
            result = -2;
        }
    }
    return result;
}

int DataBase::addUser(QString login, QString pass)
{
    int ID = -1;
    if(!UserExist(login)) {
        QSqlQuery query;
        query.prepare("INSERT INTO users (login, password) VALUES (?, ?)");
        query.addBindValue(login);
        query.addBindValue(pass);
        if(query.exec()) {
            ID = getIDbyName(login);
            return ID;
        }
    }
    return ID;
}

int DataBase::deleteUser(QString login)
{
    if(UserExist(login)) {
        QSqlQuery query;
        query.prepare("DELETE FROM users WHERE login = (:_login)");
        query.bindValue(":_login", login);
        if (query.exec()) {
            return 1;
        }
        else {
            return -2;
        }
    }
    else {
        return -1;
    }

}

void DataBase::addGroupMessage(QString sender, QString mes)
{
    int ID = getIDbyName(sender);
    QSqlQuery query;
    query.prepare("INSERT INTO group_messages (id_sender, message) VALUES (?, ?)");
    query.addBindValue(ID);
    query.addBindValue(mes);
    query.exec();

}

QVector<QString> DataBase::getGroupMessages()
{
    QVector<QString> G_mess;
    QSqlQuery query;
    query.exec("SELECT id_sender, message FROM group_messages");
    while (query.next()) {
        int ID = query.value(0).toInt();
        QString mes = query.value(1).toString();
        QString name = getNamebyID(ID);
        G_mess.push_back("<" + name + ">: " + "'" + mes + "'");
    }
    return G_mess;
}

QVector<QString> DataBase::getGroupMessagesModer()
{
    QVector<QString> G_mess;
    QSqlQuery query;
    query.exec("SELECT id_sender, message FROM group_messages");
    while (query.next()) {
        int ID = query.value(0).toInt();
        QString mes = query.value(1).toString();
        QString name = getNamebyID(ID);
        G_mess.push_back("<" + name + ">: " + "'" + mes + "'");
    }
    return G_mess;
}

QVector<QString> DataBase::getAllPrivateMessages()
{
    QVector<QString> P_mess;
    QSqlQuery query;
    query.exec("SELECT id_sender, id_receiver, message FROM private_messages");
    while (query.next()) {
        int ID_sender = query.value(0).toInt();
        int ID_receiver = query.value(1).toInt();
        QString mes = query.value(2).toString();
        QString name_sender = getNamebyID(ID_sender);
        QString name_receiver = getNamebyID(ID_receiver);
        if (name_sender == name_receiver) {
            P_mess.push_back("<" + name_sender + "> " + "while talking to yourself" + "'" + mes + "'");
        }
        else {
        P_mess.push_back("<" + name_sender + ">: " + "send signal to " + "<" + name_receiver + ">: " + "'" + mes + "'");
        }
    }
    return P_mess;
}

bool DataBase::UserExist(QString login)
{
    QSqlQuery query;
    query.prepare("SELECT login FROM users WHERE login = (:_login)");
    query.bindValue(":_login", login);
    if (query.exec()) {
        if(query.next()) {
            return true;
        }
        else {
            return false;
        }
    }
    return false;
}

bool DataBase::ModerExist(QString login)
{
    QSqlQuery query;
    query.prepare("SELECT login FROM moderators WHERE login = 'Mort'");
    //query.bindValue(":_login", login);
    if (query.exec()) {
        return true;
    }
    else {
        return false;
    }
}

int DataBase::getIDbyName(QString name)
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

QString DataBase::getNamebyID(int user_ID)
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

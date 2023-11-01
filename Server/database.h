#ifndef DATABASE_H
#define DATABASE_H


#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QString>
#include <QVector>
#include <QDebug>
#include <QSqlQuery>


class DataBase
{
public:
    DataBase();

    bool is_open_base();
    void close_base();

    bool UserExist(QString login); // +
    bool ModerExist(QString login); // +
    QString getNamebyID(int login); // +
    int getIDbyName(QString name); // +
    int checkPasswordUser(QString login, QString pass); // + return ID OR -2 (if pass not match) OR -1 (else)
    int checkPasswordModer(QString login, QString pass); // + return ID OR -2 (if pass not match) OR -1 (else)
    int addUser(QString login, QString pass);
    int deleteUser(QString login); // + return 1 (ok) OR -1 (wrong name) OR -2 (else)
    void addGroupMessage(QString sender, QString mes); // + проверить работает ли так INSERT
    void addPrivateMessage(QString sender, QString reciever, QString mes);
    QVector<QString> getGroupMessages(); // + add проверку перед возвратом vector
    QVector<QString> getGroupMessagesModer();  // + add проверку перед возвратом vector
    QVector<QString> getPrivateMessages(QString login);
    QVector<QString> getAllPrivateMessages();  // + add проверку перед возвратом vector

private:
    QSqlDatabase DBase;

};
#endif // DATABASE_H

#ifndef SERVERWINDOW_H
#define SERVERWINDOW_H

// #include "database.h"
#include "login_moder_form.h"

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include <QHostAddress>
#include <QVector>
#include <QDebug>
#include <QRegularExpression>
#include <QString>
#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
#include <QMessageBox>


QT_BEGIN_NAMESPACE
namespace Ui { class ServerWindow; }
QT_END_NAMESPACE

class ServerWindow : public QMainWindow
{
    Q_OBJECT

public:
    ServerWindow(QWidget *parent = nullptr);
    ~ServerWindow();

public slots:
    void incomingConnection(qintptr socketDescriptor);
    void slotReadyRead();

private slots:
    void on_power_server_pushButton_clicked();
    void on_enter_moder_pushButton_clicked();

private:
    Ui::ServerWindow *ui;
    Login_moder_Form *BasementEnter;

private:
    QTcpServer *server;
    QTcpSocket *socket;
    QByteArray Data;
    QVector <QTcpSocket*> Users_Sockets;
    void power_server();
    void SendtoClient(QString mes_to_client);

private:
    QSqlDatabase DBase;
    int getIDbyName(QString name);   // return ID otherwise -1
    QString getNamebyID(int user_ID);   // return user login otherwise empty string
    QString LOG_IN(QString login, QString pass);   // return 'code:01ID:_login:_' otherwise 0
    QString addUser(QString login, QString pass);  // return 'code:01ID:_login:_' otherwise '01ID:0'
    void addGroupMessage(QString sender, QString mes);
    void addPrivateMessage(QString sender, QString receiever, QString mes);
    QVector<QString> getGroupMessClient();
    QVector<QString> getPrivateMessClient(uint ID_user_to);
    QVector<QString> getAllUsers();
};
#endif // SERVERWINDOW_H

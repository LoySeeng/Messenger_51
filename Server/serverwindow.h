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
    void on_power_server_pushButton_clicked(bool checked);

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
    int getIDbyName(QString name);
    QString getNamebyID(int user_ID);
    QString LOG_IN(QString login, QString pass);
    QString addUser(QString login, QString pass);
    void addGroupMessage(QString sender, QString mes);
    void addPrivateMessage(QString sender, QString receiever, QString mes);
    QVector<QString> getGroupMessClient();
};
#endif // SERVERWINDOW_H

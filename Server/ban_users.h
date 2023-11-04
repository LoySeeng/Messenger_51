#ifndef BAN_USERS_H
#define BAN_USERS_H

#include <QWidget>
#include <QString>
#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
#include <QMessageBox>
#include <QListWidget>
#include <QDialogButtonBox>


namespace Ui {
class ban_users;
}

class ban_users : public QWidget
{
    Q_OBJECT

public:
    explicit ban_users(QWidget *parent = nullptr);
    ~ban_users();

private slots:
    void on_ban_pushButton_clicked();

private:
    void deleteUser(QString login);   // on success return 1 otherwise -1
    QVector<QString> getAllUsers();

private:
    Ui::ban_users *ui;
    QSqlDatabase db;
};

#endif // BAN_USERS_H

#ifndef BAN_USERS_H
#define BAN_USERS_H

#include <QWidget>
#include <QString>
#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>

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
    void on_moder_exet_pushButton3_clicked();

    void on_comboBox_activated(int index);

private:
    int deleteUser(QString login);

private:
    Ui::ban_users *ui;
    QSqlDatabase db;
};

#endif // BAN_USERS_H

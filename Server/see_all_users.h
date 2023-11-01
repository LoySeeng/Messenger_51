#ifndef SEE_ALL_USERS_H
#define SEE_ALL_USERS_H

#include <QWidget>
#include <QVector>
#include <QString>
#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>

namespace Ui {
class see_all_users;
}

class see_all_users : public QWidget
{
    Q_OBJECT

public:
    explicit see_all_users(QWidget *parent = nullptr);
    ~see_all_users();

private slots:
    void on_user_details_pushButton4_clicked();

private:
    QVector<QString> get_users();

private:
    Ui::see_all_users *ui;
    QSqlDatabase db;
};

#endif // SEE_ALL_USERS_H

#include "ban_users.h"
#include "ui_ban_users.h"

ban_users::ban_users(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ban_users)
{
    ui->setupUi(this);

    db = QSqlDatabase::database();
}

ban_users::~ban_users()
{
    delete ui;
}

void ban_users::on_moder_exet_pushButton3_clicked()
{

}


void ban_users::on_comboBox_activated(int index)
{
    if (index == 0) {
        // disconect users ... somehow...
    }
    if (index == 1) {
        QString ban_user = ui->bonk_lineEdit->text();
        int comback = deleteUser(ban_user);
        if (comback == -1) {
            // вставить ошибки
            // и логи
        }
    }
}

int ban_users::deleteUser(QString login)
{
    int res = 0;
    if(db.open()) {
        qDebug() << "Yes";

        QSqlQuery query;
        query.prepare("DELETE FROM users WHERE login = (:_login)");
        query.bindValue(":_login", login);
        if(query.exec()) {
            res = 1;
            return res;
        }
        else {
            qDebug() << query.lastError();
            res = -1;
            return res;
        }
    }
    else {
        qDebug() << "No :(";
    }
    return res;
}


#include "see_all_users.h"
#include "ui_see_all_users.h"

see_all_users::see_all_users(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::see_all_users)
{
    ui->setupUi(this);

    db = QSqlDatabase::database();

    ui->all_users_label->setText(tr("ALL USERS:"));
    ui->user_details_pushButton4->setText(tr("receive the report"));
}

QVector<QString> see_all_users::get_users()
{
    QVector<QString> USERS;
    if(db.open()) {
        QSqlQuery query;
        query.exec("SELECT login FROM users");
        while (query.next()) {
            QString name = query.value(0).toString();
            USERS.push_back("Dear alien: " + name);
        }
        qDebug() << "Errors of see_all_users: " << query.lastError();
        return USERS;
    }
    else {
        qDebug() << "We lost connection to Database in 'see_all_users'";
    }
    return USERS;
}

void see_all_users::on_user_details_pushButton4_clicked()
{
    QVector<QString> ALL_USERS = get_users();

    for (int i = 0; i < ALL_USERS.size(); ++i) {
        ui->listWidget_USERS->addItem(ALL_USERS.at(i));
    }
}

see_all_users::~see_all_users()
{
    delete ui;
}


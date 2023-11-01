#include "login_moder_form.h"
#include "ui_login_moder_form.h"

Login_moder_Form::Login_moder_Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Login_moder_Form)
{
    ui->setupUi(this);

    db = QSqlDatabase::database();
}

Login_moder_Form::~Login_moder_Form()
{
    delete ui;
}

void Login_moder_Form::on_moder_login_pushButton_clicked()
{
    QString mod_login = ui->login_edit->text();
    QString mod_pas = ui->pas_edit->text();

    QString get_pas = getModerPass(mod_login);

    if(get_pas == mod_pas) {
        qDebug() << "Finaly";
        Basement = new Moderator_basement();
        Basement->show();
    }
    else if (get_pas == "") {
        qDebug() << "Fuck";
    }
    else {
        qDebug() << "FUCK";
    }
}

QString Login_moder_Form::getModerPass(QString login)
{
    QString PS = "";
    if(db.open()) {
        qDebug() << "Yes";

        QSqlQuery query;
        query.prepare("SELECT password FROM moderators WHERE login = (:_login)");
        query.bindValue(":_login", login);
        if(query.exec()) {
            while(query.next()) {
                QString PS = query.value(0).toString();
                return PS;
            }
            qDebug() << query.lastError();
        }
        else {
            qDebug() << query.lastError();
        }
    }
    else {
        qDebug() << "No :(";
    }
    return PS;
}


void Login_moder_Form::on_moder_NOT_login_pushButton_clicked()
{
    ui->login_edit->clear();
    ui->pas_edit->clear();
}



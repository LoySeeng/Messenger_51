#include "moderator_basement.h"
#include "ui_moderator_basement.h"

Moderator_basement::Moderator_basement(QString mod_login, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Moderator_basement),
    b_mod_login(mod_login)
{
    ui->setupUi(this);

    ui->staf_label->setText(tr("Foundation Staff: "));
    ui->moder_name_label->setText(b_mod_login);
    ui->moder_name_label->setStyleSheet("color: rgb(159, 80, 239)");
    ui->groupBox->setTitle(tr("ALL USERS"));
    ui->groupBox_2->setTitle(tr("MESSAGES"));
    ui->groupBox_3->setTitle(tr("UNWANTED SUBJECTS"));
    ui->all_users_pushButton->setText(tr("RESEARCH"));
    ui->all_messages_pushButton->setText(tr("RESEARCH"));
    ui->banned_users_pushButton->setText(tr("RESEARCH"));

}

void Moderator_basement::on_all_users_pushButton_clicked()
{
    Users = new see_all_users();
    Users->setWindowTitle("Messenger 51");
    Users->show();
}


void Moderator_basement::on_all_messages_pushButton_clicked()
{
    Messages = new see_messages();
    Messages->setWindowTitle("Messenger 51");
    Messages->show();
}


void Moderator_basement::on_banned_users_pushButton_clicked()
{
    Ban = new ban_users();
    Ban->setWindowTitle("Messenger 51");
    Ban->show();
}

Moderator_basement::~Moderator_basement()
{
    delete ui;
}


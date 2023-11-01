#include "moderator_basement.h"
#include "ui_moderator_basement.h"

Moderator_basement::Moderator_basement(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Moderator_basement)
{
    ui->setupUi(this);
}

Moderator_basement::~Moderator_basement()
{
    delete ui;
}

void Moderator_basement::on_all_users_pushButton_clicked()
{
    Users = new see_all_users;
    Users->show();
}


void Moderator_basement::on_all_messages_pushButton_clicked()
{
    Messages = new see_messages;
    Messages->show();
}


void Moderator_basement::on_banned_users_pushButton_clicked()
{
    Ban = new ban_users;
    Ban->show();
}


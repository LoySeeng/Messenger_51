#include "see_messages.h"
#include "ui_see_messages.h"

see_messages::see_messages(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::see_messages)
{
    ui->setupUi(this);

    db = QSqlDatabase::database();

    ui->group_mes_label->setText(tr("Group messages"));
    ui->private_mes_label->setText(tr("Private messages"));
    ui->pushButton->setText(tr("Update"));
}

QString see_messages::getNamebyID(int user_ID)
{
    QSqlQuery query;
    QString username;
    query.prepare("SELECT login FROM users WHERE id_user = (:_id)");
    query.bindValue(":_id", user_ID);
    if (query.exec()) {
        while(query.next()) {
            username = query.value(0).toString();
            return username;
        }
    }
    else {
        qDebug() << "Errors of getNamebyID: " << query.lastError();
    }
    return username;
}

QVector<QString> see_messages::get_private_mess()
{
    QVector<QString> P_mes;

    if(db.open()) {
        int ID_sender;
        int ID_receiver;
        QString mes, name_sender,name_receiver;
        QSqlQuery query;
        query.exec("SELECT id_sender, id_receiver, message FROM private_messages");
        while (query.next()) {
            ID_sender = query.value(0).toInt();
            ID_receiver  = query.value(1).toInt();
            mes = query.value(2).toString();
            name_sender = getNamebyID(ID_sender);
            name_receiver = getNamebyID(ID_receiver);
            if (ID_sender ==  ID_receiver) {
                P_mes.push_back("<" + name_sender + "> " + "while talking to yourself " + "'" + mes + "'");
            }
            else {
                P_mes.push_back("<" + name_sender + ">: " + "send signal to " + "<" + name_receiver + ">: " + "'" + mes + "'");
            }
        }
        return P_mes;
    }
    else {
        qDebug() << "We lost connection to Database in 'see_messages'";
    }
    return P_mes;
}

QVector<QString> see_messages::get_group_mess()
{
    QVector<QString> G_mes;

    if(db.open()) {
        int ID_sender;
        QString mes, name_sender;
        QSqlQuery query;
        query.exec("SELECT id_sender, message FROM group_messages");
        while (query.next()) {
            ID_sender = query.value(0).toInt();
            mes = query.value(1).toString();
            name_sender = getNamebyID(ID_sender);
            G_mes.push_back("<" + name_sender + ">: " + "'" + mes + "'");
        }
        return G_mes;
    }
    else {
        qDebug() << "We lost connection to Database in 'see_messages'";
    }
    return G_mes;
}


void see_messages::on_pushButton_clicked()
{
    QVector<QString> ALL_PRIVATE = get_private_mess();
    QVector<QString> all_group = get_group_mess();

    ui->moder_group_mes_textBrowser->clear();
    ui->moder_private_mes_textBrowser->clear();

    for (int i = 0; i < ALL_PRIVATE.size(); ++i) {
        ui->moder_private_mes_textBrowser->append(ALL_PRIVATE.at(i));
    }
    for (int i = 0; i < all_group.size(); ++i) {
        ui->moder_group_mes_textBrowser->append(all_group.at(i));
    }
}

see_messages::~see_messages()
{
    delete ui;
}



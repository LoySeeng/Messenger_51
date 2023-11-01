#ifndef SEE_MESSAGES_H
#define SEE_MESSAGES_H

#include <QWidget>
#include <QVector>
#include <QString>
#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>

namespace Ui {
class see_messages;
}

class see_messages : public QWidget
{
    Q_OBJECT

public:
    explicit see_messages(QWidget *parent = nullptr);
    ~see_messages();

private slots:
    void on_moder_exet_pushButton2_clicked();

    void on_pushButton_clicked();

private:
    QVector<QString> get_private_mess();
    QVector<QString> get_group_mess();
    QString getNamebyID(int login);

private:
    Ui::see_messages *ui;
    QSqlDatabase db;
};

#endif // SEE_MESSAGES_H

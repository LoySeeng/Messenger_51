#ifndef LOGIN_MODER_FORM_H
#define LOGIN_MODER_FORM_H

//#include "database.h"
#include "moderator_basement.h"

#include <QWidget>
#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>

namespace Ui {
class Login_moder_Form;
}

class Login_moder_Form : public QWidget
{
    Q_OBJECT

public:
    explicit Login_moder_Form(QWidget *parent = nullptr);
    ~Login_moder_Form();

private slots:
    void on_moder_login_pushButton_clicked();
    void on_moder_NOT_login_pushButton_clicked();

private:
    QString getModerPass(QString login);

private:
    Ui::Login_moder_Form *ui;
    QSqlDatabase db;
    Moderator_basement *Basement;
};

#endif // LOGIN_MODER_FORM_H

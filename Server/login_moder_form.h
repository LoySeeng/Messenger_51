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
#include <QMessageBox>

namespace Ui {
class Login_moder_Form;
}

class Login_moder_Form : public QWidget
{
    Q_OBJECT

public:
    explicit Login_moder_Form(QWidget *parent = nullptr);
    ~Login_moder_Form();

public:
    QString moder_login() const;

private slots:
    void on_moder_login_pushButton_clicked();
    void on_moder_NOT_login_pushButton_clicked();

private:
    QString getModerPass(QString login); // on succsess return moderator password otherwise empty string

private:
    Ui::Login_moder_Form *ui;
    Moderator_basement *Basement;
    QSqlDatabase db;
    QString l_mod_login;

};

#endif // LOGIN_MODER_FORM_H

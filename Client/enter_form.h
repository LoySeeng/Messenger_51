#ifndef ENTER_FORM_H
#define ENTER_FORM_H

#include "main_form.h"

#include <QWidget>
#include <QTcpSocket>
#include <QDebug>
#include <QRegularExpression>
#include <QRegularExpressionMatchIterator>
#include <QStringList>


namespace Ui {
class Enter_Form;
}

class Enter_Form : public QWidget
{
    Q_OBJECT

public:
    explicit Enter_Form(QTcpSocket *socket, QWidget *parent = nullptr);
    ~Enter_Form();
    Main_Form *Start_Chat;
    void login();
    void signUp();
    int userID;


public slots:
    void onSocketReadyRead();

private slots:
    void on_login_OK_pushButton_clicked();

private:
    Ui::Enter_Form *ui;
    QTcpSocket *socket;
    QByteArray Data;
    void SendtoServer(QString mes_out);
};

#endif // ENTER_FORM_H

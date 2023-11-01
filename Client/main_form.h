#ifndef MAIN_FORM_H
#define MAIN_FORM_H

#include <QWidget>
#include <QTcpSocket>
#include <QDebug>
#include <QRegularExpression>
#include <QRegularExpressionMatchIterator>
#include <QStringList>


namespace Ui {
class Main_Form;
}

class Main_Form : public QWidget
{
    Q_OBJECT

public:
    explicit Main_Form(QTcpSocket *socket, QWidget *parent = nullptr);
    ~Main_Form();

public slots:
    void onSocketReadyRead();

private slots:
    void on_send_all_pushButton_clicked();

    void on_send_private_pushButton_clicked();

    void on_refresh_pushButton_clicked();

private:
    Ui::Main_Form *ui;
    QTcpSocket *socket;
    QByteArray Data;
    void SendtoServer(QString mes_out);
};

#endif // MAIN_FORM_H

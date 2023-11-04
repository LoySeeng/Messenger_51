#ifndef MAIN_FORM_H
#define MAIN_FORM_H

#include <QWidget>
#include <QTcpSocket>
#include <QRegularExpression>
#include <QRegularExpressionMatchIterator>
#include <QStringList>
#include <QMessageBox>
#include <QListWidget>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QSet>


namespace Ui {
class Main_Form;
}

class Main_Form : public QWidget
{
    Q_OBJECT

public:
    explicit Main_Form(QTcpSocket *socket, int userID, QString username, QWidget *parent = nullptr);
    ~Main_Form();

public slots:
    void onSocketReadyRead();

private slots:
    void on_send_all_pushButton_clicked();
    void on_send_private_pushButton_clicked();
    void on_refresh_pushButton_clicked();

private:
    Ui::Main_Form *ui;
    QTcpSocket *m_socket;
    QByteArray Data;
    void SendtoServer(QString mes_out);
    int m_userID;
    QString m_username;
    QSet<QString> All_users;
};

#endif // MAIN_FORM_H

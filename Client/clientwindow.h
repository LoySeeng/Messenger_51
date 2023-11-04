#ifndef CLIENTWINDOW_H
#define CLIENTWINDOW_H

#include "enter_form.h"

#include <QMainWindow>
#include <QString>
#include <QDebug>
#include <QTcpSocket>
#include <QByteArray>
#include <QRegularExpression>

QT_BEGIN_NAMESPACE
namespace Ui { class ClientWindow; }
QT_END_NAMESPACE

class ClientWindow : public QMainWindow
{
    Q_OBJECT

public:
    ClientWindow(QWidget *parent = nullptr);
    ~ClientWindow();
    QTcpSocket *client_socket;
    Enter_Form *CheckID;

private slots:
    void on_Enter_pushButton_clicked();

private:
    Ui::ClientWindow *ui;

private:
    QByteArray Data;
};
#endif // CLIENTWINDOW_H

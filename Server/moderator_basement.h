#ifndef MODERATOR_BASEMENT_H
#define MODERATOR_BASEMENT_H

#include "see_all_users.h"
#include "ban_users.h"
#include "see_messages.h"

#include <QWidget>

namespace Ui {
class Moderator_basement;
}

class Moderator_basement : public QWidget
{
    Q_OBJECT

public:
    explicit Moderator_basement(QWidget *parent = nullptr);
    ~Moderator_basement();

private slots:
    void on_all_users_pushButton_clicked();
    void on_all_messages_pushButton_clicked();
    void on_banned_users_pushButton_clicked();

private:
    Ui::Moderator_basement *ui;
    ban_users *Ban;
    see_all_users *Users;
    see_messages *Messages;
};

#endif // MODERATOR_BASEMENT_H

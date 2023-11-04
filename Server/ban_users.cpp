#include "ban_users.h"
#include "ui_ban_users.h"

ban_users::ban_users(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ban_users)
{
    ui->setupUi(this);

    db = QSqlDatabase::database();

    ui->ban_pushButton->setText(tr("Ban user"));
    ui->disconnect_pushButton->setText(tr("Disconnect user"));
    ui->disconnect_pushButton->setToolTip(tr("..under construction.."));
    ui->groupBox->setTitle(tr("Shut down subjects"));
    ui->groupBox_2->setTitle(tr("Eliminated subjects"));
    ui->groupBox->setToolTip(tr("Users which were disconnected during this session"));
    ui->groupBox_2->setToolTip(tr("Users which were banned during this session"));

}

void ban_users::deleteUser(QString login) // on success return 1 otherwise -1
{
    if(db.open()) {
        qDebug() << "We still here";
        QSqlQuery query;
        query.prepare("DELETE FROM users WHERE login = (:_login)");
        query.bindValue(":_login", login);
        query.exec();
    }
    else {
        qDebug() << "We lost connection to Database in 'ban_users'";
    }
}

QVector<QString> ban_users::getAllUsers()
{
    QVector<QString> USERS;
    if(db.open()) {
        qDebug() << "Database is still running";
        QSqlQuery query;
        query.exec("SELECT login FROM users");
        while (query.next()) {
            QString name = query.value(0).toString();
            USERS.push_back(name);
        }
        return USERS;
    }
    else {
        qDebug() << "We lost connection to Database";
    }
    return USERS;
}

void ban_users::on_ban_pushButton_clicked()
{
    QVector<QString> All_users = getAllUsers();

    QDialog dial(this);
    dial.setModal(true);
    auto l = new QVBoxLayout();
    dial.setLayout(l);
    auto userListWgt = new QListWidget(&dial);
    l->addWidget(userListWgt);
    auto buttonBox = new  QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dial);
    l->addWidget(buttonBox);
    connect(buttonBox, &QDialogButtonBox::accepted, &dial, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, &dial, &QDialog::reject);
    // QDialog->setStyleSheet("font: 12px 'Bahnschrift'");
    for (const auto &value : All_users) {
        userListWgt->addItem(value);
    }
    userListWgt->setCurrentRow(0);
    auto result = dial.exec();
    if(result == QDialog::Accepted && userListWgt->currentItem()) {
        QString baned_user = userListWgt->currentItem()->text();
        deleteUser(baned_user);
    }
}

ban_users::~ban_users()
{
    delete ui;
}



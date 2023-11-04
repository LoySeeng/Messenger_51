#include "login_moder_form.h"
#include "ui_login_moder_form.h"

Login_moder_Form::Login_moder_Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Login_moder_Form)
{
    ui->setupUi(this);

    db = QSqlDatabase::database();

    ui->welcome_login->setText(tr("Trying to enter secret facility..."));
    ui->welcome_login->setAlignment(Qt::AlignCenter);
    ui->moder_login_pushButton->setText(tr("Enter"));
    ui->moder_NOT_login_pushButton->setText(tr("Turn away"));
}


QString Login_moder_Form::moder_login() const
{
    return l_mod_login;
}

// getting moderator's password from database
QString Login_moder_Form::getModerPass(QString login) // on succsess return moderator password otherwise empty string
{
    QString PS;
    if(db.open()) {
        QSqlQuery query;
        query.prepare("SELECT password FROM moderators WHERE login = (:_login)");
        query.bindValue(":_login", login);
        if(query.exec()) {
            while(query.next()) {
                QString PS = query.value(0).toString();
                return PS;
            }
        }
        else {
            qDebug() << "Errors of getModerPass: " << query.lastError();
        }
    }
    else {
        qDebug() << "We lost connection to Database in 'login_moder_form'";
    }
    return PS;
}

void Login_moder_Form::on_moder_login_pushButton_clicked()
{
    QString mod_login = ui->login_edit->text();
    QString mod_pas = ui->pas_edit->text();

    if (!mod_login.isEmpty() && !mod_pas.isEmpty()) {

        if(db.isOpen()) {
            QString get_pas = getModerPass(mod_login);

            if(get_pas == mod_pas) {
                qDebug() << "Moderator " + mod_login + " joined";
                l_mod_login = mod_login;
                Basement = new Moderator_basement(this->moder_login(), nullptr);
                Basement->setWindowTitle("Messenger 51");
                Basement->show();
                this->close();
            }
            else if (get_pas.isEmpty()) {
                QMessageBox::warning(this, "Login or password is incorrect", tr("Are you sure you're working here?"));
            }
            else {
                qDebug() << "Something is wrong with process of login moderators";
            }
        }
        else {
            qDebug() << "We lost connection to Database in 'login_moder_form'";
        }
    }
}


void Login_moder_Form::on_moder_NOT_login_pushButton_clicked()
{
    ui->login_edit->clear();
    ui->pas_edit->clear();
}

Login_moder_Form::~Login_moder_Form()
{
    delete ui;
}



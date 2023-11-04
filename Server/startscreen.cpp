#include "startscreen.h"
#include "ui_startscreen.h"

StartScreen::StartScreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StartScreen)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);

    connect(ui->server_page, &ServerWindow::loginRequested, this, &StartScreen::setLoginForm);
    connect(ui->login_page, &Login_moder_Form::basementRequested, this, &StartScreen::setBasement);
    connect(ui->login_page, &Login_moder_Form::startRequested, this, &StartScreen::setStart);
}

StartScreen::~StartScreen()
{
    delete ui;
}

void StartScreen::setStart()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void StartScreen::setLoginForm()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void StartScreen::setBasement()
{
    ui->stackedWidget->setCurrentIndex(2);
}

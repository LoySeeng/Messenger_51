#ifndef STARTSCREEN_H
#define STARTSCREEN_H

#include <QWidget>

namespace Ui {
class StartScreen;
}

class StartScreen : public QWidget
{
    Q_OBJECT

public:
    explicit StartScreen(QWidget *parent = nullptr);
    ~StartScreen();

public:
    void setStart();
    void setLoginForm();
    void setBasement();

private:
    Ui::StartScreen *ui;
};

#endif // STARTSCREEN_H

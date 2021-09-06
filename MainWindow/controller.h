#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include"Login/login.h"
#include"Login/usercenter.h"
#include"MainWindow/mainwindow.h"

class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(QObject *parent = nullptr);

public slots:
    void onLoginSucceed();
    void onLogOut();
private:
    Login *login;
    UserCenter* userCenter;
    MainWindow* mainWindow;
};

#endif // CONTROLLER_H

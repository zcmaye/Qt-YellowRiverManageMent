#include "controller.h"
#include"CenterModule/Configer.h"


Controller::Controller(QObject *parent)
    : QObject(parent),
      login(new Login),
      userCenter(new UserCenter),
      mainWindow(nullptr)
{
    login->show();
    //登录成功
    connect(login,&Login::loginSucceed,this,&Controller::onLoginSucceed);
}

void Controller::onLoginSucceed()
{
    login->hide();
    if(mainWindow == nullptr)
    {
        mainWindow = new MainWindow;
        //注销
        connect(mainWindow,&MainWindow::logout,this,&Controller::onLogOut);
        //用户中心
        connect(mainWindow,&MainWindow::userCenter,userCenter,&UserCenter::exec);
    }


    cout<<"onlog";
    mainWindow->updateUserName();
    mainWindow->show();
}

void Controller::onLogOut()
{
    mainWindow->deleteLater();
    mainWindow = nullptr;

    login->show();
}

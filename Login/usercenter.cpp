#include "usercenter.h"
#include<QLineEdit>
#include<QLabel>
#include<QPushButton>
#include<QLayout>
#include<QSqlQuery>
#include<CenterModule/Configer.h>
#include"Database/databasecontroller.h"

UserCenter::UserCenter(QWidget *parent) : QDialog(parent)
{
    setFixedSize(400,300);
    setWindowTitle("修改密码");
    oldPasswdEdit = new QLineEdit;
    newPasswdEdit = new QLineEdit;
    newPasswdAgainEdit = new QLineEdit;
    modifyBtn = new QPushButton("确认修改");

    oldPasswdEdit->setEchoMode(QLineEdit::EchoMode::Password);
    newPasswdEdit->setEchoMode(QLineEdit::EchoMode::Password);
    newPasswdAgainEdit->setEchoMode(QLineEdit::EchoMode::Password);

    QGridLayout* mainLayout = new QGridLayout;
    mainLayout->addWidget(new QLabel("旧密码"),0,0);
    mainLayout->addWidget(oldPasswdEdit,0,1);
    mainLayout->addWidget(new QLabel("新密码"),1,0);
    mainLayout->addWidget(newPasswdEdit,1,1);
    mainLayout->addWidget(new QLabel("确认新密码"),2,0);
    mainLayout->addWidget(newPasswdAgainEdit,2,1);
    mainLayout->addWidget(modifyBtn,3,1);
    modifyBtn->setFixedWidth(150);

    setStyleSheet("QPushButton{border:none;background-color:rgb(5,186,251);height:30px;"
                  "border-radius:5px;}"
                  "QPushButton:hover{background-color:rgb(31,199,253);}"
                  "QLineEdit{border:1px solid gray;border-radius:5px;width:200px;height:30px;"
                  "font-size:20px;}");

    QHBoxLayout *hlayout = new QHBoxLayout;
    hlayout->addStretch();
    hlayout->addLayout(mainLayout);
    hlayout->addStretch();

    QVBoxLayout*vlayout = new QVBoxLayout;
    vlayout->addStretch();
    vlayout->addLayout(hlayout);
    vlayout->addStretch();

    setLayout(vlayout);
    connectSignalsAndSlots();
}

void UserCenter::connectSignalsAndSlots()
{
    connect(modifyBtn,&QPushButton::clicked,this,[=]()
    {
        cout<<"clicked";
        QString oldPasswd = MD5(oldPasswdEdit->text());
        QString newPasswd = MD5(newPasswdEdit->text());
        QString newPasswdAgian = MD5(newPasswdAgainEdit->text());

        QSqlQuery q;
        //用户原来的密码(md5)
        QString srcPasswd =  DataBaseController::SingleIstance()->userInfo().password;
        if(oldPasswd != srcPasswd)
        {
            //老密码错误
            cout<<"旧密码输入错误";
            QMessageBox::warning(this,"警告","旧密码输入错误");
            return;
        }
        if(newPasswd != newPasswdAgian)
        {
            //两次新密码输入不一致
            cout<<"两次新密码输入不一致";
            QMessageBox::warning(this,"警告","两次新密码输入不一致");
            return;
        }

        bool ok = q.exec(QString("UPDATE user SET password='%1' where username='%2'").arg(newPasswd).arg(DataBaseController::SingleIstance()->userInfo().username));
        if(ok)
        {
            //修改成功
            cout<<"修改成功"<<endl;
            this->hide();
            QMessageBox::warning(this,"修改成功","修改成功，请记住您的密码哦~");
        }
    });
}

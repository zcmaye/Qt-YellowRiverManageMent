#include "login.h"
#include "ui_login.h"
#include<QSettings>
#include "Database/databasecontroller.h"
#include "MainWindow/mainwindow.h"
#include"CenterModule/Configer.h"
#include<QMouseEvent>
#include<QMessageBox>

Login::Login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    //去掉窗口边框
    setWindowFlags(Qt::FramelessWindowHint);
    setWindowTitle("用户登录");
    database = DataBaseController::SingleIstance();

    loadUsers();
    initUi();
}

Login::~Login()
{
    delete ui;
}

void Login::mousePressEvent(QMouseEvent *event)
{
    isPress = true;
    begPos = event->pos();
}

void Login::mouseMoveEvent(QMouseEvent *event)
{
    if(isPress)
    {
        move(mapToGlobal(event->pos())-begPos);
    }
}

void Login::mouseReleaseEvent(QMouseEvent *)
{
    isPress = false;
}

void Login::loadUsers()
{
    //从数据库读取用户使用过的账号和密码
    users = database->readUserAndPasswd();
    for(auto&user :users)
    {
        ui->userEdit->addItem(user.first);
        ui->userEdit->setEditText(user.first);
        ui->passwdEdit->setText(user.second);
    }
    //选择不同的账号时，密码也要同时更新
    connect(ui->userEdit,QOverload<int>::of(&QComboBox::currentIndexChanged),this,[=](int index)
    {
        if(index<users.size())  //防止users越界访问
            ui->passwdEdit->setText(users[index].second);
    });
}

void Login::initUi()
{
    ui->userEdit->lineEdit()->setPlaceholderText("账号");
    ui->passwdEdit->setPlaceholderText("密码");
    ui->passwdEdit->setEchoMode(QLineEdit::EchoMode::Password);

    //输入完账号和密码，按回车发射登录按钮的pressed信号
    connect(ui->userEdit->lineEdit(),&QLineEdit::returnPressed,ui->loginBtn,&QPushButton::pressed);
    connect(ui->passwdEdit,&QLineEdit::returnPressed,ui->loginBtn,&QPushButton::pressed);

    //编辑账号时，把密码清空
    connect(ui->userEdit->lineEdit(),&QLineEdit::textEdited,ui->passwdEdit,&QLineEdit::clear);
    //编辑账号时，去users里面查找是否存在，找到了同时把密码给补上
    connect(ui->userEdit->lineEdit(),&QLineEdit::textEdited,this,[=]()
    {
        for(auto&str:users)
        {
            if(str.first == ui->userEdit->currentText())
            {
                ui->passwdEdit->setText(str.first);
            }
        }
    });
}

void Login::on_loginBtn_pressed()
{
    QString userName = ui->userEdit->currentText();
    QString passWord = ui->passwdEdit->text();
    if(database->VerifyLogin(userName,MD5(passWord)))
    {
        emit loginSucceed(userName);

        database->saveUserAndPasswd(userName,passWord);
    }else
    {
        QMessageBox::warning(this,"警告","用户名或密码错误QAQ");
    }
}

void Login::on_minBtn_clicked()
{
    showMinimized();
}

void Login::on_closeBtn_clicked()
{
    qApp->quit();
}

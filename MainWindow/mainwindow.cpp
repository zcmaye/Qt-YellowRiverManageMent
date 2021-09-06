#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QStandardItem>
#include<QFile>
#include"CenterModule/Configer.h"
#include<QPoint>
#include"Database/databasecontroller.h"
#include"Module/controlwindow.h"
#include"Login/usercenter.h"
#include<QSystemTrayIcon>
#include<QMenu>

 static Qt::WindowFlags flag;   //保存窗口初始得标志
MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initListView();
    controlWindow = new ControlWindow;
    ui->center_widget->setLayout(new QGridLayout);
    ui->center_widget->layout()->addWidget(controlWindow);
    ui->center_widget->layout()->setMargin(0);
    ui->center_widget->layout()->setSpacing(0);

    initSystemTray();
    flag = this->windowFlags();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *)
{
    //qDebug()<<ui->center_widget->size()<<ui->center_widget->sizeHint();
}

void MainWindow::changeEvent(QEvent *ev)
{

    if(ev->type() == QEvent::WindowStateChange)
    {
        switch (this->windowState())
        {
        case Qt::WindowMinimized:
            cout<<"windowstate"<<this->windowState()<<" "<<this->windowFlags();
           // this->setWindowFlags(flag | Qt::Tool);  //加Qt::Tool 为了隐藏任务栏图标
            break;
        case Qt::WindowMaximized:

            break;
        case Qt::WindowActive:
            cout<<"WindowActive";
            break;
        }
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    systemTray->hide();
}

void MainWindow::initListView()
{
    //ui->left_treeWidget->setHeaderItem(new QTreeWidgetItem({"选择操作"}));
    ui->left_treeWidget->setHeaderHidden(true);

    ui->left_treeWidget->addTopLevelItem(createItmes("助教",{"流量录入","流量查询"}));
    ui->left_treeWidget->addTopLevelItem(createItmes("转化",{"业绩录入","业绩查询"}));
    ui->left_treeWidget->addTopLevelItem(createItmes("讲师",{"听课数据录入","听课数据查询"}));
    ui->left_treeWidget->addTopLevelItem(createItmes("其他",{"other1","other2"}));

    QFile read("://left_treeWidget.qss");
    if(read.open(QIODevice::ReadOnly))
    {
        ui->left_treeWidget->setStyleSheet(read.readAll());
    }
    read.close();
    connect(ui->left_treeWidget,&QTreeView::clicked,this,&MainWindow::setCenter_widget);

}

QTreeWidgetItem* MainWindow::createItmes(const QString&rootStr,const QStringList&childStr)
{
    QTreeWidgetItem *item = new QTreeWidgetItem({rootStr});
    for(auto&tstr:childStr)
    {
        item->addChild(new QTreeWidgetItem({tstr}));
    }
    return item;
}

void MainWindow::updateUserName()
{
    QString userName = DataBaseController::SingleIstance()->userInfo().username;

    ui->user_info->setText(QString(userName+"%1").arg(",你好"));
    setWindowTitle(QString("顿开教育黄河管理系统：当前用户<%1>").arg(userName));
}

void MainWindow::initSystemTray()
{
    systemTray = new QSystemTrayIcon(this);
    systemTray->setIcon(QIcon("://images/DunKai.png"));
    systemTray->setToolTip("顿开教育黄河管理系统");

    QMenu *menu = new QMenu("aa");
    QAction*openMainWnd =  menu->addAction("打开主面板");
    menu->addSeparator();
    QAction* quit = menu->addAction("退出");

    systemTray->setContextMenu(menu);
    systemTray->show();

    connect(systemTray,&QSystemTrayIcon::activated,this,[this](QSystemTrayIcon::ActivationReason reason)
    {
        if(reason == QSystemTrayIcon::DoubleClick)
        {
            this->showNormal();
        }
    });
    connect(openMainWnd,&QAction::triggered,this,&MainWindow::showNormal);
    connect(quit,&QAction::triggered,qApp,&QApplication::quit);
}

void MainWindow::setCenter_widget(const QModelIndex& index)
{
    //助教
    if(index.parent().row() == 0)
    {
        if(index.row() == 0)        //录入
        {
            controlWindow->setCurrentIndex(0);
        }
        else if(index.row() == 1)   //查询
        {
            controlWindow->setCurrentIndex(1);
            if(DataBaseController::SingleIstance()->userInfo().token ==1)
            {
                controlWindow->setAdministratorPrivileges(true);
            }else
            {
                controlWindow->setAdministratorPrivileges(false);
            }
        }
    }
    //讲师
    if(index.parent().row() == 2)
    {
        if(index.row() == 0)        //录入
        {
            controlWindow->setCurrentIndex(2);
        }
        else if(index.row() == 1)   //查询
        {
            controlWindow->setCurrentIndex(3);
        }
    }
}

void MainWindow::on_logout_btn_clicked()
{
    emit logout();
}

void MainWindow::on_user_info_clicked()
{
    emit userCenter();
}

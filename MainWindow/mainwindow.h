#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

class QStandardItemModel;
class QStandardItem;
class QTreeWidgetItem;
class QSystemTrayIcon;

class Login;
class IDentityInfo;
class ControlWindow;
class UserCenter;

namespace Ui {
class MainWindow;
}

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void resizeEvent(QResizeEvent *event) override;
    void changeEvent(QEvent * ev) override;
    void closeEvent(QCloseEvent *event) override;

    void initListView();
    QTreeWidgetItem* createItmes(const QString&rootStr,const QStringList&childStr);

    //设置用户名
    void updateUserName();

    //设置系统托盘
    void initSystemTray();

signals:
    void logout();      //退出登录
    void userCenter();  //用户中心

private slots:
    void on_logout_btn_clicked();
    void setCenter_widget(const QModelIndex& index);

    void on_user_info_clicked();

private:
    Ui::MainWindow *ui;

    ControlWindow *controlWindow;
    QSystemTrayIcon* systemTray;
};

#endif // MAINWINDOW_H

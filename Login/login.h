#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
class DataBaseController;
class MainWindow;


namespace Ui {
class Login;
}

class Login : public QWidget
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    //加载本地用户信息
    void loadUsers();
    //初始化ui
    void initUi();
signals:
    void loginSucceed(const QString& userName);
private slots:
    //点击登录按钮
    void on_loginBtn_pressed();
    //最小化
    void on_minBtn_clicked();
    //关闭
    void on_closeBtn_clicked();

private:
    Ui::Login *ui;
    DataBaseController * database;

    //拖拽
    bool isPress;
    QPoint begPos;
    //从数据库读取用户使用过的账号和密码
    QList<QPair<QString,QString>> users;
};

#endif // LOGIN_H

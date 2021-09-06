#ifndef DATABASECONTROLLER_H
#define DATABASECONTROLLER_H

#include <QObject>
#include<QSqlDatabase>
#include"Database/readjsonconfigure.h"

class QSqlQuery;
//用户表数据
class TableUser
{
public:
    TableUser();
    //从数据库获取用户信息
    TableUser& parseUserInfo(const QSqlQuery& q);
    friend QDebug operator<<(QDebug out,TableUser& user);
public:
    quint8 id;          //身份编号
    QString username;   //用户名
    QString nickname;   //昵称()
    QString password;   //密码
    quint8 token;       //身份权限
};

class DataBaseController : public QObject
{
    Q_OBJECT
public:
    //获取单例
    static DataBaseController* SingleIstance();
public:
    //验证用户登录(需要绑定远程数据库)
    bool VerifyLogin(const QString&userName,const QString& password);
    //获取当前登录的用户信息
    TableUser userInfo();

    //保存和读取本地账号和密码(明文)
    QList<QPair<QString,QString>> readUserAndPasswd();
    void saveUserAndPasswd(const QString&userName,const QString& passwd);
protected:
    explicit DataBaseController(QObject *parent = nullptr);
    void createTable();
private:
    QStringList list_createTableSql;
    TableUser user;
};




#endif // DATABASECONTROLLER_H

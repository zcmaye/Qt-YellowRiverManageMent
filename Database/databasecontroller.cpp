#include "databasecontroller.h"
#include<QSqlQuery>
#include"CenterModule/Configer.h"
#include<QMessageBox>
#include<QApplication>
#include<QSqlError>


DataBaseController *DataBaseController::SingleIstance()
{
    static DataBaseController* instance =nullptr;
    if(!instance)
    {
        instance = new DataBaseController;
    }
    return instance;
}

DataBaseController::DataBaseController(QObject *parent) : QObject(parent)
{
    list_createTableSql<<"create table freecustomers(id int,qq varchar,tel varchar,weixin varchar,userid int,"
                         "assistant varchar,entrytime datetime,source varchar,images varchar,creater varchar,createtime datetime)"

                      <<"create table user(id int,username varchar,nickname varchar,password varchar,token int)"

                      <<"create table 'desc lectures'(id int,nickname varchar,qq varchar,weixin varchar,"
                        "lectures_date   date,course varchar,lecture_length varchar,lecture_interval varchar)"

                     <<"create table course(id int,coursename varchar)";

    SqlConfigure fig = ReadJsonConfigure::create()->getSqlConfigure();
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(fig.HostName);
    db.setPort(fig.Port);
    db.setUserName(fig.UserName);
    db.setPassword(fig.PassWord);
    db.setDatabaseName(fig.DatabaseName);

    if(!db.open())
    {
        cout<<"DunKai 数据库打开失败";
        QMessageBox::warning(nullptr,"错误",QString("数据库连接失败,请检查网络 %1").arg(db.lastError().text()));
        QApplication::quit();
        return;
    }
}

//登录成功会通过sigUserinfo信号把用户的数据发出去
bool DataBaseController::VerifyLogin(const QString &userName, const QString &password)
{
    if(userName.isEmpty() || password.isEmpty())
    {
        return false;
    }

    QSqlQuery q;
    q.exec(QString("select * from user where username='%1' and password='%2'").arg(userName).arg(password));
    if(!q.next())
    {
        cout<<"登录失败~";
        return false;
    }
    user = TableUser().parseUserInfo(q);
    return true;
}

TableUser DataBaseController::userInfo()
{
    return user;
}
/*@createTable测试代码，请勿随意调用*/
void DataBaseController::createTable()
{
    QSqlQuery q;
    if(!q.exec(list_createTableSql[0]))
        cout<<"freecustomers table create failed";
    if(!q.exec(list_createTableSql[1]))
        cout<<"user table create failed";
    if(!q.exec(list_createTableSql[2]))
        cout<<"desc lectures table create failed";
    if(!q.exec(list_createTableSql[3]))
        cout<<"course table create failed";

    bool isok = false;
    isok = q.exec("insert into freecustomers values(1,'825943234','17363691460','zhangchi',1,'顽老板','2021/6/18','MAYE','null','玩蛇','2021/6/30')");
    if(!isok)
        cout<<"freecustomers 表插入数据失败~";
    isok = q.exec("insert into 'desc lectures' values(1,'槑','5698542','wecat','2020/05/20','C语言入门到放弃',"
           "'120分钟','啥？')");
    if(!isok)
        cout<<"desc lectures 表插入数据失败~";

    isok &= q.exec("insert into course values(1000,'C语言入门到放弃')");
    isok &= q.exec("insert into course values(1001,'Qt入门到精通')");
    isok &= q.exec("insert into user values(0,'里奇','里奇啊','12345678',1)");
    isok &= q.exec("insert into user values(1,'maye','maye','asd12345678',1)");
    isok &= q.exec("insert into user values(2,'莫影','moying','12345678',1)");
    isok &= q.exec("insert into user values(3,'英杰','英杰','12345678',0)");
    isok &= q.exec("insert into user values(4,'夏沫','满心欢喜','12345678',0)");
    isok &= q.exec("insert into user values(5,'苏苏','你是谁','12345678',0)");
    isok &= q.exec("insert into user values(6,'三水','嘎嘎','12345678',0)");
    if(!isok)
        cout<<"user 表插入数据失败~";
}
QList<QPair<QString, QString>> DataBaseController::readUserAndPasswd()
{
    //添加数据库驱动
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","qt_sql_read");
    //设置数据库名称
    db.setDatabaseName("user");
    //打开数据库
    if(!db.open())
    {
        cout<<"数据库打开失败";
        return QList<QPair<QString, QString>>();
    }

    //执行sql语句
    QSqlQuery query(db);
    //查询是否存在users表
    if(!query.exec("select * from users"))
    {
        cout<<"users表不存在，没有用户信息";
        return QList<QPair<QString, QString>>();
    }
    //查询users表中的所有数据
    if(!query.exec("select * from users"))
    {
         return QList<QPair<QString, QString>>();
    }

    QList<QPair<QString, QString>> res;
    //query查询到了数据，循环获取
    while(query.next())
    {
        QString userName = query.value("userName").toString();
        QString passWord = query.value("passWord").toString();
        cout<<userName<<" "<<passWord;
        res.push_back(QPair<QString,QString>(userName,passWord));
    }
    return res;
}

void DataBaseController::saveUserAndPasswd(const QString &userName, const QString &passwd)
{
    //添加数据库驱动
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","qt_sql_save");
    //设置数据库名称
    db.setDatabaseName("user");
    //打开数据库
    if(!db.open())
    {
        cout<<"数据库打开失败";
        return;
    }

    //执行sql语句
    QSqlQuery query(db);
    //查询是否存在users表,不存在则创建
    if(!query.exec("select * from users"))
    {
        cout<<"users 不表存在，正在创建...";
        if(!query.exec("create table users(userName varchar,passWord varchar)"))
        {
            cout<<"users表创建失败...";
        }
    }
    //查看用户是否存在
    if(query.exec(QString("select * from users where userName = '%1'").arg(userName))
            && query.next())
    {
        cout<<userName<<" 存在，不会继续保存";

        return;
    }

    //插入数据
    if(query.exec(QString("insert into users values('%1','%2')").arg(userName).arg(passwd)))
    {
        cout<<"保存用户信息成功:"<<userName<<" "<<passwd;
    }
}

/*@user结构*/
TableUser::TableUser()
{

}

TableUser& TableUser::parseUserInfo(const QSqlQuery &q)
{
    this->id = q.value(0).toUInt();
    this->username = q.value(1).toString();
    this->nickname = q.value(2).toString();
    this->password = q.value(3).toString();
    this->token = q.value(4).toUInt();
    return *this;
}

QDebug operator<<(QDebug out,TableUser& user)
{
    out<<user.id<<" "<<user.username<<" "<<user.nickname<<" "<<user.password<<" "<<user.token;
    return out;
}

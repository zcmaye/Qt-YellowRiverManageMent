#include "mythreadsearch.h"
#include<QSqlRecord>
#include<QStandardItemModel>
#include"CenterModule/Configer.h"
#include<QString>
#include"Database/databasecontroller.h"


MythreadSearch::MythreadSearch(QObject *parent):QObject(parent)
{
    m_model = new QStandardItemModel;
    initFieldContrast();
}

void MythreadSearch::initFieldContrast()
{
    fieldAliasComparisonTable.insert("id","编号");
    fieldAliasComparisonTable.insert("username","用户名");
    fieldAliasComparisonTable.insert("nickname","昵称");
    fieldAliasComparisonTable.insert("password","密码");
    fieldAliasComparisonTable.insert("token","权限标记");

    fieldAliasComparisonTable.insert("id","编号");
    fieldAliasComparisonTable.insert("qq","QQ");
    fieldAliasComparisonTable.insert("tel","电话");
    fieldAliasComparisonTable.insert("weixin","微信");
    fieldAliasComparisonTable.insert("userid","用户ID");
    fieldAliasComparisonTable.insert("assistant","助教");
    fieldAliasComparisonTable.insert("entrytime","进群时间");
    fieldAliasComparisonTable.insert("source","来源");
    fieldAliasComparisonTable.insert("images","录入截图");
    fieldAliasComparisonTable.insert("creater","创建人");
    fieldAliasComparisonTable.insert("createtime","创建时间");

    fieldAliasComparisonTable.insert("id","编号");
    fieldAliasComparisonTable.insert("nickname","昵称");
    fieldAliasComparisonTable.insert("qq","QQ");
    fieldAliasComparisonTable.insert("weixin","微信");
    fieldAliasComparisonTable.insert("lectures_date","听课日期");
    fieldAliasComparisonTable.insert("course","课程名称");
    fieldAliasComparisonTable.insert("lecture_length","听课时长");
    fieldAliasComparisonTable.insert("lecture_interval","听课间隔");

    fieldAliasComparisonTable.insert("id","编号");
    fieldAliasComparisonTable.insert("coursename","课程名称");
}

void MythreadSearch::connectDataBase()
{
    SqlConfigure fig = ReadJsonConfigure::create()->getSqlConfigure();

    static int cnt = 0;
    dataBaseConName = QString("thread_sql_%1").arg(cnt++);
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL",dataBaseConName);
    db.setHostName(fig.HostName);
    db.setPort(fig.Port);
    db.setUserName(fig.UserName);
    db.setPassword(fig.PassWord);
    db.setDatabaseName(fig.DatabaseName);
    if(!db.open())
    {
        qDebug()<<"数据库打开失败";
    }
}

void MythreadSearch::createModel()
{
    connectDataBase();
    QSqlQuery query(QSqlDatabase::database(dataBaseConName));
   if(!query.exec(sql))
   {
       qDebug()<<"query 执行失败";
       searchFinished(m_model);
       return;
   }

   QStringList header;
   //获取表的所有字段
   QSqlRecord record(query.record());
   for(int i =0;i<record.count();i++)
   {
       header << fieldAliasComparisonTable.value(record.fieldName(i));
       //header << record.fieldName(i);
   }
   m_model->clear();
   m_model->setHorizontalHeaderLabels(header);
   //获取所有数据
    while(query.next())
    {
        QList<QStandardItem*> items;
        for(int i = 0;i<record.count();i++)
        {
           items << new QStandardItem(query.value(i).toString());
        }
        m_model->appendRow(items);
    }
    searchFinished(m_model);
    QSqlDatabase::removeDatabase(dataBaseConName);
}

void MythreadSearch::setSql(const QString& sql)
{
    this->sql = sql;
}


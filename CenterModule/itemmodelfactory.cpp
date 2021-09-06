#include "itemmodelfactory.h"
#include<QSqlQuery>
#include<QSqlRecord>
#include"CenterModule/Configer.h"

ItemModelFactory::ItemModelFactory()
{
    s_model = new QStandardItemModel;
    initFieldContrast();
}

ItemModelFactory *ItemModelFactory::create()
{
    static ItemModelFactory * singletonInstance = nullptr;
    if(singletonInstance == nullptr)
    {
        singletonInstance = new ItemModelFactory;
    }
    return singletonInstance;
}

void ItemModelFactory::initFieldContrast()
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
   fieldAliasComparisonTable.insert("images","图片");
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

QStandardItemModel *ItemModelFactory::createTableModel(QSqlQuery q)
{
    QStringList header;
   //获取表的所有字段
    QSqlRecord record(q.record());
    for(int i =0;i<record.count();i++)
    {
        header << fieldAliasComparisonTable.value(record.fieldName(i));
    }
   s_model->clear();
   s_model->setHorizontalHeaderLabels(header);
   //获取所有数据
    while(q.next())
    {
        QList<QStandardItem*> items;
        for(int i = 0;i<record.count();i++)
        {
           items << new QStandardItem(q.value(i).toString());
        }
        s_model->appendRow(items);
    }
    return s_model;
}

#ifndef ITEMMODELFACTORY_H
#define ITEMMODELFACTORY_H
#include<QString>
#include<QMap>
#include<QStandardItemModel>
#include<QSqlDatabase>


class ItemModelFactory:public QObject
{
    Q_OBJECT
public:
    //创建单例
   static ItemModelFactory*  create();

    //初始化数据库字段对照表
   void initFieldContrast();
   //根据表名创建model
   QStandardItemModel *createTableModel(const QString &tableName);
   //根据表名创建model
   QStandardItemModel *createTableModel(QSqlQuery q);
protected:
    ItemModelFactory();
signals:
    void searchFinished(QStandardItemModel* model);
private:
    QSqlDatabase db;
    QStandardItemModel* s_model = nullptr;
    QMap<QString,QString> fieldAliasComparisonTable;   //字段和别名对照表
};

#endif // ITEMMODELFACTORY_H

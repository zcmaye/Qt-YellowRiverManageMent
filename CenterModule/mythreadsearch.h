#ifndef MYTHREADSEARCH_H
#define MYTHREADSEARCH_H

#include <QObject>
#include <QThread>
#include <QStandardItemModel>
#include <QSqlQuery>

class MythreadSearch : public QObject
{
    Q_OBJECT
public:
    MythreadSearch(QObject*parent = nullptr);
    void connectDataBase();
    void createModel();
    void setSql(const QString& sql);

    void initFieldContrast();
signals:
    void searchFinished(QStandardItemModel* model);
private:
    QStandardItemModel* m_model;

    QString sql;
    QString dataBaseConName;

    QMap<QString,QString> fieldAliasComparisonTable;   //字段和别名对照表
};

#endif // MYTHREADSEARCH_H

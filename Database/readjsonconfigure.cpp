#include "readjsonconfigure.h"

#include<QFile>

#include<QMessageBox>
#include<QDebug>

ReadJsonConfigure::ReadJsonConfigure(QObject *parent) : QObject(parent)
{
    ParseSqlConfigure();
}

ReadJsonConfigure *ReadJsonConfigure::create()
{
    static ReadJsonConfigure *instance =nullptr;
    if(!instance)
    {
        instance = new ReadJsonConfigure;
    }
    return instance;
}

QByteArray ReadJsonConfigure::jsonFromFile(const QString &fileName)
{
    QFile read("://sqlConfigure.json");
    if(!read.open(QIODevice::Text | QIODevice::ReadOnly))
    {
        qDebug()<<"配置文件读取失败";
        return QByteArray();
    }
    return read.readAll();
}

SqlConfigure ReadJsonConfigure::getSqlConfigure()
{
     if(sqlCon.HostName.isEmpty())
         qDebug()<<"请先调用ParseSqlConfigure 解析配置文件";
     return sqlCon;
}

void ReadJsonConfigure::ParseSqlConfigure()
{
    jsonData = jsonFromFile("://sqlConfigure.json");
    jsonDoc = QJsonDocument::fromJson(jsonData,&jsonError);
    if(jsonError.error != QJsonParseError::NoError)
    {
           qDebug()<<"json 解析失败:"<<jsonError.errorString();
           return;
    }

    if(jsonDoc.isObject())
    {
       QJsonObject obj = jsonDoc.object();
       QJsonValue val =  obj.value("sqlConfigure");
       QJsonObject sqlObj = val.toObject();

       sqlCon.HostName = sqlObj.value("HostName").toString();
       sqlCon.Port = sqlObj.value("Port").toInt();
       sqlCon.UserName = sqlObj.value("UserName").toString();
       sqlCon.PassWord = sqlObj.value("PassWord").toString();
       sqlCon.DatabaseName = sqlObj.value("DatabaseName").toString();
    }
}

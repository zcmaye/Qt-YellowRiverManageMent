#ifndef READJSONCONFIGURE_H
#define READJSONCONFIGURE_H

#include <QObject>
#include<QJsonDocument>
#include<QJsonValue>
#include<QJsonParseError>
#include<QJsonObject>

struct SqlConfigure
{
    SqlConfigure():Port(0){}

    QString HostName;
    qint16  Port;
    QString UserName;
    QString PassWord;
    QString DatabaseName;
};

class ReadJsonConfigure : public QObject
{
    Q_OBJECT
public:
    static ReadJsonConfigure* create();
    QByteArray jsonFromFile(const QString& fileName);
public:
    SqlConfigure getSqlConfigure();
protected:
    explicit ReadJsonConfigure(QObject *parent = nullptr);
    void ParseSqlConfigure();
signals:
private:
    QJsonParseError jsonError;
    QJsonDocument   jsonDoc;
    QByteArray jsonData;

    SqlConfigure sqlCon;
};

#endif // READJSONCONFIGURE_H

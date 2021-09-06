#ifndef CONFIGER_H
#define CONFIGER_H
#include<QDebug>
#include<QMessageBox>
#include<QCryptographicHash>
#define cout qDebug()<<"[line"<<__LINE__<<"] "

//MD5加密
#define MD5(String) QString(QCryptographicHash::hash(String.toUtf8(),QCryptographicHash::Md5).toHex())


#endif // CONFIGER_H

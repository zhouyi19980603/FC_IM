#include "database.h"
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlQuery>

bool User::createConnect()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("user_message");
    db.setUserName("root");
    db.setPassword("root");

    if(!db.open())
    {
        qDebug()<<"failed"<<endl;
        return false;
    }else {
        qDebug()<<"succed"<<endl;
        return true;
    }
    db.close();
}

//QString User::showTitle()
//{
//    return "hhh";
//}

User::~User()
{

}

User::User()
{
    m_userEmail="@123";
    m_password="123";
}

//查询
bool User::queryMessage(const QString &userEmail, const QString &password)
{
    if(createConnect())
    {
        QString queryValue = "select *from user where email='"+userEmail+"' and password='"+password+"'";
        QSqlQuery query;
        query.exec(queryValue);
        if(query.size() == 0)
            return false;
        else {
            return true;
        }
    }
}

//插入
bool User::insertMessage(const QString &userEmail, const QString &password)
{
    if(createConnect())
    {
        QString queryInsert = "insert into user(email,password) values('"+userEmail+"','"+password+"')";
        QSqlQuery query;
        if(query.exec(queryInsert))
            return true;
        else
            return false;
    }
}

//bool User::queryMessage()
//{
//    if(createConnect())
//    {
//        QString queryValue = "select *from user where email='"+m_userEmail+"' and password='"+m_password+"'";
//        QSqlQuery query;
//        query.exec(queryValue);
//        if(query.size() == 0)
//            return false;
//        else {
//            return true;
//        }
//    }
//}

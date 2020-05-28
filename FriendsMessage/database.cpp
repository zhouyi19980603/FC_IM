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

//void User::test(const std::string &user, const std::string &pass)
//{
//    std::cout<<user<<"  "<<pass<<std::endl;
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


#include "fc_database.h"


#include <QSqlDatabase>
#include <QDebug>
#include <QSqlQuery>

//bool User::createConnect()
//{
//    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
//    db.setHostName("localhost");
//    db.setDatabaseName("user_message");
//    db.setUserName("root");
//    db.setPassword("root");

//    if(!db.open())
//    {
//        qDebug()<<"failed"<<endl;
//        return false;
//    }else {
//        qDebug()<<"succed"<<endl;
//        return true;
//    }
//    db.close();
//}

//User::~User()
//{

//}

//User::User()
//{
//    m_userEmail="@123";
//    m_password="123";
//}

////查询
//bool User::queryMessage(const QString &userEmail, const QString &password)
//{
//    if(createConnect())
//    {
//        QString queryValue = "select *from user where email='"+userEmail+"' and password='"+password+"'";
//        QSqlQuery query;
//        query.exec(queryValue);
//        if(query.size() == 0)
//            return false;
//        else {
//            return true;
//        }
//    }
//}

////插入
//bool User::insertMessage(const QString &userEmail, const QString &password)
//{
//    if(createConnect())
//    {
//        QString queryInsert = "insert into user(email,password) values('"+userEmail+"','"+password+"')";
//        QSqlQuery query;
//        if(query.exec(queryInsert))
//            return true;
//        else
//            return false;
//    }
//}


FC_Database::FC_Database(FC_Server *server)
    :_server(server)
{

}

unordered_map<string, unordered_map<string, vector<string> > > FC_Database::getFriendList()
{
    unordered_map<string, unordered_map<string, vector<string> > > _groupList;
    _groupList["@12345"]["friends"].push_back("@13456");
    _groupList["@12345"]["family"].push_back("@56789");
    _groupList["@24567"]["friends"].push_back("@13456");
    _groupList["@24567"]["friends"].push_back("@23456");
    return _groupList;
//    if(createConnect())
//        qDebug()<<"ok"<<endl;
}

//数据库中初始化好友列表信息
//std::unordered_map<std::string, std::vector<std::string> > FC_Database::getFriendList()
//{
//    std::unordered_map<std::string, std::vector<std::string> > _friendslist;
//    _friendslist["@12345"].push_back("@13456");
//    _friendslist["@12345"].push_back("@56789");
//    _friendslist["@24567"].push_back("@13456");

//    return _friendslist;
////    createConnect();
//}

bool FC_Database::createConnect()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("FC_IM");
    db.setUserName("root");
    db.setPassword("passworld");

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

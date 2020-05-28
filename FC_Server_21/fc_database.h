#ifndef FC_DATABASE_H
#define FC_DATABASE_H

#include <QObject>
#include <QString>
#include <string>
#include <iostream>
#include <unordered_map>
#include <vector>

class FC_Server;
//class User : public QObject
//{
//  Q_OBJECT
//public:
//    User();
//    Q_INVOKABLE bool queryMessage(const QString& userEmail,const QString& password);
//    Q_INVOKABLE bool insertMessage(const QString& userEmail,const QString& password);
//    bool createConnect();
//    ~User();

//private:
//    QString m_userEmail;
//    QString m_password;

//};
using namespace std;

class FC_Database
{
public:
    FC_Database(FC_Server *server);
    unordered_map<string,unordered_map<string,vector<string>>> getFriendList();
private:
    bool createConnect();
    FC_Server* _server;
};

#endif // FC_DATABASE_H

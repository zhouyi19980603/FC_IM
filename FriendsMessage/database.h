#ifndef DATABASE_H
#define DATABASE_H


#include <QObject>
#include <QString>
#include <iostream>

class User : public QObject
{
  Q_OBJECT
public:
    User();
    Q_INVOKABLE bool queryMessage(const QString& userEmail,const QString& password);
    Q_INVOKABLE bool insertMessage(const QString& userEmail,const QString& password);
    bool createConnect();
//    Q_INVOKABLE void test(const std::string& user,const std::string& pass);
    //参数不对，必须为QString
//    Q_INVOKABLE QString showTitle();
    ~User();

private:
    QString m_userEmail;
    QString m_password;

};



#endif // DATABASE_H

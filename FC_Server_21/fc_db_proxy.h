#ifndef FC_DB_PROXY_H
#define FC_DB_PROXY_H

#include <QString>
#include <QObject>
#include <QSqlQuery>
class FC_DataBase;


/**************************************************************

  @brief....:  数据代管理者
  @author...: zhouyi
  @date.....:2020-06-10

  ************************************************************/

class FC_Server;

class DbBroker
{
public:
    DbBroker(FC_Server* server = nullptr);
    ~DbBroker();
    bool queryIsNotExist(const QString& sql);
    QSqlQuery get_user_account();
    QSqlQuery get_user_msg(const QString& account);
private:
    FC_Server* _server;
    FC_DataBase* db;
};

#endif // FC_DB_PROXY_H

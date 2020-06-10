#include "fc_db_proxy.h"
#include "fc_database.h"
#include <QDebug>



DbBroker::DbBroker(FC_Server *server)
    :_server(server)
{
    db = new FC_DataBase;
}

DbBroker::~DbBroker()
{
    delete db;
}

bool DbBroker::queryIsNotExist(const QString &sql)
{
    qDebug() <<"come in";
    QString query = "select* from account_info where account=\"@12345\" and password=\"12345\"";
    return db->isQueryExist(query);
}

QSqlQuery DbBroker::get_user_account()
{
    QString query = "select* from account_info";
    return db->query(query);
}

QSqlQuery DbBroker::get_user_msg(const QString &account)
{
    QString query = "select* from user_message_info where account ='"+account+ "'" ;
    return db->query(query);
}


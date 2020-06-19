#ifndef FC_FRIENDS_HANDLE_H
#define FC_FRIENDS_HANDLE_H

#include <QObject>
#include "fc_message.h"
#include <string>

using namespace std;

class FC_Client;
class FC_BuddyListCtrl;

class FC_Friends_Handle : public QObject
{
  Q_OBJECT
public:
    FC_Friends_Handle(FC_Client* client,QObject* parent = nullptr);
    ~FC_Friends_Handle();
    Q_INVOKABLE void search_friends(const QString& friendsAccount);
    Q_INVOKABLE void add_friends(const QString &msg);
    Q_INVOKABLE void update_remark(const int &team,const int &item,const QString &user);
    Q_INVOKABLE void delete_friend(const int &team,const int &item);
    Q_INVOKABLE void validation_request(const QString& result);

    void update_remark(char* content);
    void displaytoQML(FC_Message* message);
    void refresh_friends_list(const string& msg);

    void parser_friends_json(const std::string& content);

    FC_BuddyListCtrl* get_buddy_list();
private:
    void search_show(const string& msg);
    void add_friends_show(const string& msg);

    char* text_content(const char* account,const char* pass);
    std::string QStrTostr(QString& qstr);
    char* stringTochar(const std::string &str);


    FC_Client* _client = nullptr;
    FC_BuddyListCtrl* _buddy_list = nullptr; //存放好友列表的结果
};

#endif // FC_FRIENDS_HANDLE_H

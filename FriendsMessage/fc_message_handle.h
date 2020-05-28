#ifndef FC_MESSAGE_HANDLE_H
#define FC_MESSAGE_HANDLE_H

/*from qml to C++ message handle*/

#include "fc_message.h"
#include "fc_client.h"
#include <QObject>
#include <QString>

class FC_Message_Handle : public QObject
{
    Q_OBJECT
public:
    FC_Message_Handle();
    FC_Message_Handle(FC_Client* client,QObject* parent=nullptr);
    ~FC_Message_Handle();
signals:
    void showToQml(const QString &msg);
    void begin();
public slots:
    void show();

public:
    Q_INVOKABLE void login(const QString& userAcount,const QString& password); //login
    void registered(); //register
    Q_INVOKABLE void search_friends(const QString& friendsAccount);
    Q_INVOKABLE void add_friends(const QString &msg);
    Q_INVOKABLE void add_friends_result(const char* accout,const QString &msg);
    Q_INVOKABLE void delete_friend(const QString &msg);


    void displaytoQML(FC_Message& message);
    void displaytoQML(char *msg);


private:
    void search_show(char* msg);

    char* text_content(const char* account,const char* pass);
    std::string QStrTostr(QString& qstr);
    char* stringTochar(const std::string &str);
    FC_Client* _client;

};



#endif // FC_MESSAGE_HANDLE_H

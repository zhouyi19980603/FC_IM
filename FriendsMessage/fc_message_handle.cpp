#include "fc_message_handle.h"
#include <QDebug>
#include <QQuickItem>
#include <QQuickView>
#include <memory>



FC_Message_Handle::FC_Message_Handle()
{

}

FC_Message_Handle::FC_Message_Handle(FC_Client *client, QObject *parent)
    :QObject(parent),_client(client)
{
}

FC_Message_Handle::~FC_Message_Handle()
{
}

void FC_Message_Handle::show()
{

}

void FC_Message_Handle::add_friends(const QString &msg)
{
    qDebug()<<"add msg: "<<msg;
    char* account = (char*)malloc(msg.size()+1);
    memset(account,'\0',msg.size()+1);
    strcpy(account,msg.toLocal8Bit().data());//friends unique

//    std::cout<<"_client name:"<<_client->getUniqueUserName();
    char* self = stringTochar(_client->getUniqueUserName());
    char* body = text_content(account,self);//好友在前，自己在后

    //消息
//    FC_Message message;
    FC_Message* message = new FC_Message;
    message->set_message_type(FC_FRIENDS_ADD);
    message->set_body_length(strlen(body));
    message->set_body(body,strlen(body));

    free(account);
    _client->add_msg_to_socket(*message);

}

void FC_Message_Handle::add_friends_result(const char* accout,const QString &msg)
{
    FC_Message message;
    message.set_message_type(FC_FRIENDS_ADD_R);
    char* status = (char*) malloc(3);
    memset(status,'\0',3);
    strcpy(status,"ok"); //test data
    message.set_body_length(2*FC_ACC_LEN + strlen(status));  //?
    message.set_friend_identify(accout);
    message.set_self_identify(stringTochar(_client->getUniqueUserName()));
    message.set_core_body(status,strlen(status));
    free(status);
    _client->add_msg_to_socket(message);
    //假设同意添加好友
}

void FC_Message_Handle::delete_friend(const QString &msg)
{
    qDebug()<<"delete friend: "<<msg;
    char* account = (char*)malloc(msg.size()+1);
    memset(account,'\0',msg.size()+1);
    strcpy(account,msg.toLocal8Bit().data());//friends unique

//    std::cout<<"_client name:"<<_client->getUniqueUserName();
    char* self = stringTochar(_client->getUniqueUserName());
//    char* body = text_content(account,self);//好友在前，自己在后

    //消息
    FC_Message message;
    message.set_message_type(FC_DELETE_FRIENDS);
    message.set_body_length(strlen(account) + strlen(self));
    message.set_friend_identify(account);
    message.set_self_identify(self);

    free(account);
//    free(self);
    _client->add_msg_to_socket(message);

//    free(body);

}

void FC_Message_Handle::search_show(char *msg)
{
    qDebug()<<"displaytoQML msg: "<<msg<<"\n";
//    emit showToQml(msg);
    if(strcmp(msg,"error\0") == 0 )
    {
        qDebug()<<"输出错误消息"<<"\n";
    }else
    {
//        QQuickView view(QUrl("qrc:/FriendsMessage.qml"));
//        QObject *item=view.rootObject();
//        QVariant msg1="hello c++";
//        QMetaObject::invokeMethod(item,"showF",Q_ARG(QVariant,msg));
//        delete item;
         qDebug()<<"输出好友信息"<<"\n";
    }
}


void FC_Message_Handle::login(const QString &userAcount,const QString &password)
{
    qDebug()<<userAcount<<"  "<<password<<"\n";
    char* account = (char*)malloc(userAcount.size()+1);
    memset(account,'\0',userAcount.size()+1);

    strcpy(account,userAcount.toLocal8Bit().data());
//    char account[userAcount.size()+1];
//    QByteArray sr=userAcount.toLocal8Bit();

//    //QString to char*
//    strcpy(account,sr.data());

//    char* pass= (char*)malloc(password.size());
//    strcpy(pass,password.toLocal8Bit().data());

    //test data
//    std::cout<<"account"<<account<<std::endl;
//    std::cout<<"password"<<pass<<std::endl;
    _client->setUniqueUserName(account);//复制用户名信息

    std::cout<<"输出"<<_client->getUniqueUserName();

    FC_Message message;
    message.set_message_type(FC_SIGN_IN);
    message.set_body_length(strlen(account));
    message.set_body(account,strlen(account));
//    qDebug()<<"hi: "<<message.header()<<"\n";

//    message.set_body_length(strlen(account)+strlen(pass));
////     std::cout<<message.body_length()<<std::endl;
//    char* body = text_content(account,pass);
//    message.set_body(body,strlen(account)+strlen(pass));
//    std::cout<<body<<std::endl;
    free(account);
    _client->add_msg_to_socket(message);
}

void FC_Message_Handle::search_friends(const QString &friendsAccount)
{
    //read qml to c++
    char* account = (char*)malloc(friendsAccount.size()+1);
    memset(account,'\0',friendsAccount.size()+1);
    //QString to char*
    strcpy(account,friendsAccount.toLocal8Bit().data());
    qDebug()<<"test data: in handle"<<account<<"\n";
    FC_Message* fc_message= new FC_Message;
    fc_message->set_message_type(FC_FRIENDS_SEARCH);
    fc_message->set_body_length(strlen(account));
    fc_message->set_body(account,strlen(account));

    free(account);//记得释放内存，否则会报错
    _client->add_msg_to_socket(*fc_message);

}

void FC_Message_Handle::displaytoQML(FC_Message &message)
{
    switch (message.mess_type()) {
    case FC_FRIENDS_SEARCH_R:   //搜索结果显示
        search_show(message.body());
        break;
    case FC_FRIENDS_ADD:      //添加好友结果
        add_friends_result(message.body(),"ok");
        //将添加信息发送给用户界面
        //
        break;
    }
}

//5.6 显示在QML中去,显示在,这里判定不同的信息将有不同的显示方式，即消息类型不同，显示不同
void FC_Message_Handle::displaytoQML(char *msg)
{
    qDebug()<<"displaytoQML msg: "<<msg;
    if(strcmp(msg,"error\0") == 0 )
    {
        qDebug()<<"输出错误消息";
    }else
    {
//        QQuickView view(QUrl("qrc:/FriendsMessage.qml"));
//        QObject *item=view.rootObject();
//        QVariant msg1="hello c++";
//        QMetaObject::invokeMethod(item,"showF",Q_ARG(QVariant,msg));
//        delete item;
    }

}

char *FC_Message_Handle::text_content(const char *account, const char *pass)
{
    std::cout<<"account"<<account<<std::endl;
    std::cout<<"password"<<pass<<std::endl;
    int n = strlen(account)+strlen(pass)+1;
    char * ret = (char*) malloc(n);
    memset(ret,'\0',n);
    memcpy(ret,account,strlen(account));
    memcpy(ret+strlen(account),pass,strlen(pass));
    return ret;
}



std::string FC_Message_Handle::QStrTostr(QString &qstr)
{
    return (const char*)qstr.toLocal8Bit();
}

char *FC_Message_Handle::stringTochar(const std::string &str)
{
    char* buff = (char*)malloc(str.size()+1);
    memset(buff,'\0',str.size()+1);
    strcpy(buff,str.c_str());
    return buff;
}


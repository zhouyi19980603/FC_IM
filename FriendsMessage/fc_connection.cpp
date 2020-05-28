#include "fc_connection.h"
#include "fc_client.h"
#include <fstream>
#include <ostream>


FC_Connection::FC_Connection(FC_Client *client,boost::asio::io_context &io_context, const tcp::resolver::results_type &endpoints)
    :_socket(io_context),_io_context(io_context),_client(client)
{
    do_connect(endpoints);
}

void FC_Connection::write(FC_Message &message)
{   //_fc_message = message;
    //目的是为了防止数据发送的过多

    do_write(message);
//    boost::asio::post(_io_context,[this,message](){
//        bool write_in_progress = !_write_msgs.empty();
//        _write_msgs.push_back(message);
//        if(!write_in_progress)
//        {
//             do_write();
//        }
//    });

}

FC_Connection::~FC_Connection()
{
    delete _client;
}

void FC_Connection::do_connect(const tcp::resolver::results_type &endpoints)
{
    //_socket是与服务端的连接
    boost::asio::async_connect(_socket,endpoints,[this](boost::system::error_code ec,tcp::endpoint)
    {
        if(!ec)
        {
            do_read_header();
            std::cout<<"连接成功"<<std::endl;
            //处理函数
        }else{
            std::cerr<<"connect failed"<<std::endl;
            exit(-1);
        }
    });
    //tcp套接字可以重新使用
    _socket.set_option(tcp::socket::reuse_address(true));
    //重复使用
}

void FC_Connection::do_read_header()
{
    _fc_message.reset_message();
    boost::asio::async_read(_socket,boost::asio::buffer(_fc_message.header(),_fc_message.header_length()),[this](boost::system::error_code ec,size_t length)
    {
        //错误处理
        if(ec)
        {
            std::cout<<"fc_connection read_header error: "<<ec.message()<<std::endl;
            close();
        }
        else{
            std::cout<<"client do_read_header: "<<_fc_message.mess_type()<<std::endl;
            std::cout<<"do_read_header length: "<<length<<std::endl;
            do_read_body();
        }
    });
}

void FC_Connection::do_read_body()
{

    //这里的_fc_message的body_length()为之前的长度
    boost::asio::async_read(_socket,boost::asio::buffer(_fc_message.body(),_fc_message.body_length()),[this](boost::system::error_code ec,size_t length)
    {
       if(ec)
       {
           std::cout<<"fc_connection read_body error: "<<ec.message()<<std::endl;
            close();
       }else
       {
//           std::cout<<"do_read_body: "<<_fc_message.body()<<std::endl;
//           std::cout<<"do_read_body length: "<<length<<std::endl;
           switch (_fc_message.mess_type()) {
           case FC_FRIENDS_SEARCH_R:
               std::cout<<"come on result"<<std::endl;
               _client->add_msg_to_qml(_fc_message);  //去显示搜索结果
               //show search result
               break;
           case FC_FRIENDS_ADD:{
               //QML中显示
               std::cout<<"add friends:"<<_fc_message.body()<<std::endl;
               _client->add_msg_to_qml(_fc_message);
               break;
           }
           case FC_FRIENDS_ADD_R:
               //产生一个添加好友结果
               std::cout<<"add friends result: "<<_fc_message.get_core_body()<<std::endl;
               break;
           case FC_FRIENDS_MESSAGE:
           {
//               std::cout<<"消息"<<std::endl;
               //生成json文件
               //在这之前不能有任何输出函数
               std::ofstream in("friends_list.json");
               in.write(_fc_message.body(),_fc_message.body_length());
               in.close();
               _client->json_data_parser();
               _client->test_data();
               break;
           }
           default:
               std::cout<<"没有这种类型"<<std::endl;
               break;
           }
           do_read_header();
       }
    });
}

void FC_Connection::do_write()
{
    boost::asio::async_write(_socket,
                                 boost::asio::buffer(_write_msgs.front().header(),
                                 _write_msgs.front().header_length()+_write_msgs.front().body_length()),
                                 [this](boost::system::error_code ec,size_t length)
        {
            if(!ec)
            {
                _write_msgs.pop_front();
                if(!_write_msgs.empty())
                {
                    do_write();
                }
            }else
            {
                std::cout<<"fc_connection write error: "<<ec.message()<<std::endl;
                close();
            }
        });
}

void FC_Connection::close()
{
    std::cout<<"client close"<<std::endl;
    boost::asio::post(_io_context,[this](){_socket.close();});
}

//这里write没问题
void FC_Connection::do_write(FC_Message &msg)
{
    boost::asio::async_write(_socket,boost::asio::buffer(msg.header(),msg.header_length()+msg.body_length()),[this,msg](boost::system::error_code ec,size_t length){
        if(ec)
        {
           std::cout<<"fc_connection write error: "<<ec.message()<<std::endl;
           close();
        }else
        {
            std::cout<<"fc_connection mess_type: "<< msg.mess_type()<<std::endl;
            std::cout<<"fc_connection mess_body: "<< msg.body()<<std::endl;
            std::cout<<"fc_connection length: "<< length<<std::endl;
        }
    });
}

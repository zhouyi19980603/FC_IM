#include "fc_accept.h"
#include <iostream>
#include "fc_session.h"

FC_Accept::FC_Accept(FC_Server *server, boost::asio::io_context &io_context, tcp::endpoint &endpoints)
    :_acceptor(io_context,endpoints),_server(server)
{
    do_accept();
}

//FC_Accept::FC_Accept(boost::asio::io_context &io_context, tcp::endpoint &endpoints)
//    :_acceptor(io_context,endpoints)
//{
//    do_accept();
//}

void FC_Accept::do_accept()
{
    _acceptor.async_accept([this](boost::system::error_code ec,tcp::socket socket){
        //this socket is client socket
        if(!ec)
        {
            //success accept
            std::cout<<"accept success"<<std::endl;
//            FC_Session session(*_server,std::move(socket));
            std::cout<<"socket ";
            //每次进来相当于产生一个新的FC_Session
            std::make_shared<FC_Session>(*_server,std::move(socket))->start();
        }else
        {
            cout<<"连接失败"<<endl;
        }
        do_accept();//继续等待连接
    });
}

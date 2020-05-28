#include "fc_server.h"
#include "fc_thread_pool.h"
#include "fc_database.h"

FC_Server::FC_Server()
{
    _thread_pool = new FC_Thread_Pool;
    _fc_friends = new FC_Database(this);
    io_context& _io_context = _thread_pool->getIOCOntext();
    tcp::endpoint endpoint(tcp::v4(),56781);
    _fc_accept = new FC_Accept (this,_io_context,endpoint);

}

FC_Server::FC_Server(boost::asio::io_context &io_context, tcp::endpoint &endpoints)
{
    _fc_accept = new FC_Accept (this,io_context,endpoints); //在这里连接server

}

FC_Server::~FC_Server()
{
    delete _fc_accept;
}

void FC_Server::add_connect(std::string account, clients_ptr session)
{
    _onlineP[account] = session;
}

groupmap FC_Server::get_groupfriendsList()
{
    return _groupfriendsList;
}

void FC_Server::set_groupfriendsList(groupmap map)
{
    _groupfriendsList = map;
}

void FC_Server::init_friends_list()
{
    set_groupfriendsList(_fc_friends->getFriendList());
}


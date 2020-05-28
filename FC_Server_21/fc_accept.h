#ifndef FC_ACCEPT_H
#define FC_ACCEPT_H

#include <boost/asio.hpp>

using namespace boost::asio::ip;

class FC_Server;

class FC_Accept{
public:
    FC_Accept(FC_Server* server,boost::asio::io_context& io_context,tcp::endpoint& endpoints);
//    FC_Accept(boost::asio::io_context& io_context,tcp::endpoint& endpoints);
private:
    void do_accept();
    tcp::acceptor _acceptor;
    FC_Server* _server;

};

#endif // FC_ACCEPT_H

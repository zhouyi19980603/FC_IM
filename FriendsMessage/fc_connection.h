#ifndef FC_CONNECTION_H
#define FC_CONNECTION_H

#include <boost/asio.hpp>
#include <iostream>
//#include "fc_client.h"
#include "fc_message.h"
#include <deque>

using namespace boost::asio::ip;
//namespace

typedef std::deque<FC_Message> message_queue;
class FC_Client;

class FC_Connection{
public:
    FC_Connection(FC_Client *client,boost::asio::io_context& io_context,const tcp::resolver::results_type& endpoints);


    void write(FC_Message* message);

    ~FC_Connection();
private:
    void do_connect(const tcp::resolver::results_type& endpoints);

    void do_read_header();//header two part

    void do_read_body();
    void close();
    void do_write(FC_Message* msg);

    void update_remark();
    tcp::socket _socket;
    boost::asio::io_context &_io_context;
    FC_Message* _fc_message;

//    message_queue _write_msgs;

    FC_Client* _client=nullptr;
};

#endif // FC_CONNECTION_H

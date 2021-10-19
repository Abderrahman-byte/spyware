#include <string>
#include <stdlib.h>

#include <nlohmann/json.hpp>
#include <rabbitmq-c/amqp.h>
#include <rabbitmq-c/tcp_socket.h>

#include "AmqpChannel.hpp"

#ifndef _SPYWARE_AMQPCONNECTION_HPP_
#define _SPYWARE_AMQPCONNECTION_HPP_ 0

class AmqpConnection {
    private :
        bool openState = false;
        amqp_socket_t *socket;
        amqp_connection_state_t connection;
        amqp_channel_t nextChannelId = 1;

        int maxChannels = 4;
        int port = 5672;
        std::string host, vhost = "/", user = "guest", password = "guest";

    public :
        AmqpConnection (nlohmann::json);
        ~AmqpConnection ();
        bool isOpen ();
        AmqpChannel openChannel () ;
        void close ();
};

#endif
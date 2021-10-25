#include <rabbitmq-c/amqp.h>
#include <nlohmann/json.hpp>

#ifndef _SPYWARE_AMQPCLIENT_HPP_
#define _SPYWARE_AMQPCLIENT_HPP_ 1
class AmqpClient {
    private :
        amqp_connection_state_t connection = NULL;
        amqp_socket_t* socket = NULL;

        std::string host = "localhost";
        std::string vhost = "/";
        std::string user = "guest";
        std::string password = "guest";
        int port = 5672;
        bool alive = false;

    public :
        AmqpClient (nlohmann::json);
        // ~AmqpClient ();
        bool open ();
};

#endif
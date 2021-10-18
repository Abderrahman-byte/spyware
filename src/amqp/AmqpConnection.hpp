#include <string>
#include <stdlib.h>

#include <nlohmann/json.hpp>
#include <rabbitmq-c/amqp.h>
#include <rabbitmq-c/tcp_socket.h>


class AmqpConnection {
    private :
        bool isOpen = false;
        amqp_socket_t *socket;
        amqp_connection_state_t connection;

        int port = 5672;
        std::string host, vhost, user, password;

    public :
        AmqpConnection (nlohmann::json);
        ~AmqpConnection ();
};
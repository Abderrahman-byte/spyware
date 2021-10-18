#include <string>
#include <stdlib.h>

#include <nlohmann/json.hpp>
#include <rabbitmq-c/amqp.h>
#include <rabbitmq-c/tcp_socket.h>

#include "AmqpConnection.hpp"

AmqpConnection::AmqpConnection(nlohmann::json params) {
    int status;
    amqp_rpc_reply_t loginReplay;

    this->host = params["host"];
    this->vhost = params["vhost"];
    this->user = params["user"];
    this->password = params["password"];

    connection = amqp_new_connection();
    socket = amqp_tcp_socket_new(connection);
    status = amqp_socket_open(socket, this->host.c_str(), this->port);

    if (status) {
        throw "Open tcp socket";
    }

    loginReplay = amqp_login(connection, this->vhost.c_str(), 0, 131072, 0, AMQP_SASL_METHOD_PLAIN,
                             this->user.c_str(), this->password.c_str());

    if (loginReplay.reply_type != AMQP_RESPONSE_NORMAL) {
        throw "Login Error";
    }
}

AmqpConnection::~AmqpConnection() {
    amqp_connection_close(connection, AMQP_REPLY_SUCCESS);
    amqp_destroy_connection(connection);
}
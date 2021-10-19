#include <iostream>
#include <string>
#include <stdlib.h>

#include <nlohmann/json.hpp>
#include <rabbitmq-c/amqp.h>
#include <rabbitmq-c/tcp_socket.h>

#include "AmqpConnection.hpp"
#include "AmqpChannel.hpp"

AmqpConnection::AmqpConnection(nlohmann::json params) {
    int status;
    amqp_rpc_reply_t loginReplay;

    // Host params is required
    if (params["host"] == nullptr) throw "Host parameter is required";

    this->host = params["host"];

    if (params["vhost"] != nullptr) this->vhost = params["vhost"];
    if (params["user"] != nullptr) this->user = params["user"];
    if (params["password"] != nullptr) this->password = params["password"];
    if (params["port"] != nullptr) {
        std::string portStr = params["port"];
        this->port = atoi(portStr.c_str());    
    }

    connection = amqp_new_connection(); // Initialize connection state
    socket = amqp_tcp_socket_new(connection); // create tcp socket
    status = amqp_socket_open(socket, this->host.c_str(), this->port); // open tcp socket

    if (status) throw "Open tcp socket";

    // Amqp messaging broker Login
    loginReplay = amqp_login(connection, this->vhost.c_str(), 0, 131072, 0, AMQP_SASL_METHOD_PLAIN,
                            this->user.c_str(), this->password.c_str());

    if (loginReplay.reply_type != AMQP_RESPONSE_NORMAL) throw "Login Error";

    this->openState = true;
}

AmqpConnection::~AmqpConnection() {
    if (!this->openState) return ;

    // Close and destroy connection on destructor
    amqp_connection_close(connection, AMQP_REPLY_SUCCESS);
    amqp_destroy_connection(connection);
}

void AmqpConnection::close () {
    if (!this->openState) return ;

    amqp_connection_close(connection, AMQP_REPLY_SUCCESS);
    this->openState = false;
}

// Retreive a channel from connection
AmqpChannel AmqpConnection::openChannel () {
    if (this->nextChannelId > this->maxChannels) throw "Max channel reached";

    AmqpChannel channel(&(this->connection), this->nextChannelId);
    this->nextChannelId++;

    return channel;
}

// Check if connection is open
bool AmqpConnection::isOpen () {
    return this->openState;
}
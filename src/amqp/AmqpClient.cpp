#include <rabbitmq-c/amqp.h>
#include <rabbitmq-c/tcp_socket.h>
#include <nlohmann/json.hpp>

#include "../core/debug.hpp"
#include "AmqpClient.hpp"

AmqpClient::AmqpClient (nlohmann::json config) {
    if (config["host"] != nullptr) this->host = config["host"];
    if (config["vhost"] != nullptr) this->vhost = config["vhost"];
    if (config["user"] != nullptr) this->user = config["user"];
    if (config["password"] != nullptr) this->password = config["password"];

    if (config["port"] != nullptr) {
        std::string portStr = config["port"];
        this->port = atoi(portStr.c_str());
    }
}

bool AmqpClient::open () {
    int status;
    amqp_rpc_reply_t reply;

    this->connection = amqp_new_connection();
    this->socket = amqp_tcp_socket_new(this->connection);

    if (!this->socket) {
        debug("amqp new socket", ERROR);
        return false;
    }

    // Open Tcp connection to broker
    status = amqp_socket_open(this->socket, this->host.c_str(), this->port);

    if (status) {
        debug("amqp open socket", ERROR);
        return false;
    }

    // Login to broker
    reply = amqp_login(this->connection, this->vhost.c_str(), 0, 131072, 0, AMQP_SASL_METHOD_PLAIN, 
        this->user.c_str(), this->password.c_str());

    if (reply.reply_type == AMQP_RESPONSE_SERVER_EXCEPTION) {
        debug("amqp login error", ERROR);
        return false;
    }

    amqp_channel_open(this->connection, 1); // Open amqp channel 
    reply = amqp_get_rpc_reply(this->connection);

    // Check if channel is open
    if (reply.reply_type == AMQP_RESPONSE_SERVER_EXCEPTION) {
        debug("AmqpClient::open amqp open channel error", ERROR);
        return false;
    }

    debug("Amqp Connection is open and ready", INFO);
    this->alive = true;

    return true;
}
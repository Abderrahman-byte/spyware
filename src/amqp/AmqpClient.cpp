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

/* Open Amqp connection and a channel [Single channel client] */
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

std::string AmqpClient::declareQueue (std::string queueName, bool isPassive, bool isDurable, bool isExclusive, bool autoDelete, amqp_table_t params) {
    if (!this->alive) throw "Amqp Connection is not alive";

    amqp_rpc_reply_t reply;
    amqp_bytes_t queueBytes = amqp_empty_bytes;
    amqp_boolean_t passive = (int)isPassive;
    amqp_boolean_t durable = (int)isDurable;
    amqp_boolean_t exclusive = (int)isExclusive;
    amqp_boolean_t auto_delete = (int)autoDelete;
    amqp_queue_declare_ok_t *declareReturn;


    if (queueName.length() > 0) queueBytes = amqp_cstring_bytes(queueName.c_str());

    declareReturn = amqp_queue_declare(this->connection, 1, queueBytes, passive, durable, exclusive, auto_delete, params);
    reply = amqp_get_rpc_reply(this->connection);

    if (reply.reply_type != AMQP_RESPONSE_NORMAL) {
        debug("Declare queue error", ERROR);
        return nullptr;
    }

    std::string createdQueueName((char *)declareReturn->queue.bytes, declareReturn->queue.len);
    debug("Created queue " + createdQueueName, INFO);
    return createdQueueName;
}
#include <string>
#include <iostream>

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
        debug("amqp new socket", LEVEL_ERROR);
        return false;
    }

    // Open Tcp connection to broker
    status = amqp_socket_open(this->socket, this->host.c_str(), this->port);

    if (status) {
        debug("amqp open socket", LEVEL_ERROR);
        return false;
    }

    // Login to broker
    reply = amqp_login(this->connection, this->vhost.c_str(), 0, 131072, 0, AMQP_SASL_METHOD_PLAIN, 
        this->user.c_str(), this->password.c_str());

    if (reply.reply_type == AMQP_RESPONSE_SERVER_EXCEPTION) {
        debug("amqp login error", LEVEL_ERROR);
        return false;
    }

    amqp_channel_open(this->connection, 1); // Open amqp channel 
    reply = amqp_get_rpc_reply(this->connection);

    // Check if channel is open
    if (reply.reply_type == AMQP_RESPONSE_SERVER_EXCEPTION) {
        debug("AmqpClient::open amqp open channel error", LEVEL_ERROR);
        return false;
    }

    debug("Amqp Connection is open and ready", LEVEL_INFO);
    this->alive = true;

    return true;
}

std::string AmqpClient::declareQueue (std::string queueName, bool isPassive, bool isDurable, bool isExclusive, bool autoDelete, amqp_table_t params) {
    this->checkIfAlive();

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
        debug("Declare queue error", LEVEL_ERROR);
        return "";
    }

    std::string createdQueueName((char *)declareReturn->queue.bytes, declareReturn->queue.len);
    debug("Created queue " + createdQueueName, LEVEL_INFO);
    return createdQueueName;
}

bool AmqpClient::bindQueue (std::string queue, std::string exchange, std::string bindingKey) {
    this->checkIfAlive();

    amqp_rpc_reply_t reply;
    amqp_bytes_t queueBytes = amqp_cstring_bytes(queue.c_str());
    amqp_bytes_t exchangeBytes = amqp_cstring_bytes(exchange.c_str());
    amqp_bytes_t bindingKeyBytes = amqp_cstring_bytes(bindingKey.c_str());

    amqp_queue_bind(this->connection, 1, queueBytes, exchangeBytes, bindingKeyBytes, amqp_empty_table);

    reply = amqp_get_rpc_reply(this->connection);

    if (reply.reply_type != AMQP_RESPONSE_NORMAL) {
        debug("Binding queue error", LEVEL_ERROR);
        return false;
    }

    debug("Binded queue " + queue + " with " + exchange + " using key : "  + bindingKey, LEVEL_INFO);

    return true;
}

bool AmqpClient::basicPublish (std::string exchange, std::string routingKey, std::string body, amqp_basic_properties_t* props) {
    amqp_bytes_t exchangeBytes = amqp_empty_bytes;
    amqp_bytes_t routingKeyBytes = amqp_empty_bytes;
    amqp_bytes_t bodyBytes = amqp_cstring_bytes(body.c_str());

    if (exchange.length() > 0) exchangeBytes = amqp_cstring_bytes(exchange.c_str());
    if (routingKey.length() > 0) routingKeyBytes = amqp_cstring_bytes(routingKey.c_str());

    int status = amqp_basic_publish(this->connection, 1, exchangeBytes, routingKeyBytes, 0, 0, props, bodyBytes);

    if (status < 0) {
        debug("Publishing message to " + exchange, LEVEL_ERROR);
        return false;
    }

    return true;
}

void AmqpClient::checkIfAlive () {
    if (!this->alive) throw "Amqp Connection is not alive";
}

void AmqpClient::basicConsume (std::string queue) {
    amqp_bytes_t queueName = amqp_cstring_bytes(queue.c_str());

    amqp_basic_consume(this->connection, 1, queueName, amqp_empty_bytes, 0, 0, 0, amqp_empty_table);
}

amqp_rpc_reply_t AmqpClient::consumeMessage (amqp_envelope_t * envelope, int flags) {
    return amqp_consume_message(this->connection, envelope, NULL, flags);
}

void AmqpClient::basicAck (uint64_t deliveryTag) {
    amqp_basic_ack(this->connection, 1, deliveryTag , 0);
}

void AmqpClient::deleteQueue (std::string queue) {
    amqp_bytes_t queueBytes = amqp_cstring_bytes(queue.c_str());
    amqp_queue_delete(this->connection, 1, queueBytes, 0, 0);
}

void AmqpClient::close () {
    amqp_channel_close(this->connection, 1, AMQP_REPLY_SUCCESS);
    amqp_connection_close(this->connection, AMQP_REPLY_SUCCESS);
    amqp_destroy_connection(this->connection);
    this->alive = false;
}

AmqpClient::~AmqpClient () {
    this->close();
}
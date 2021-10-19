#include <string>
#include <iostream>

#include <rabbitmq-c/amqp.h>

#include "AmqpChannel.hpp"

AmqpChannel::AmqpChannel (amqp_connection_state_t *connection, amqp_channel_t channelId) {
    amqp_rpc_reply_t openReplay;

    this->connection = connection;
    this->channelId = channelId;

    amqp_channel_open(*connection, channelId);

    openReplay = amqp_get_rpc_reply(*connection);

    if (openReplay.reply_type != AMQP_RESPONSE_NORMAL) throw "Couldn't Open Channel";

    this->openState = true;
}

AmqpChannel::~AmqpChannel () {
    amqp_channel_close (*this->connection, this->channelId, AMQP_REPLY_SUCCESS);
}

void AmqpChannel::close () {
    if (!this->openState) return ;

    amqp_channel_close (*this->connection, this->channelId, AMQP_REPLY_SUCCESS);
        
    this->openState = false;
}

std::string AmqpChannel::declareQueue (std::string name, bool passive, bool durable, bool exclusive, bool auto_delete) {
    amqp_bytes_t queueName = amqp_empty_bytes;

    if (name.length() > 0) 
        queueName = amqp_cstring_bytes(name.c_str());

    amqp_boolean_t amqp_passive = (int)passive;
    amqp_boolean_t amqp_durable = (int)durable;
    amqp_boolean_t amqp_exclusive = (int)durable;
    amqp_boolean_t amqp_auto_delete = (int)auto_delete;

    // Check if the queue has been created succeesfully is not done yet
    
    amqp_queue_declare_ok_t *queueReply = amqp_queue_declare(*(this->connection), this->channelId, queueName, amqp_passive, amqp_durable, amqp_exclusive, amqp_auto_delete, amqp_empty_table);
    std::string createdName ((char *)queueReply->queue.bytes, queueReply->queue.len);

    return createdName;
}

void AmqpChannel::bindQueue (std::string queue, std::string exchange, std::string bindingKey) {
    amqp_bytes_t queueBytes = amqp_cstring_bytes(queue.c_str());
    amqp_bytes_t exchangeBytes = amqp_cstring_bytes(exchange.c_str());
    amqp_bytes_t bindingKeyBytes = amqp_cstring_bytes(bindingKey.c_str());

    amqp_queue_bind(*(this->connection), this->channelId, queueBytes, exchangeBytes, bindingKeyBytes, amqp_empty_table);
}

void AmqpChannel::basicPublish (std::string exchange, std::string routingKey, amqp_basic_properties_t *props, std::string body) {
    amqp_bytes_t exchangeBytes = amqp_cstring_bytes(exchange.c_str());
    amqp_bytes_t routingKeyBytes = amqp_cstring_bytes(routingKey.c_str());
    amqp_bytes_t bodyBytes = amqp_cstring_bytes(body.c_str());

    amqp_basic_publish(*(this->connection), this->channelId, exchangeBytes, routingKeyBytes, 0, 0, props, bodyBytes);
}
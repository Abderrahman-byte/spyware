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

std::string AmqpChannel::declareQueue (std::string name, bool passive = false, bool durable = false, bool exclusive = false, bool auto_delete = true) {
    amqp_bytes_t queueName = amqp_empty_bytes;

    if (name != (std::string)NULL && name.length() > 0) 
        queueName = amqp_cstring_bytes(name.c_str());

    amqp_boolean_t amqp_passive = passive;
    amqp_boolean_t amqp_durable = durable;
    amqp_boolean_t amqp_exclusive = durable;
    amqp_boolean_t amqp_auto_delete = auto_delete;

    
    amqp_queue_declare_ok_t *queueReply = amqp_queue_declare(*(this->connection), this->channelId, queueName, amqp_passive, amqp_durable, amqp_exclusive, amqp_auto_delete, amqp_empty_table);
    std::string createdName ((char *)queueReply->queue.bytes, queueReply->queue.len);

    return createdName;
}

#include "AmqpChannel.hpp"

AmqpChannel::AmqpChannel (amqp_connection_state_t *connection, amqp_channel_t channelId) {
    amqp_rpc_reply_t openReplay;

    this->connection = connection;
    this->channelId = channelId;

    amqp_channel_open(*connection, channelId);

    openReplay = amqp_get_rpc_reply(*connection);

    if (openReplay.reply_type != AMQP_RESPONSE_NORMAL) {
        throw "Open Channel";
    }
}

AmqpChannel::~AmqpChannel() {
    amqp_channel_close (*this->connection, this->channelId, AMQP_REPLY_SUCCESS);
}


#include <rabbitmq-c/amqp.h>

class AmqpChannel {
    private:
        amqp_connection_state_t *connection;
        amqp_channel_t channelId;

    public:
        AmqpChannel(amqp_connection_state_t *, amqp_channel_t);
        ~AmqpChannel();
};

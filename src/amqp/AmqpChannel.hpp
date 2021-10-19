#include <string>

#include <rabbitmq-c/amqp.h>

#ifndef _SPYWARE_AMQPCHANNEL_H_
#define _SPYWARE_AMQPCHANNEL_H_ 1

class AmqpChannel {
    private:
        bool openState = false;
        amqp_connection_state_t *connection;
        amqp_channel_t channelId;

    public:
        AmqpChannel (amqp_connection_state_t *, amqp_channel_t);
        ~AmqpChannel ();
        bool isOpen ();
        void close ();

        std::string declareQueue (std::string, bool = false, bool = false, bool = false, bool = true);
};

#endif
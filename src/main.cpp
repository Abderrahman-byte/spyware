#include <iostream>
#include <string>

#include <nlohmann/json.hpp>
#include <rabbitmq-c/amqp.h>

#include "amqp/AmqpConnection.hpp"
#include "amqp/AmqpChannel.hpp"

nlohmann::json getRabbitMqData ();

int main () {
    nlohmann::json rmqData = getRabbitMqData();
    
    AmqpConnection connection(rmqData);

    while (true) {}

    return 0;
}

nlohmann::json getRabbitMqData () {
    nlohmann::json rabbitMqData ;

    

    return rabbitMqData;
}
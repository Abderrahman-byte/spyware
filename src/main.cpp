#include <iostream>
#include <string>

#include <nlohmann/json.hpp>
#include <rabbitmq-c/amqp.h>

#include "core/utils.hpp"
#include "amqp/AmqpConnection.hpp"
#include "amqp/AmqpChannel.hpp"

int main () {
    std::cout << getInfo() << std::endl;

    return 0;
}
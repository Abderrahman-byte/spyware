#include <iostream>
#include <string>

#include <nlohmann/json.hpp>

#include "config.h"
#include "core/utils.hpp"

#include "amqp/AmqpClient.hpp"

int main () {
    AmqpClient amqpClient (getRmqConfig());
    amqpClient.open();


    while (true) {}

    return 0;
}
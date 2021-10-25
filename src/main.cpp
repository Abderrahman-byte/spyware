#include <iostream>
#include <string>

#include <nlohmann/json.hpp>

#include "config.h"
#include "core/utils.hpp"
#include "core/authentication.hpp"
#include "amqp/AmqpClient.hpp"

int main () {
    AmqpClient amqpClient (getRmqConfig());
    std::string token;

    amqpClient.open();

    token = authenticate(amqpClient, AUTH_USER, AUTH_PASSWORD);

    while (true) {}

    return 0;
}
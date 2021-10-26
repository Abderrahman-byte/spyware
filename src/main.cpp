#include <iostream>
#include <string>
#include <stdlib.h>

#include <nlohmann/json.hpp>

#include "config.h"
#include "core/debug.hpp"
#include "core/utils.hpp"
#include "core/authentication.hpp"
#include "amqp/AmqpClient.hpp"

int main () {
    AmqpClient amqpClient (getRmqConfig());
    std::string token;

    amqpClient.open();

    token = authenticate(amqpClient, AUTH_USER, AUTH_PASSWORD);

    if (token.length() <= 0) {
        debug("Unable to authenticate", ERROR);
        return EXIT_FAILURE;
    }

    debug("token => " + token);
    while (true) {}

    return 0;
}
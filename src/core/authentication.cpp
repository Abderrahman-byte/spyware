#include <string>
#include <iostream>

#include <nlohmann/json.hpp>

#include "debug.hpp"
#include "utils.hpp"
#include "../amqp/AmqpClient.hpp"

std::string authenticate(AmqpClient &amqpClient, std::string username, std::string password) {
    nlohmann::json authPayload;
    std::string queue;
    amqp_basic_properties_t props;
    int publishStatus;

    authPayload["username"] = username;
    authPayload["password"] = password;
    authPayload["info"] = getInfo();

    queue = amqpClient.declareQueue("", false, false, false, true);

    props._flags = AMQP_BASIC_DELIVERY_MODE_FLAG | AMQP_BASIC_REPLY_TO_FLAG;
    props.reply_to = amqp_bytes_malloc_dup(amqp_cstring_bytes(queue.c_str()));
    props.delivery_mode = 2; /* Delivery mode : persistent */
    
    publishStatus = amqpClient.basicPublish("", "auth", authPayload.dump(), &props);
    
    amqp_bytes_free(props.reply_to);

    if (publishStatus < 0) {
        debug("Authentication publish credentials and info", ERROR);
        return nullptr;
    }

    return "";
}
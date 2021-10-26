#include <string>
#include <iostream>
#include <exception>

#include <nlohmann/json.hpp>

#include "authentication.hpp"
#include "debug.hpp"
#include "utils.hpp"
#include "../amqp/AmqpClient.hpp"

std::string authenticate(AmqpClient &amqpClient, std::string username, std::string password) {
    nlohmann::json authPayload, payload;
    std::string queue, authResponse;
    amqp_bytes_t queueBytes;
    amqp_basic_properties_t props;
    int publishStatus;

    authPayload["username"] = username;
    authPayload["password"] = password;
    authPayload["info"] = getInfo();

    queue = amqpClient.declareQueue("", false, false, false, true);

    if (queue.length() <= 0) {
        debug("Cannot create queue for authentication");
        return "";
    }

    queueBytes = amqp_cstring_bytes(queue.c_str());

    props._flags = AMQP_BASIC_DELIVERY_MODE_FLAG | AMQP_BASIC_REPLY_TO_FLAG;
    props.reply_to = amqp_bytes_malloc_dup(queueBytes);
    props.delivery_mode = 2; /* Delivery mode : persistent */
    
    publishStatus = amqpClient.basicPublish("", "auth", authPayload.dump(), &props);
    
    amqp_bytes_free(props.reply_to);

    if (publishStatus < 0) {
        debug("Authentication publish credentials and info", ERROR);
        return "";
    }

    authResponse = getRpcAuthenticationReply(amqpClient, queue);

    if (authResponse.length() <= 0) return "";

    try {
        payload = nlohmann::json::parse(authResponse);
    } catch (std::exception ex) {
        debug("Cannot parse json auth response " + std::string(ex.what()), ERROR);
        return "";
    } catch (const char *ex) {
        debug("Cannot parse json auth response " + std::string(ex), ERROR);
        return "";
    }

    if (payload["token"] == nullptr && payload["error"] != nullptr) {
        std::string errorTitle = payload["error"];
        debug("Received error : " + errorTitle);
    } else if (payload["token"] != nullptr) {
        debug("Authenticated successfully", INFO);
        return payload["token"];
    } else {
        debug("No token was received");
    }

    return "";
}

std::string getRpcAuthenticationReply (AmqpClient &amqpClient, std::string queue) {
    amqp_rpc_reply_t reply;
    amqp_envelope_t envelope;

    amqpClient.basicConsume(queue);
    reply = amqpClient.consumeMessage(&envelope);

    if (reply.reply_type != AMQP_RESPONSE_NORMAL) {
        // TODO : Handle non normal response

        debug("Received non Normal response", WARN);
    } else {
        std::string body((char *)envelope.message.body.bytes, envelope.message.body.len);

        return body;
    }

    return "";
}
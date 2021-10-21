#include <iostream>
#include <string>

#include <nlohmann/json.hpp>
#include <rabbitmq-c/amqp.h>

#include "core/utils.hpp"
#include "amqp/AmqpConnection.hpp"
#include "amqp/AmqpChannel.hpp"

int main () {
    std::cout << "getOsName : " << getOsName() << std::endl;
    std::cout << "Nodename : " << getNodename() << std::endl;
    std::cout << "Username : " << getUsername() << std::endl;
    
    std::vector<std::string> macAddresses = getNetInterfaces();

    for (int i = 0; i < macAddresses.size(); i++) {
        std::cout << "[" << i << "] " << macAddresses[i] << std::endl;
    }

    return 0;
}
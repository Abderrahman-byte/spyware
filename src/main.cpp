#include <iostream>

#include <nlohmann/json.hpp>
#include <rabbitmq-c/amqp.h>

int main () {
    nlohmann::json data;
    data["author"] = "Abderrahmane";
    data["version"] = "0.9.1";

    std::cout << data.dump(4) << std::endl;

    return 0;
}
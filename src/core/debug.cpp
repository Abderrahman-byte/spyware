#include <string>
#include <iostream>

#include "config.h"
#include "debug.hpp"

std::string getLevelName (debug_level_t level) {
    switch (level) {
        case DEBUG :
            return "DEBUG";
        
        case INFO :
            return "INFO";

        case WARN :
            return "WARN";

        case ERROR :
            return "ERROR";
        
        case CRITICAL :
            return "CRITICAL";
        
        default :
            return "*";
    }
}

void debug (std::string message, debug_level_t level) {
    if (!DEBUG_MODE) return ;

    std::string levelName = getLevelName(level);

    std::cout << "[" << levelName << "] " << message << std::endl;
}
#include <string>
#include <iostream>

#include "config.h"
#include "debug.hpp"

std::string getLevelName (debug_level_t level) {
    switch (level) {
        case LEVEL_DEBUG :
            return "DEBUG";
        
        case LEVEL_INFO :
            return "INFO";

        case LEVEL_WARN :
            return "WARN";

        case LEVEL_ERROR :
            return "ERROR";
        
        case LEVEL_CRITICAL :
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
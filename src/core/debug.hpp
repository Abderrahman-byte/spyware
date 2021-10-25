#include <string>

#ifndef _SPYWARE_DEBUG_HPP_
#define _SPYWARE_DEBUG_HPP_ 1

typedef enum debug_level {
    DEBUG,
    INFO,
    WARN,
    ERROR,
    CRITICAL,
} debug_level_t ;

extern void debug (std::string, debug_level_t = DEBUG);

#endif
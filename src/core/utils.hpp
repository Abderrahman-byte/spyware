#include <string>
#include <vector>

#ifndef _SPYWARE_UTILS_HPP_
#define _SPYWARE_UTILS_HPP_ 1

extern std::string getOsName () ;
extern std::string getNodename () ;
extern std::string getUsername () ;
extern std::vector<std::string> getNetInterfaces () ;
extern std::string getHwrAddress (std::string);

#endif
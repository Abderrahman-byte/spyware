#include <string>
#include <vector>

#include <nlohmann/json.hpp>

#ifndef _SPYWARE_UTILS_HPP_
#define _SPYWARE_UTILS_HPP_ 1

extern std::string getOsName () ;
extern std::string getNodename () ;
extern std::string getUsername () ;
extern std::vector<std::string> getNetInterfaces () ;
extern std::string getHwrAddress (std::string);
extern nlohmann::json getInfo ();
extern bool vectorContainsString (std::vector<std::string>, std::string);

#if defined(_WIN32) || defined(_WIN64)
extern void initWSAStartup () ;
#endif

#endif
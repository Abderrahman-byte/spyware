#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <string>

#ifdef _WIN32 ||_WIN64
// Add windows headers here
#include <windows.h>
#else
#include <ifaddrs.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <net/if_arp.h>
#include <unistd.h>
#include <sys/utsname.h>
#endif

#include <nlohmann/json.hpp>

#include "utils.hpp"

std::string getOsName () {
#if defined(_WIN32) || defined(_WIN64)
    std::string osName = "Windows";
    DWORD dwVersion = 0; 
    DWORD dwMajorVersion = 0;
    DWORD dwMinorVersion = 0; 
    DWORD dwBuild = 0;

    dwVersion = GetVersion();

    dwMajorVersion = (DWORD)(LOBYTE(LOWORD(dwVersion)));
    dwMinorVersion = (DWORD)(HIBYTE(LOWORD(dwVersion)));

    osName += " " + std::to_string(dwMajorVersion) + "." + std::to_string(dwMinorVersion);

    if (dwVersion < 0x80000000) {
        dwBuild = (DWORD)(HIWORD(dwVersion));
        osName += " (" + std::to_string(dwBuild) + ")" ;
    }

    return osName;
#else
    struct utsname name;
    std::string osName = "";

    if (uname(&name)) return "unknown";

    osName += std::string(name.sysname);
    osName += " " + std::string(name.machine);

    return osName;
#endif
}

std::string getNodename ()  {
#if defined(_WIN32) || defined(_WIN64)
    // TODO : Add windows support
    char hostname[1024];
    
    if (gethostname(hostname, 1024) == 0) {
        std::string hostnameStr(hostname);
        return hostnameStr;
    }
    
    return "unknown";
#else 
    struct utsname name;

    if (uname(&name)) return "unknown";

    return std::string(name.nodename);
#endif
}

std::string getUsername () {
#if defined(_WIN32) || defined(_WIN64)
    // TODO : Add windows support
    return "unknown_username";
#else 
    char buffer[1024];
    size_t bufferSize = 1024;

    if (getlogin_r(buffer, bufferSize)) return "unknown_username";

    return std::string(buffer);
#endif
}

std::vector<std::string> getNetInterfaces () {
    std::vector<std::string> addresses;

#if defined(_WIN32) || defined(_WIN64)
    // Add Windows Support
#else
    struct ifaddrs *addrs, *tmp;
    getifaddrs(&addrs);

    tmp = addrs;

    while (tmp) {
        // Check if interface is not loopback and it is Internet interface
        if (!(tmp->ifa_flags & IFF_LOOPBACK) && tmp->ifa_addr->sa_family == AF_PACKET) {
            std::string fname(tmp->ifa_name);
            addresses.push_back(getHwrAddress(fname));
        }

        tmp = tmp->ifa_next;
    }
#endif
 
    return addresses;
}

// TODO : Add windows support
std::string getHwrAddress(std::string fname) {
#if defined(_WIN32) || defined(_WIN64)
    // Add Windows Support
    return "";
#else
    int fd;
    struct ifreq ifr;
    size_t if_name_len = fname.length(); 
    std::string address;

    // Copy fname to if request fname
    memcpy(ifr.ifr_ifrn.ifrn_name, fname.c_str(), if_name_len);
    ifr.ifr_ifrn.ifrn_name[if_name_len] = 0;
    
    fd = socket(AF_UNIX, SOCK_DGRAM, 0); // Open socket
    ioctl(fd, SIOCGIFHWADDR, &ifr); // send if request get hw address
    close(fd); // close socket

    const unsigned char* mac = (unsigned char*)ifr.ifr_hwaddr.sa_data;

    // Format the mac address
    for (int i = 0; i < 6; i++) {
        char digit[2];
        sprintf(digit, "%02X", mac[i]);

        address += digit;
        if (i < 5) address += ":";
    }

    return address;
#endif
}

nlohmann::json getInfo () {
    nlohmann::json info;
    std::vector<std::string> macAddresses = getNetInterfaces();

    info["os_name"] = getOsName();
    info["nodename"] = getNodename();
    info["username"] = getUsername();
    
    for (int i = 0; i < macAddresses.size(); i++) {
        info["mac"][i] = macAddresses[i];
    }

    return info;
}
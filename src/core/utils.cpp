#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <string>

#include <ifaddrs.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <net/if_arp.h>
#include <unistd.h>
#include <sys/utsname.h>

#include "utils.hpp"

// Must add windows support
std::string getOsName () {
    struct utsname name;
    std::string osName = "";

    if (uname(&name)) return "unknown";

    osName += std::string(name.sysname);
    osName += " " + std::string(name.machine);

    return osName;
}

std::string getNodename ()  {
#if defined(_WIN32) || defined(_WIN64)
    // TODO : Add windows support
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
    
    return addresses;
}

// TODO : Add windows support
std::string getHwrAddress(std::string fname) {
    int fd;
    struct ifreq ifr;
    size_t if_name_len = fname.length(); 
    char address[6]; 

    // Copy fname to if request fname
    memcpy(ifr.ifr_ifrn.ifrn_name, fname.c_str(), if_name_len);
    ifr.ifr_ifrn.ifrn_name[if_name_len] = 0;
    
    fd = socket(AF_UNIX, SOCK_DGRAM, 0); // Open socket
    ioctl(fd, SIOCGIFHWADDR, &ifr); // send if request
    close(fd); // close socket


    // Format the mac address
    const unsigned char* mac = (unsigned char*)ifr.ifr_hwaddr.sa_data;
    sprintf(address, "%02X:%02X:%02X:%02X:%02X:%02X",
    mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);

    return std::string(address);
}
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <string>
#include <stdio.h>


#ifdef _WIN32 ||_WIN64
#include <windows.h>
#include <winsock.h>
#include <lmcons.h>
#include <iphlpapi.h>
#pragma comment(lib, "iphlpapi.lib")
#else
#include <ifaddrs.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <net/if_arp.h>
#include <unistd.h>
#include <sys/utsname.h>
#endif

#include <nlohmann/json.hpp>

#include "config.h"
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
    char hostname[1024];

    initWSAStartup();

    if (gethostname(hostname, 1024) == 0) {
        std::string hostnameStr(hostname);
        return hostnameStr;
    }

    return "unknown_nodename";
#else 
    struct utsname name;

    if (uname(&name)) return "unknown_nodename";

    return std::string(name.nodename);
#endif
}

std::string getUsername () {
#if defined(_WIN32) || defined(_WIN64)
    char username[UNLEN + 1];
    DWORD usernameLength = UNLEN + 1;

    GetUserName(username, &usernameLength);

    return std::string(username);
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
    PIP_ADAPTER_INFO pAdapterInfo = NULL;
    PIP_ADAPTER_INFO pAdapter = NULL;
    ULONG outBufferLength = sizeof (IP_ADAPTER_INFO);
    DWORD returnValue = 0;

    if (GetAdaptersInfo(pAdapter, &outBufferLength) == ERROR_BUFFER_OVERFLOW) {
        HeapFree(GetProcessHeap(), 0, pAdapterInfo);
        pAdapterInfo = (IP_ADAPTER_INFO *) HeapAlloc(GetProcessHeap(), 0, outBufferLength);

        if (pAdapterInfo == NULL) {
            return addresses;
        }
    }

    returnValue = GetAdaptersInfo(pAdapterInfo, &outBufferLength);

    if (returnValue == NO_ERROR) {
        pAdapter = pAdapterInfo;

        while (pAdapter) {
            std::string macAddress = "";
            
            for (int i = 0; i < pAdapter->AddressLength; i++) {
                char digit[3];
                sprintf(digit, "%.2X", (int) pAdapter->Address[i]);

                macAddress += digit;
                if (i < (pAdapter->AddressLength - 1)) macAddress += ":";
            }
            
            if (pAdapter->Type != MIB_IF_TYPE_LOOPBACK && !vectorContainsString(addresses, macAddress)) {
                addresses.push_back(macAddress);
            }

            pAdapter = pAdapter->Next;
        }
    } else if (returnValue != ERROR_NO_DATA) {
        std::cerr << "ERROR nomber : " << returnValue << std::endl;
    }

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

std::string getHwrAddress(std::string fname) {
#if defined(_WIN32) || defined(_WIN64)
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

#if defined(_WIN64) || defined(_WIN32) 
void initWSAStartup () {
    static bool initialized = false;
    WORD wVersionRequested;
    WSADATA wsaData;

    if (initialized) return ;

    wVersionRequested = MAKEWORD(2, 2);

    if (WSAStartup(wVersionRequested, &wsaData) == 0) initialized = true;
}
#endif

bool vectorContainsString (std::vector<std::string> v, std::string str) {
    int size = v.size();
    
    for (int i = 0; i < size ; i++) {
        if (str.compare(v[i]) == 0) return true;
    }

    return false;
}

nlohmann::json getRmqConfig() {
    nlohmann::json config;

    config["host"] = RMQ_HOST;
    config["user"] = RMQ_USER;
    config["vhost"] = RMQ_VHOST;
    config["password"] = RMQ_PASSWORD;

    return config;
}
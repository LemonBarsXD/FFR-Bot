#include <Windows.h>
#include <iostream>

#define WIN32_LEAN_AND_MEAN

int main() {
    HDC dng = GetDC(NULL);

    while (true) {
        Sleep(50);

        if (GetAsyncKeyState(VK_LSHIFT) & 0x8000) {
            POINT cursorPos;
            GetCursorPos(&cursorPos);

            COLORREF c = GetPixel(dng, cursorPos.x, cursorPos.y);

            int red = GetRValue(c);
            int green = GetGValue(c);
            int blue = GetBValue(c);

            std::cout << "Mouse Position: (" << cursorPos.x << ", " << cursorPos.y << ")"
                      << " | RGB: (" << red << ", " << green << ", " << blue << ")\n";
        }

        if (GetAsyncKeyState(VK_DELETE) & 0x8000) {
            break;
        }
    }

    ReleaseDC(NULL, dng);

    return EXIT_SUCCESS;
}
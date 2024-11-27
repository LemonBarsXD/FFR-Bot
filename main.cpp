/*
Author: LemonBarsXD
Ver: 1.0.0

Setup:
    Window Size: 1048x645
    Window Position: 200x200
    Skin: Velocity (all notes white)
*/


#include <Windows.h>
#include <iostream>
#include <array>
#include <thread>
#include <mutex>
#include <vector>

#define WIN32_LEAN_AND_MEAN

std::mutex consoleMutex; 

bool IsWithinMargin(int value, int target, int margin) {
    return value >= (target - margin) && value <= (target + margin);
}

void SimulateKeyPress(WORD key) {
    INPUT input{};
    input.type = INPUT_KEYBOARD;
    input.ki.wVk = key;

    SendInput(1, &input, sizeof(INPUT));
    input.ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(1, &input, sizeof(INPUT));
}

void DetectAndPress(int index, int x_position, int y_position, int target, int margin, WORD key) {
    HDC dng = GetDC(NULL);
    bool pressed = false;

    while (true) {
        Sleep(1); 

        COLORREF color = GetPixel(dng, x_position, y_position);
        int green = GetGValue(color);

        if (IsWithinMargin(green, target, margin)) {
            if (!pressed) {
                pressed = true; // mark as pressed
                {
                    std::lock_guard<std::mutex> lock(consoleMutex);
                    char buffer[20] = "Note detected ";

                    buffer[14] = index + 1 + '0';     // convert int to char and append
                    buffer[14 + 1] = '\n';
                    buffer[14 + 2] = '\0';        // null-terminate

                    fwrite(buffer, 1, strlen(buffer), stdout);
                }
                Sleep(5); // compensating for early clicks
                SimulateKeyPress(key);
            }
        } else {
            pressed = false;
        }

        if (GetAsyncKeyState(VK_DELETE) & 0x8000) {
            break;
        }
    }

    ReleaseDC(NULL, dng);
}

int main() {
    const std::array<int, 4> x_positions{570, 680, 780, 900};
    const int y_position = 715;

    const int target = 215;
    const int margin = 40;

    const std::array<WORD, 4> keys{0x41, 0x53, 0x4B, 0x4C};

    std::vector<std::thread> threads;

    for (size_t i = 0; i < x_positions.size(); ++i) {
        threads.emplace_back(DetectAndPress, i, x_positions[i], y_position, target, margin, keys[i]);
    }

    while (true) {
        if (GetAsyncKeyState(VK_DELETE) & 0x8000) {
            break;
        }
        Sleep(100);
    }

    // join threads before exiting
    for (auto& thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }

    return EXIT_SUCCESS;
}
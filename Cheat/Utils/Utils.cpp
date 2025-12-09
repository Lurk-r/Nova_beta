#include "Utils.hpp"
#include <Windows.h>

namespace Utils
{
    bool keyPressed(int vKey)
    {
        return (GetAsyncKeyState(vKey) & 1) != 0;
    }

    bool isGameWindowActive(HWND hwndGame)
    {
        HWND hwndForeground = GetForegroundWindow();

        if (hwndForeground != hwndGame)
            return false;

        if (IsIconic(hwndGame))
            return false;

        return true;
    }
}
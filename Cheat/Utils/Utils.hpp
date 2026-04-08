#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#define OPEN_MENU_KEYS { VK_RCONTROL, VK_INSERT, VK_F1, VK_RSHIFT }

namespace Utils
{
    bool keyPressed(int vKey);
    bool isGameWindowActive(HWND hwndGame);
}
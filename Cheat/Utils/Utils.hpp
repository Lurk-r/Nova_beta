#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#define OPEN_MENU_KEY VK_RCONTROL

namespace Utils
{
    bool keyPressed(int vKey);
    bool isGameWindowActive(HWND hwndGame);
}
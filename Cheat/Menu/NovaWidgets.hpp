#pragma once

#include "Font/Byte.hpp"
#include "Font/FontAwesome.hpp"
#include <Obfusheader.hpp>
#include <string>
#include <vector>
#include <array>
#include <map>
#include <Windows.h>
#include <winreg.h>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <chrono>
#include <random>
#include <sstream>
#include <iomanip>
#include <d3d11.h>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#define STBI_NO_FAILURE_STRINGS
#include <stb_image.h>

#include "../Game/Data/AdderLists.hpp"
#include "../Cheat.hpp"

#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include <ImGui/imgui.hpp>
#include <ImGui/imgui_internal.hpp>

namespace stdvoid
{
    inline std::string username = "Leminare";
    inline std::string subscriptionType = "Lifetime";
    inline std::string subscriptionExpiry = "6/06/2026";
}

namespace ImGui
{
    extern ImVec4 ThemeColor;
}

namespace NovaUI
{
    inline ImFont* FontNormal = nullptr;
    inline ImFont* FontBold = nullptr;

    inline ID3D11ShaderResourceView* UserTexture = nullptr;

    struct ThemeImageEntry
    {
        std::string name;
        ImVec4 color;
        unsigned char* data;
        size_t size;
        ID3D11ShaderResourceView* texture;
        int width;
        int height;
    };

    inline std::vector<ThemeImageEntry> ThemeImages;
    inline ID3D11ShaderResourceView* CurrentSidebarTexture = nullptr;
    inline int CurrentSidebarWidth = 0;
    inline int CurrentSidebarHeight = 0;

    inline const float BaseFontSize = 14.0f;
    inline bool IsDragging = false;
    inline ImVec2 DragOffset = ImVec2(0, 0);
    inline ImVec2 WindowPos = ImVec2(-1, -1);
    inline const int DPIOptions[] = { 75, 100, 110, 120, 130, 150 };
    inline const char* DPILabels[] = { "75%", "100%", "110%", "120%", "130%", "150%" };
    inline int DPIIndex = 1;

    inline float Scale()
    {
        return DPIOptions[DPIIndex] / 100.0f;
    }

    inline float S(float v)
    {
        return v * Scale();
    }

    inline ImVec2 S(ImVec2 v)
    {
        return ImVec2(v.x * Scale(), v.y * Scale());
    }

    inline float FontSize()
    {
        return BaseFontSize * Scale();
    }

    inline ImVec4 ThemeColor = ImVec4(1.0f, 0.30f, 0.50f, 1.0f);
    inline int CurrentThemeIndex = 0;
    inline int ActiveTab = 0;
    inline int ActiveSubTab = 0;
    inline float CurrentPanelContentWidth = 0.0f;
    inline std::map<ImGuiID, float> Anim;
    inline float GlowTime = 0.0f;
    inline float MenuAlpha = 1.0f;
    inline float CurrentAlpha = 1.0f;

    struct UITheme
    {
        ImU32 Bg, Panel, PanelHeader, Sidebar, TopBar, Text, TextDim, TextMuted, Separator, ScrollGrab, Logo;
        ImU32 FrameBg, FrameBorder, PopupBg, ButtonBg, ButtonBorder;
    };

    inline UITheme Themes[] = {

        { IM_COL32(16, 16, 22, 255), IM_COL32(22, 22, 30, 255), IM_COL32(28, 28, 38, 255), IM_COL32(20, 20, 28, 255), IM_COL32(20, 20, 28, 255), IM_COL32(230, 230, 240, 255), IM_COL32(100, 100, 115, 255), IM_COL32(65, 65, 80, 255), IM_COL32(40, 40, 52, 255), IM_COL32(70, 70, 88, 255), IM_COL32(255, 255, 255, 255), IM_COL32(25, 25, 35, 255), IM_COL32(40, 40, 52, 255), IM_COL32(22, 22, 30, 255), IM_COL32(35, 35, 45, 255), IM_COL32(55, 55, 65, 255) },
        { IM_COL32(235, 230, 240, 250), IM_COL32(255, 255, 255, 255), IM_COL32(248, 248, 252, 255), IM_COL32(250, 248, 255, 255), IM_COL32(250, 248, 255, 255), IM_COL32(50, 50, 60, 255), IM_COL32(140, 140, 150, 255), IM_COL32(160, 160, 170, 255), IM_COL32(220, 220, 230, 255), IM_COL32(180, 180, 195, 255), IM_COL32(30, 30, 40, 255), IM_COL32(240, 240, 245, 255), IM_COL32(210, 210, 220, 255), IM_COL32(255, 255, 255, 255), IM_COL32(230, 230, 235, 255), IM_COL32(200, 200, 210, 255) },
        { IM_COL32(10, 10, 10, 255), IM_COL32(18, 18, 18, 255), IM_COL32(24, 24, 24, 255), IM_COL32(14, 14, 14, 255), IM_COL32(14, 14, 14, 255), IM_COL32(240, 240, 240, 255), IM_COL32(120, 120, 120, 255), IM_COL32(80, 80, 80, 255), IM_COL32(40, 40, 40, 255), IM_COL32(60, 60, 60, 255), IM_COL32(255, 255, 255, 255), IM_COL32(24, 24, 24, 255), IM_COL32(40, 40, 40, 255), IM_COL32(18, 18, 18, 255), IM_COL32(30, 30, 30, 255), IM_COL32(50, 50, 50, 255) },
        { IM_COL32(11, 15, 25, 255), IM_COL32(18, 24, 38, 255), IM_COL32(24, 32, 48, 255), IM_COL32(15, 20, 32, 255), IM_COL32(15, 20, 32, 255), IM_COL32(220, 230, 255, 255), IM_COL32(110, 120, 150, 255), IM_COL32(80, 90, 120, 255), IM_COL32(35, 45, 70, 255), IM_COL32(50, 70, 100, 255), IM_COL32(255, 255, 255, 255), IM_COL32(24, 32, 48, 255), IM_COL32(40, 50, 80, 255), IM_COL32(18, 24, 38, 255), IM_COL32(30, 40, 60, 255), IM_COL32(50, 70, 100, 255) },
        { IM_COL32(40, 42, 54, 255), IM_COL32(68, 71, 90, 255), IM_COL32(98, 114, 164, 255), IM_COL32(48, 50, 65, 255), IM_COL32(48, 50, 65, 255), IM_COL32(248, 248, 242, 255), IM_COL32(180, 180, 190, 255), IM_COL32(130, 130, 140, 255), IM_COL32(98, 114, 164, 255), IM_COL32(139, 233, 253, 255), IM_COL32(255, 255, 255, 255), IM_COL32(68, 71, 90, 255), IM_COL32(98, 114, 164, 255), IM_COL32(40, 42, 54, 255), IM_COL32(80, 85, 110, 255), IM_COL32(100, 105, 130, 255) },
        { IM_COL32(46, 52, 64, 255), IM_COL32(59, 66, 82, 255), IM_COL32(67, 76, 94, 255), IM_COL32(53, 60, 75, 255), IM_COL32(53, 60, 75, 255), IM_COL32(236, 239, 244, 255), IM_COL32(180, 185, 190, 255), IM_COL32(140, 145, 150, 255), IM_COL32(76, 86, 106, 255), IM_COL32(136, 192, 208, 255), IM_COL32(255, 255, 255, 255), IM_COL32(59, 66, 82, 255), IM_COL32(76, 86, 106, 255), IM_COL32(46, 52, 64, 255), IM_COL32(67, 76, 94, 255), IM_COL32(85, 95, 115, 255) },
        { IM_COL32(0, 43, 54, 255), IM_COL32(7, 54, 66, 255), IM_COL32(15, 62, 74, 255), IM_COL32(3, 48, 60, 255), IM_COL32(3, 48, 60, 255), IM_COL32(131, 148, 150, 255), IM_COL32(100, 115, 120, 255), IM_COL32(80, 95, 100, 255), IM_COL32(25, 70, 80, 255), IM_COL32(42, 161, 152, 255), IM_COL32(255, 255, 255, 255), IM_COL32(7, 54, 66, 255), IM_COL32(25, 70, 80, 255), IM_COL32(0, 43, 54, 255), IM_COL32(15, 65, 80, 255), IM_COL32(30, 85, 100, 255) },
        { IM_COL32(20, 25, 20, 255), IM_COL32(30, 38, 30, 255), IM_COL32(38, 48, 38, 255), IM_COL32(25, 32, 25, 255), IM_COL32(25, 32, 25, 255), IM_COL32(220, 240, 220, 255), IM_COL32(120, 140, 120, 255), IM_COL32(80, 100, 80, 255), IM_COL32(45, 60, 45, 255), IM_COL32(70, 90, 70, 255), IM_COL32(255, 255, 255, 255), IM_COL32(30, 38, 30, 255), IM_COL32(45, 60, 45, 255), IM_COL32(20, 25, 20, 255), IM_COL32(40, 50, 40, 255), IM_COL32(60, 80, 60, 255) },
        { IM_COL32(25, 15, 15, 255), IM_COL32(38, 22, 22, 255), IM_COL32(48, 28, 28, 255), IM_COL32(32, 18, 18, 255), IM_COL32(32, 18, 18, 255), IM_COL32(255, 230, 230, 255), IM_COL32(180, 140, 140, 255), IM_COL32(140, 100, 100, 255), IM_COL32(60, 35, 35, 255), IM_COL32(90, 50, 50, 255), IM_COL32(255, 255, 255, 255), IM_COL32(38, 22, 22, 255), IM_COL32(60, 35, 35, 255), IM_COL32(25, 15, 15, 255), IM_COL32(50, 30, 30, 255), IM_COL32(80, 40, 40, 255) },
        { IM_COL32(20, 15, 30, 255), IM_COL32(35, 25, 50, 255), IM_COL32(45, 35, 65, 255), IM_COL32(28, 20, 40, 255), IM_COL32(28, 20, 40, 255), IM_COL32(240, 230, 255, 255), IM_COL32(150, 130, 180, 255), IM_COL32(110, 90, 140, 255), IM_COL32(60, 45, 80, 255), IM_COL32(90, 70, 120, 255), IM_COL32(255, 255, 255, 255), IM_COL32(35, 25, 50, 255), IM_COL32(60, 45, 80, 255), IM_COL32(20, 15, 30, 255), IM_COL32(45, 35, 65, 255), IM_COL32(75, 60, 100, 255) },
        { IM_COL32(240, 248, 255, 255), IM_COL32(255, 255, 255, 255), IM_COL32(245, 252, 255, 255), IM_COL32(245, 250, 255, 255), IM_COL32(245, 250, 255, 255), IM_COL32(40, 60, 80, 255), IM_COL32(100, 120, 140, 255), IM_COL32(130, 150, 170, 255), IM_COL32(200, 215, 230, 255), IM_COL32(160, 190, 220, 255), IM_COL32(30, 50, 70, 255), IM_COL32(240, 250, 255, 255), IM_COL32(200, 215, 230, 255), IM_COL32(255, 255, 255, 255), IM_COL32(220, 240, 255, 255), IM_COL32(190, 210, 230, 255) },
        { IM_COL32(250, 245, 235, 255), IM_COL32(255, 252, 245, 255), IM_COL32(252, 248, 242, 255), IM_COL32(252, 248, 240, 255), IM_COL32(252, 248, 240, 255), IM_COL32(60, 55, 50, 255), IM_COL32(140, 130, 120, 255), IM_COL32(170, 160, 150, 255), IM_COL32(230, 220, 210, 255), IM_COL32(200, 190, 180, 255), IM_COL32(40, 35, 30, 255), IM_COL32(252, 248, 240, 255), IM_COL32(230, 220, 210, 255), IM_COL32(255, 252, 245, 255), IM_COL32(240, 235, 225, 255), IM_COL32(210, 205, 195, 255) },
        { IM_COL32(5, 20, 30, 255), IM_COL32(10, 35, 50, 255), IM_COL32(15, 45, 65, 255), IM_COL32(8, 28, 40, 255), IM_COL32(8, 28, 40, 255), IM_COL32(200, 230, 255, 255), IM_COL32(100, 150, 180, 255), IM_COL32(60, 100, 130, 255), IM_COL32(30, 60, 80, 255), IM_COL32(40, 80, 110, 255), IM_COL32(255, 255, 255, 255), IM_COL32(15, 40, 60, 255), IM_COL32(30, 70, 100, 255), IM_COL32(10, 30, 50, 255), IM_COL32(20, 50, 80, 255), IM_COL32(40, 90, 120, 255) },
        { IM_COL32(39, 40, 34, 255), IM_COL32(62, 61, 50, 255), IM_COL32(73, 72, 62, 255), IM_COL32(49, 50, 44, 255), IM_COL32(49, 50, 44, 255), IM_COL32(248, 248, 242, 255), IM_COL32(165, 165, 155, 255), IM_COL32(117, 113, 94, 255), IM_COL32(73, 72, 62, 255), IM_COL32(102, 217, 239, 255), IM_COL32(255, 255, 255, 255), IM_COL32(62, 61, 50, 255), IM_COL32(117, 113, 94, 255), IM_COL32(39, 40, 34, 255), IM_COL32(80, 80, 70, 255), IM_COL32(100, 100, 90, 255) },
        { IM_COL32(40, 40, 40, 255), IM_COL32(50, 48, 47, 255), IM_COL32(60, 56, 54, 255), IM_COL32(45, 45, 45, 255), IM_COL32(45, 45, 45, 255), IM_COL32(235, 219, 178, 255), IM_COL32(168, 153, 132, 255), IM_COL32(146, 131, 116, 255), IM_COL32(80, 73, 69, 255), IM_COL32(214, 93, 14, 255), IM_COL32(255, 255, 255, 255), IM_COL32(50, 48, 47, 255), IM_COL32(102, 92, 84, 255), IM_COL32(40, 40, 40, 255), IM_COL32(60, 56, 54, 255), IM_COL32(80, 73, 69, 255) },

        { IM_COL32(28, 11, 43, 255), IM_COL32(45, 20, 70, 255), IM_COL32(60, 30, 90, 255), IM_COL32(35, 15, 55, 255), IM_COL32(35, 15, 55, 255), IM_COL32(255, 0, 110, 255), IM_COL32(180, 80, 180, 255), IM_COL32(140, 50, 140, 255), IM_COL32(80, 40, 100, 255), IM_COL32(0, 255, 230, 255), IM_COL32(255, 255, 255, 255), IM_COL32(45, 20, 70, 255), IM_COL32(80, 40, 100, 255), IM_COL32(28, 11, 43, 255), IM_COL32(60, 30, 90, 255), IM_COL32(100, 50, 150, 255) },

        { IM_COL32(8, 8, 15, 255), IM_COL32(15, 15, 30, 255), IM_COL32(25, 25, 50, 255), IM_COL32(10, 10, 22, 255), IM_COL32(10, 10, 22, 255), IM_COL32(250, 240, 50, 255), IM_COL32(100, 200, 255, 255), IM_COL32(60, 120, 180, 255), IM_COL32(30, 60, 100, 255), IM_COL32(255, 0, 90, 255), IM_COL32(255, 255, 255, 255), IM_COL32(15, 15, 30, 255), IM_COL32(50, 50, 90, 255), IM_COL32(8, 8, 15, 255), IM_COL32(25, 25, 50, 255), IM_COL32(60, 60, 120, 255) },

        { IM_COL32(235, 242, 235, 255), IM_COL32(255, 255, 255, 255), IM_COL32(245, 252, 245, 255), IM_COL32(240, 250, 240, 255), IM_COL32(255, 255, 255, 255), IM_COL32(80, 100, 80, 255), IM_COL32(130, 160, 130, 255), IM_COL32(160, 190, 160, 255), IM_COL32(200, 220, 200, 255), IM_COL32(100, 180, 100, 255), IM_COL32(40, 60, 40, 255), IM_COL32(250, 255, 250, 255), IM_COL32(210, 230, 210, 255), IM_COL32(255, 255, 255, 255), IM_COL32(240, 250, 240, 255), IM_COL32(200, 230, 200, 255) },

        { IM_COL32(255, 245, 247, 255), IM_COL32(255, 255, 255, 255), IM_COL32(255, 250, 252, 255), IM_COL32(255, 248, 250, 255), IM_COL32(255, 255, 255, 255), IM_COL32(150, 80, 100, 255), IM_COL32(220, 140, 160, 255), IM_COL32(240, 180, 200, 255), IM_COL32(255, 210, 220, 255), IM_COL32(255, 120, 160, 255), IM_COL32(80, 40, 50, 255), IM_COL32(255, 252, 254, 255), IM_COL32(250, 220, 230, 255), IM_COL32(255, 255, 255, 255), IM_COL32(255, 235, 240, 255), IM_COL32(255, 200, 220, 255) },

        { IM_COL32(0, 0, 0, 255), IM_COL32(8, 8, 8, 255), IM_COL32(15, 15, 15, 255), IM_COL32(5, 5, 5, 255), IM_COL32(5, 5, 5, 255), IM_COL32(255, 255, 255, 255), IM_COL32(150, 150, 150, 255), IM_COL32(100, 100, 100, 255), IM_COL32(50, 50, 50, 255), IM_COL32(255, 255, 255, 255), IM_COL32(255, 255, 255, 255), IM_COL32(8, 8, 8, 255), IM_COL32(50, 50, 50, 255), IM_COL32(0, 0, 0, 255), IM_COL32(20, 20, 20, 255), IM_COL32(60, 60, 60, 255) },

        { IM_COL32(18, 15, 12, 255), IM_COL32(30, 25, 20, 255), IM_COL32(45, 38, 30, 255), IM_COL32(25, 20, 16, 255), IM_COL32(25, 20, 16, 255), IM_COL32(240, 210, 140, 255), IM_COL32(180, 150, 100, 255), IM_COL32(140, 110, 70, 255), IM_COL32(90, 70, 45, 255), IM_COL32(218, 165, 32, 255), IM_COL32(255, 255, 255, 255), IM_COL32(30, 25, 20, 255), IM_COL32(80, 65, 45, 255), IM_COL32(18, 15, 12, 255), IM_COL32(45, 38, 30, 255), IM_COL32(100, 85, 60, 255) },

        { IM_COL32(230, 240, 250, 255), IM_COL32(255, 255, 255, 255), IM_COL32(240, 250, 255, 255), IM_COL32(245, 252, 255, 255), IM_COL32(255, 255, 255, 255), IM_COL32(50, 80, 120, 255), IM_COL32(100, 140, 180, 255), IM_COL32(140, 180, 220, 255), IM_COL32(180, 210, 240, 255), IM_COL32(80, 180, 240, 255), IM_COL32(30, 50, 80, 255), IM_COL32(250, 252, 255, 255), IM_COL32(200, 225, 245, 255), IM_COL32(255, 255, 255, 255), IM_COL32(235, 245, 255, 255), IM_COL32(190, 220, 245, 255) },

        { IM_COL32(30, 15, 25, 255), IM_COL32(50, 25, 35, 255), IM_COL32(70, 35, 50, 255), IM_COL32(40, 20, 30, 255), IM_COL32(40, 20, 30, 255), IM_COL32(255, 200, 150, 255), IM_COL32(200, 120, 100, 255), IM_COL32(160, 80, 80, 255), IM_COL32(100, 50, 60, 255), IM_COL32(255, 140, 50, 255), IM_COL32(255, 255, 255, 255), IM_COL32(50, 25, 35, 255), IM_COL32(100, 60, 80, 255), IM_COL32(30, 15, 25, 255), IM_COL32(70, 35, 50, 255), IM_COL32(120, 70, 90, 255) },

        { IM_COL32(20, 25, 24, 255), IM_COL32(30, 38, 36, 255), IM_COL32(40, 50, 48, 255), IM_COL32(25, 32, 30, 255), IM_COL32(25, 32, 30, 255), IM_COL32(150, 250, 200, 255), IM_COL32(100, 180, 150, 255), IM_COL32(70, 130, 110, 255), IM_COL32(40, 80, 70, 255), IM_COL32(60, 220, 160, 255), IM_COL32(255, 255, 255, 255), IM_COL32(30, 38, 36, 255), IM_COL32(60, 100, 90, 255), IM_COL32(20, 25, 24, 255), IM_COL32(40, 50, 48, 255), IM_COL32(80, 110, 100, 255) },

        { IM_COL32(25, 20, 30, 255), IM_COL32(40, 30, 50, 255), IM_COL32(60, 45, 75, 255), IM_COL32(35, 25, 45, 255), IM_COL32(35, 25, 45, 255), IM_COL32(220, 200, 255, 255), IM_COL32(160, 140, 200, 255), IM_COL32(120, 100, 160, 255), IM_COL32(80, 60, 100, 255), IM_COL32(180, 130, 240, 255), IM_COL32(255, 255, 255, 255), IM_COL32(40, 30, 50, 255), IM_COL32(90, 70, 120, 255), IM_COL32(25, 20, 30, 255), IM_COL32(60, 45, 75, 255), IM_COL32(110, 85, 140, 255) },

        { IM_COL32(30, 25, 22, 255), IM_COL32(45, 38, 34, 255), IM_COL32(60, 50, 45, 255), IM_COL32(40, 32, 28, 255), IM_COL32(40, 32, 28, 255), IM_COL32(230, 200, 170, 255), IM_COL32(170, 140, 110, 255), IM_COL32(130, 100, 80, 255), IM_COL32(80, 60, 50, 255), IM_COL32(210, 160, 110, 255), IM_COL32(255, 255, 255, 255), IM_COL32(45, 38, 34, 255), IM_COL32(90, 75, 65, 255), IM_COL32(30, 25, 22, 255), IM_COL32(60, 50, 45, 255), IM_COL32(110, 90, 80, 255) },

        { IM_COL32(30, 35, 40, 255), IM_COL32(45, 50, 55, 255), IM_COL32(60, 65, 70, 255), IM_COL32(40, 45, 50, 255), IM_COL32(40, 45, 50, 255), IM_COL32(220, 225, 230, 255), IM_COL32(140, 150, 160, 255), IM_COL32(100, 110, 120, 255), IM_COL32(70, 80, 90, 255), IM_COL32(100, 120, 140, 255), IM_COL32(255, 255, 255, 255), IM_COL32(45, 50, 55, 255), IM_COL32(80, 90, 100, 255), IM_COL32(30, 35, 40, 255), IM_COL32(60, 65, 70, 255), IM_COL32(90, 100, 110, 255) },

        { IM_COL32(25, 10, 10, 255), IM_COL32(45, 20, 20, 255), IM_COL32(65, 30, 30, 255), IM_COL32(35, 15, 15, 255), IM_COL32(35, 15, 15, 255), IM_COL32(255, 200, 200, 255), IM_COL32(200, 100, 100, 255), IM_COL32(150, 60, 60, 255), IM_COL32(90, 30, 30, 255), IM_COL32(230, 40, 60, 255), IM_COL32(255, 255, 255, 255), IM_COL32(45, 20, 20, 255), IM_COL32(100, 40, 40, 255), IM_COL32(25, 10, 10, 255), IM_COL32(65, 30, 30, 255), IM_COL32(120, 50, 50, 255) },

        { IM_COL32(10, 15, 30, 255), IM_COL32(20, 30, 55, 255), IM_COL32(30, 45, 80, 255), IM_COL32(15, 25, 45, 255), IM_COL32(15, 25, 45, 255), IM_COL32(200, 220, 255, 255), IM_COL32(100, 140, 220, 255), IM_COL32(60, 90, 160, 255), IM_COL32(40, 60, 100, 255), IM_COL32(50, 100, 240, 255), IM_COL32(255, 255, 255, 255), IM_COL32(20, 30, 55, 255), IM_COL32(50, 70, 120, 255), IM_COL32(10, 15, 30, 255), IM_COL32(30, 45, 80, 255), IM_COL32(60, 100, 180, 255) },

        { IM_COL32(5, 20, 10, 255), IM_COL32(10, 35, 20, 255), IM_COL32(15, 50, 30, 255), IM_COL32(8, 28, 15, 255), IM_COL32(8, 28, 15, 255), IM_COL32(180, 255, 200, 255), IM_COL32(100, 200, 140, 255), IM_COL32(60, 140, 90, 255), IM_COL32(30, 80, 50, 255), IM_COL32(40, 220, 120, 255), IM_COL32(255, 255, 255, 255), IM_COL32(10, 35, 20, 255), IM_COL32(40, 100, 60, 255), IM_COL32(5, 20, 10, 255), IM_COL32(15, 50, 30, 255), IM_COL32(40, 110, 70, 255) },

        { IM_COL32(10, 10, 10, 255), IM_COL32(20, 20, 20, 255), IM_COL32(30, 30, 30, 255), IM_COL32(15, 15, 15, 255), IM_COL32(15, 15, 15, 255), IM_COL32(100, 255, 50, 255), IM_COL32(60, 180, 40, 255), IM_COL32(40, 120, 30, 255), IM_COL32(20, 60, 15, 255), IM_COL32(57, 255, 20, 255), IM_COL32(255, 255, 255, 255), IM_COL32(20, 20, 20, 255), IM_COL32(60, 60, 60, 255), IM_COL32(10, 10, 10, 255), IM_COL32(30, 30, 30, 255), IM_COL32(50, 50, 50, 255) },

        { IM_COL32(255, 240, 245, 255), IM_COL32(255, 255, 255, 255), IM_COL32(255, 235, 240, 255), IM_COL32(255, 245, 250, 255), IM_COL32(255, 255, 255, 255), IM_COL32(255, 20, 147, 255), IM_COL32(200, 50, 120, 255), IM_COL32(160, 80, 110, 255), IM_COL32(255, 180, 200, 255), IM_COL32(255, 20, 147, 255), IM_COL32(50, 10, 30, 255), IM_COL32(255, 245, 250, 255), IM_COL32(255, 210, 225, 255), IM_COL32(255, 255, 255, 255), IM_COL32(255, 230, 240, 255), IM_COL32(255, 180, 200, 255) },

        { IM_COL32(40, 43, 48, 255), IM_COL32(54, 57, 63, 255), IM_COL32(66, 69, 73, 255), IM_COL32(47, 49, 54, 255), IM_COL32(32, 34, 37, 255), IM_COL32(220, 221, 222, 255), IM_COL32(185, 187, 190, 255), IM_COL32(114, 118, 125, 255), IM_COL32(54, 57, 63, 255), IM_COL32(114, 137, 218, 255), IM_COL32(255, 255, 255, 255), IM_COL32(54, 57, 63, 255), IM_COL32(114, 118, 125, 255), IM_COL32(32, 34, 37, 255), IM_COL32(66, 69, 73, 255), IM_COL32(114, 137, 218, 255) },

        { IM_COL32(18, 18, 18, 255), IM_COL32(24, 24, 24, 255), IM_COL32(40, 40, 40, 255), IM_COL32(30, 30, 30, 255), IM_COL32(0, 0, 0, 255), IM_COL32(255, 255, 255, 255), IM_COL32(179, 179, 179, 255), IM_COL32(120, 120, 120, 255), IM_COL32(64, 64, 64, 255), IM_COL32(29, 185, 84, 255), IM_COL32(255, 255, 255, 255), IM_COL32(24, 24, 24, 255), IM_COL32(83, 83, 83, 255), IM_COL32(18, 18, 18, 255), IM_COL32(64, 64, 64, 255), IM_COL32(29, 185, 84, 255) },

        { IM_COL32(255, 255, 255, 255), IM_COL32(245, 245, 245, 255), IM_COL32(230, 230, 230, 255), IM_COL32(249, 249, 249, 255), IM_COL32(255, 255, 255, 255), IM_COL32(15, 15, 15, 255), IM_COL32(96, 96, 96, 255), IM_COL32(144, 144, 144, 255), IM_COL32(220, 220, 220, 255), IM_COL32(255, 0, 0, 255), IM_COL32(255, 255, 255, 255), IM_COL32(245, 245, 245, 255), IM_COL32(204, 204, 204, 255), IM_COL32(255, 255, 255, 255), IM_COL32(230, 230, 230, 255), IM_COL32(200, 200, 200, 255) },

        { IM_COL32(0, 0, 0, 255), IM_COL32(0, 0, 0, 255), IM_COL32(20, 20, 20, 255), IM_COL32(10, 10, 10, 255), IM_COL32(0, 0, 0, 255), IM_COL32(255, 255, 255, 255), IM_COL32(150, 150, 150, 255), IM_COL32(80, 80, 80, 255), IM_COL32(40, 40, 40, 255), IM_COL32(255, 255, 255, 255), IM_COL32(255, 255, 255, 255), IM_COL32(10, 10, 10, 255), IM_COL32(60, 60, 60, 255), IM_COL32(0, 0, 0, 255), IM_COL32(30, 30, 30, 255), IM_COL32(80, 80, 80, 255) },

        { IM_COL32(10, 15, 10, 255), IM_COL32(0, 20, 0, 255), IM_COL32(0, 35, 0, 255), IM_COL32(0, 18, 0, 255), IM_COL32(0, 10, 0, 255), IM_COL32(0, 255, 65, 255), IM_COL32(0, 180, 50, 255), IM_COL32(0, 100, 30, 255), IM_COL32(0, 60, 20, 255), IM_COL32(0, 255, 65, 255), IM_COL32(0, 255, 65, 255), IM_COL32(0, 20, 0, 255), IM_COL32(0, 80, 20, 255), IM_COL32(10, 15, 10, 255), IM_COL32(0, 40, 10, 255), IM_COL32(0, 120, 40, 255) },

        { IM_COL32(255, 240, 245, 255), IM_COL32(240, 248, 255, 255), IM_COL32(255, 228, 225, 255), IM_COL32(230, 230, 250, 255), IM_COL32(255, 255, 255, 255), IM_COL32(100, 149, 237, 255), IM_COL32(255, 105, 180, 255), IM_COL32(219, 112, 147, 255), IM_COL32(176, 196, 222, 255), IM_COL32(255, 182, 193, 255), IM_COL32(50, 50, 80, 255), IM_COL32(240, 248, 255, 255), IM_COL32(180, 200, 230, 255), IM_COL32(255, 255, 255, 255), IM_COL32(255, 240, 245, 255), IM_COL32(255, 192, 203, 255) },

        { IM_COL32(25, 15, 5, 255), IM_COL32(40, 25, 10, 255), IM_COL32(60, 35, 15, 255), IM_COL32(35, 20, 8, 255), IM_COL32(35, 20, 8, 255), IM_COL32(255, 200, 100, 255), IM_COL32(200, 140, 50, 255), IM_COL32(160, 100, 40, 255), IM_COL32(100, 60, 20, 255), IM_COL32(255, 140, 0, 255), IM_COL32(255, 255, 255, 255), IM_COL32(40, 25, 10, 255), IM_COL32(120, 70, 20, 255), IM_COL32(25, 15, 5, 255), IM_COL32(60, 35, 15, 255), IM_COL32(150, 80, 20, 255) },

        { IM_COL32(10, 25, 30, 255), IM_COL32(20, 40, 45, 255), IM_COL32(30, 55, 60, 255), IM_COL32(15, 35, 40, 255), IM_COL32(15, 35, 40, 255), IM_COL32(127, 255, 212, 255), IM_COL32(64, 224, 208, 255), IM_COL32(40, 160, 150, 255), IM_COL32(30, 80, 80, 255), IM_COL32(0, 255, 255, 255), IM_COL32(255, 255, 255, 255), IM_COL32(20, 40, 45, 255), IM_COL32(50, 100, 110, 255), IM_COL32(10, 25, 30, 255), IM_COL32(30, 55, 60, 255), IM_COL32(60, 120, 130, 255) },

        { IM_COL32(20, 18, 24, 255), IM_COL32(32, 28, 40, 255), IM_COL32(45, 40, 55, 255), IM_COL32(28, 24, 35, 255), IM_COL32(28, 24, 35, 255), IM_COL32(230, 230, 250, 255), IM_COL32(180, 170, 210, 255), IM_COL32(130, 120, 160, 255), IM_COL32(80, 70, 100, 255), IM_COL32(147, 112, 219, 255), IM_COL32(255, 255, 255, 255), IM_COL32(32, 28, 40, 255), IM_COL32(70, 60, 90, 255), IM_COL32(20, 18, 24, 255), IM_COL32(45, 40, 55, 255), IM_COL32(90, 80, 120, 255) },

        { IM_COL32(255, 245, 240, 255), IM_COL32(255, 255, 255, 255), IM_COL32(255, 235, 230, 255), IM_COL32(255, 242, 238, 255), IM_COL32(255, 255, 255, 255), IM_COL32(255, 127, 80, 255), IM_COL32(205, 92, 92, 255), IM_COL32(180, 80, 80, 255), IM_COL32(255, 200, 190, 255), IM_COL32(255, 160, 122, 255), IM_COL32(60, 30, 20, 255), IM_COL32(255, 242, 238, 255), IM_COL32(255, 210, 200, 255), IM_COL32(255, 255, 255, 255), IM_COL32(255, 230, 225, 255), IM_COL32(255, 150, 130, 255) },

        { IM_COL32(35, 35, 40, 255), IM_COL32(50, 50, 55, 255), IM_COL32(70, 70, 75, 255), IM_COL32(45, 45, 50, 255), IM_COL32(45, 45, 50, 255), IM_COL32(220, 220, 230, 255), IM_COL32(160, 160, 170, 255), IM_COL32(120, 120, 130, 255), IM_COL32(80, 80, 90, 255), IM_COL32(176, 196, 222, 255), IM_COL32(255, 255, 255, 255), IM_COL32(50, 50, 55, 255), IM_COL32(90, 90, 100, 255), IM_COL32(35, 35, 40, 255), IM_COL32(70, 70, 75, 255), IM_COL32(110, 110, 120, 255) },

        { IM_COL32(15, 10, 25, 255), IM_COL32(25, 20, 40, 255), IM_COL32(40, 30, 60, 255), IM_COL32(20, 15, 35, 255), IM_COL32(20, 15, 35, 255), IM_COL32(200, 180, 240, 255), IM_COL32(140, 120, 180, 255), IM_COL32(100, 80, 140, 255), IM_COL32(60, 50, 90, 255), IM_COL32(100, 50, 200, 255), IM_COL32(255, 255, 255, 255), IM_COL32(25, 20, 40, 255), IM_COL32(70, 50, 100, 255), IM_COL32(15, 10, 25, 255), IM_COL32(40, 30, 60, 255), IM_COL32(80, 60, 120, 255) },

        { IM_COL32(10, 20, 30, 255), IM_COL32(20, 35, 50, 255), IM_COL32(30, 50, 70, 255), IM_COL32(15, 30, 45, 255), IM_COL32(15, 30, 45, 255), IM_COL32(200, 240, 255, 255), IM_COL32(120, 180, 220, 255), IM_COL32(80, 120, 160, 255), IM_COL32(40, 70, 100, 255), IM_COL32(100, 200, 255, 255), IM_COL32(255, 255, 255, 255), IM_COL32(20, 35, 50, 255), IM_COL32(50, 90, 120, 255), IM_COL32(10, 20, 30, 255), IM_COL32(30, 50, 70, 255), IM_COL32(70, 110, 150, 255) }
    };

    inline const char* ThemeNames[] = {
        "Dark", "Light", "Obsidian", "Midnight", "Dracula", "Nord", "Solarized", "Forest", "Crimson", "Royal",
        "Sky", "Cream", "Deep Ocean", "Monokai", "Gruvbox",

        "Synthwave", "Cyberpunk", "Matcha", "Cherry Blossom", "Void",
        "Luxury Gold", "Ice", "Sunset", "Mint Dark", "Lilac",
        "Coffee", "Slate", "Ruby", "Sapphire", "Emerald City",
        "Toxic", "Hot Pink", "Discord", "Spotify", "YouTube",
        "OLED Black", "Terminal", "Cotton Candy", "Pumpkin", "Aquamarine",
        "Lavender Dark", "Coral", "Steel", "Nightshade", "Glacier Dark"
    };

    inline ImFont* GetFont()
    {
        return FontNormal ? FontNormal : ImGui::GetFont();
    }

    inline ImFont* GetFontBold()
    {
        return FontBold ? FontBold : GetFont();
    }

    inline float GetAnim(ImGuiID id, bool active, float speed = 0.1f)
    {
        float& v = Anim[id];
        v += ((active ? 1.0f : 0.0f) - v) * speed;
        return v;
    }

    inline ImVec4 HSVtoRGB(float h, float s, float v)
    {
        float r, g, b;
        ImGui::ColorConvertHSVtoRGB(h, s, v, r, g, b);
        return ImVec4(r, g, b, 1.0f);
    }

    inline void RGBtoHSV(ImVec4 c, float& h, float& s, float& v)
    {
        ImGui::ColorConvertRGBtoHSV(c.x, c.y, c.z, h, s, v);
    }

    inline ImU32 A(ImU32 col)
    {
        return (col & 0x00FFFFFF) | ((int)((col >> 24) * CurrentAlpha) << 24);
    }

    inline ImU32 ColBg() { return A(Themes[CurrentThemeIndex].Bg); }
    inline ImU32 ColPanel() { return A(Themes[CurrentThemeIndex].Panel); }
    inline ImU32 ColPanelHeader() { return A(Themes[CurrentThemeIndex].PanelHeader); }
    inline ImU32 ColSidebar() { return A(Themes[CurrentThemeIndex].Sidebar); }
    inline ImU32 ColTopBar() { return A(Themes[CurrentThemeIndex].TopBar); }
    inline ImU32 ColText() { return A(Themes[CurrentThemeIndex].Text); }
    inline ImU32 ColTextDim() { return A(Themes[CurrentThemeIndex].TextDim); }
    inline ImU32 ColTextMuted() { return A(Themes[CurrentThemeIndex].TextMuted); }
    inline ImU32 ColSeparator() { return A(Themes[CurrentThemeIndex].Separator); }
    inline ImU32 ColScrollGrab() { return A(Themes[CurrentThemeIndex].ScrollGrab); }
    inline ImU32 ColLogo() { return A(Themes[CurrentThemeIndex].Logo); }
    inline ImU32 ColFrameBg() { return A(Themes[CurrentThemeIndex].FrameBg); }
    inline ImU32 ColFrameBorder() { return A(Themes[CurrentThemeIndex].FrameBorder); }
    inline ImU32 ColPopupBg() { return A(Themes[CurrentThemeIndex].PopupBg); }
    inline ImU32 ColButtonBg() { return A(Themes[CurrentThemeIndex].ButtonBg); }
    inline ImU32 ColButtonBorder() { return A(Themes[CurrentThemeIndex].ButtonBorder); }

    inline ImU32 ColTheme()
    {
        return ImGui::GetColorU32(ImVec4(ThemeColor.x, ThemeColor.y, ThemeColor.z, ThemeColor.w * CurrentAlpha));
    }

    inline ImU32 ColThemeLight()
    {
        return ImGui::GetColorU32(ImVec4(ThemeColor.x + (1.0f - ThemeColor.x) * 0.4f, ThemeColor.y + (1.0f - ThemeColor.y) * 0.4f, ThemeColor.z + (1.0f - ThemeColor.z) * 0.4f, 1.0f * CurrentAlpha));
    }

    inline const char* TabNames[] = { "Main", "Clans", "Gameplay", "Gifting", "Cloning", "Misc", "Settings" };
    inline const int TabCount = 7;
    inline const char* SubTabs[7][4] = {
        { "Basic", "Currency", "Items", "Other" },
        { "Members", "Wars", "Chat", "Settings" },
        { "Weapons", "Mods", "Testing", "Debug" },
        { "Send", "Receive", "History", "Settings" },
        { "Templates", "Active", "History", "Settings" },
        { "Tools", "Scripts", "Logs", "Other" },
        { "Appearance", "Scale", "About", "Info" }
    };

    struct ColorPreset
    {
        const char* name;
        ImVec4 color;
    };

    inline ColorPreset Presets[] = {
        { "Rose", ImVec4(1.0f, 0.30f, 0.50f, 1.0f) },
        { "Ocean", ImVec4(0.20f, 0.60f, 1.0f, 1.0f) },
        { "Emerald", ImVec4(0.20f, 0.85f, 0.50f, 1.0f) },
        { "Sunset", ImVec4(1.0f, 0.50f, 0.20f, 1.0f) },
        { "Violet", ImVec4(0.60f, 0.30f, 1.0f, 1.0f) },
        { "Cherry", ImVec4(1.0f, 0.25f, 0.30f, 1.0f) },
        { "Aqua", ImVec4(0.20f, 0.85f, 0.90f, 1.0f) },
        { "Gold", ImVec4(1.0f, 0.80f, 0.20f, 1.0f) }
    };
    inline const int PresetCount = 8;

    inline void LoadTextureFromBytes(ID3D11Device* device, const unsigned char* data, int len, ID3D11ShaderResourceView** outSrv, int* outW, int* outH)
    {
        if (*outSrv) return;
        int w, h, ch;
        unsigned char* image_data = stbi_load_from_memory(data, len, &w, &h, &ch, 4);
        if (!image_data) return;

        D3D11_TEXTURE2D_DESC desc;
        ZeroMemory(&desc, sizeof(desc));
        desc.Width = w;
        desc.Height = h;
        desc.MipLevels = 1;
        desc.ArraySize = 1;
        desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        desc.SampleDesc.Count = 1;
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        desc.CPUAccessFlags = 0;

        ID3D11Texture2D* pTexture = NULL;
        D3D11_SUBRESOURCE_DATA subResource;
        subResource.pSysMem = image_data;
        subResource.SysMemPitch = desc.Width * 4;
        subResource.SysMemSlicePitch = 0;
        device->CreateTexture2D(&desc, &subResource, &pTexture);

        if (pTexture)
        {
            D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
            ZeroMemory(&srvDesc, sizeof(srvDesc));
            srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
            srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
            srvDesc.Texture2D.MipLevels = desc.MipLevels;
            srvDesc.Texture2D.MostDetailedMip = 0;
            device->CreateShaderResourceView(pTexture, &srvDesc, outSrv);
            pTexture->Release();
            *outW = w;
            *outH = h;
        }
        stbi_image_free(image_data);
    }

    inline float GetColorDistance(ImVec4 c1, ImVec4 c2)
    {
        float r = c1.x - c2.x;
        float g = c1.y - c2.y;
        float b = c1.z - c2.z;
        return (r * r) + (g * g) + (b * b);
    }

    inline void UpdateClosestTexture()
    {
        if (ThemeImages.empty()) return;

        float minDist = FLT_MAX;
        int bestIndex = 0;

        for (int i = 0; i < ThemeImages.size(); i++)
        {
            float dist = GetColorDistance(ThemeColor, ThemeImages[i].color);
            if (dist < minDist)
            {
                minDist = dist;
                bestIndex = i;
            }
        }

        CurrentSidebarTexture = ThemeImages[bestIndex].texture;
        CurrentSidebarWidth = ThemeImages[bestIndex].width;
        CurrentSidebarHeight = ThemeImages[bestIndex].height;
    }

    inline void TopBar()
    {

    }

    inline bool Checkbox(const char* label, bool* v)
    {
        ImGuiWindow* window = ImGui::GetCurrentWindow();
        if (window->SkipItems) return false;
        ImGuiID id = window->GetID(label);
        ImVec2 pos = window->DC.CursorPos;
        float size = S(18.0f), scale = Scale(), fs = FontSize();
        ImVec2 labelSize = ImGui::CalcTextSize(label);
        ImRect checkBB(pos, ImVec2(pos.x + size, pos.y + size));
        ImRect totalBB(pos, ImVec2(pos.x + size + S(12.0f) + labelSize.x * scale, pos.y + ImMax(size, labelSize.y * scale)));

        ImGui::ItemSize(totalBB);
        if (!ImGui::ItemAdd(totalBB, id)) return false;

        bool hovered, held;
        if (ImGui::ButtonBehavior(checkBB, id, &hovered, &held)) *v = !*v;

        float anim = GetAnim(id, *v, 0.15f);

        window->DrawList->AddRectFilled(checkBB.Min, checkBB.Max, ColFrameBg(), S(4.0f));
        ImU32 borderColor = (*v || hovered) ? ColTheme() : ColFrameBorder();
        window->DrawList->AddRect(checkBB.Min, checkBB.Max, borderColor, S(4.0f), 0, S(1.5f));

        if (anim > 0.01f)
        {
            ImVec2 checkCenter = checkBB.GetCenter();
            float checkSize = fs * 0.8f * anim;
            window->DrawList->AddText(Fonts::FontAwesome, checkSize, ImVec2(checkCenter.x - checkSize * 0.5f + S(1.0f), checkCenter.y - checkSize * 0.5f + S(1.0f)), IM_COL32(0, 0, 0, (int)(200 * anim * CurrentAlpha)), ICON_FA_CHECK);
            window->DrawList->AddText(Fonts::FontAwesome, checkSize, ImVec2(checkCenter.x - checkSize * 0.5f, checkCenter.y - checkSize * 0.5f), IM_COL32(255, 255, 255, (int)(255 * anim * CurrentAlpha)), ICON_FA_CHECK);
        }

        window->DrawList->AddText(GetFont(), fs, ImVec2(checkBB.Max.x + S(12.0f), pos.y + (size - labelSize.y * scale) * 0.5f), ColText(), label);
        return *v;
    }

    inline bool Slider(const char* label, float* v, float vmin, float vmax)
    {
        ImGuiWindow* window = ImGui::GetCurrentWindow();
        if (window->SkipItems) return false;
        ImGuiID id = window->GetID(label);
        ImVec2 pos = window->DC.CursorPos;
        float width = CurrentPanelContentWidth > 0 ? CurrentPanelContentWidth : ImGui::GetContentRegionAvail().x;
        float trackH = S(4.0f), grabR = S(6.0f), totalH = grabR * 2.0f;
        float fs = FontSize();

        char valueText[32];
        snprintf(valueText, sizeof(valueText), "%.1f", *v);
        ImVec2 valueSize = GetFont()->CalcTextSizeA(fs, FLT_MAX, 0.0f, valueText);

        float rowH = fs;
        float sliderY = pos.y + rowH + S(4.0f);

        ImRect frameBB(pos, ImVec2(pos.x + width, sliderY + totalH));
        ImRect trackBB(
            ImVec2(pos.x + grabR, sliderY + (totalH - trackH) * 0.5f),
            ImVec2(pos.x + width - grabR, sliderY + (totalH + trackH) * 0.5f)
        );

        ImGui::ItemSize(frameBB);
        if (!ImGui::ItemAdd(frameBB, id)) return false;

        bool hovered, held;
        ImGui::ButtonBehavior(frameBB, id, &hovered, &held);
        if (held) *v = vmin + ImClamp((ImGui::GetIO().MousePos.x - trackBB.Min.x) / trackBB.GetWidth(), 0.0f, 1.0f) * (vmax - vmin);

        float t = (*v - vmin) / (vmax - vmin);

        window->DrawList->AddText(GetFont(), fs, ImVec2(pos.x, pos.y), ColText(), label);
        window->DrawList->AddText(GetFont(), fs, ImVec2(pos.x + width - valueSize.x, pos.y), ColTheme(), valueText);

        window->DrawList->AddRectFilled(trackBB.Min, trackBB.Max, ColSeparator(), trackH * 0.5f);
        window->DrawList->AddRectFilled(trackBB.Min, ImVec2(trackBB.Min.x + t * trackBB.GetWidth(), trackBB.Max.y), ColTheme(), trackH * 0.5f);
        window->DrawList->AddCircleFilled(ImVec2(trackBB.Min.x + t * trackBB.GetWidth(), trackBB.GetCenter().y), grabR, ColTheme());

        return held;
    }

    inline bool SliderInt(const char* label, int* v, int vmin, int vmax)
    {
        ImGuiWindow* window = ImGui::GetCurrentWindow();
        if (window->SkipItems) return false;
        ImGuiID id = window->GetID(label);
        ImVec2 pos = window->DC.CursorPos;
        float width = CurrentPanelContentWidth > 0 ? CurrentPanelContentWidth : ImGui::GetContentRegionAvail().x;
        float trackH = S(4.0f), grabR = S(6.0f), totalH = grabR * 2.0f;
        float fs = FontSize();

        char valueText[32];
        snprintf(valueText, sizeof(valueText), "%d", *v);
        ImVec2 valueSize = GetFont()->CalcTextSizeA(fs, FLT_MAX, 0.0f, valueText);
        ImVec2 labelSize = GetFont()->CalcTextSizeA(fs, FLT_MAX, 0.0f, label);

        float rowH = fs;
        float labelRowY = pos.y;
        float sliderY = labelRowY + rowH + S(4.0f);

        ImRect frameBB(pos, ImVec2(pos.x + width, sliderY + totalH));
        ImRect trackBB(
            ImVec2(pos.x + grabR, sliderY + (totalH - trackH) * 0.5f),
            ImVec2(pos.x + width - grabR, sliderY + (totalH + trackH) * 0.5f)
        );

        ImGui::ItemSize(frameBB);
        if (!ImGui::ItemAdd(frameBB, id)) return false;

        bool hovered, held;
        ImGui::ButtonBehavior(frameBB, id, &hovered, &held);

        if (held)
        {
            float t = ImClamp((ImGui::GetIO().MousePos.x - trackBB.Min.x) / trackBB.GetWidth(), 0.0f, 1.0f);
            *v = vmin + (int)(t * (vmax - vmin));
        }

        float t = (float)(*v - vmin) / (float)(vmax - vmin);

        window->DrawList->AddText(GetFont(), fs, ImVec2(pos.x, labelRowY), ColText(), label);
        window->DrawList->AddText(GetFont(), fs, ImVec2(pos.x + width - valueSize.x, labelRowY), ColTheme(), valueText);

        window->DrawList->AddRectFilled(trackBB.Min, trackBB.Max, ColSeparator(), trackH * 0.5f);
        window->DrawList->AddRectFilled(trackBB.Min, ImVec2(trackBB.Min.x + t * trackBB.GetWidth(), trackBB.Max.y), ColTheme(), trackH * 0.5f);
        window->DrawList->AddCircleFilled(ImVec2(trackBB.Min.x + t * trackBB.GetWidth(), trackBB.GetCenter().y), grabR, ColTheme());

        return held;
    }

    inline bool Combo(const char* label, int* current, const char* const items[], int count)
    {
        ImGuiWindow* window = ImGui::GetCurrentWindow();
        if (window->SkipItems) return false;

        ImGuiID id = window->GetID(label);
        float w = CurrentPanelContentWidth > 0 ? CurrentPanelContentWidth : ImGui::GetContentRegionAvail().x;
        float h = S(34.0f);
        float scale = Scale();
        float fs = FontSize();
        ImVec2 pos = window->DC.CursorPos;

        ImVec2 labelSize = ImGui::CalcTextSize(label);
        window->DrawList->AddText(GetFont(), fs, pos, ColText(), label);

        ImVec2 boxPos = ImVec2(pos.x, pos.y + labelSize.y + S(6.0f));
        ImRect bb(boxPos, ImVec2(boxPos.x + w, boxPos.y + h));

        ImGui::ItemSize(ImVec2(w, labelSize.y + S(6.0f) + h + S(6.0f)));
        if (!ImGui::ItemAdd(bb, id)) return false;

        bool hovered, held;
        bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held);
        bool popup_open = ImGui::IsPopupOpen(id, ImGuiPopupFlags_None);

        ImU32 bgCol = ColFrameBg();
        ImU32 borderCol = (hovered || popup_open) ? ColTheme() : ColFrameBorder();

        window->DrawList->AddRectFilled(bb.Min, bb.Max, bgCol, S(6.0f));
        if (hovered || popup_open)
            window->DrawList->AddRectFilled(bb.Min, bb.Max, ImGui::GetColorU32(ImVec4(ThemeColor.x, ThemeColor.y, ThemeColor.z, 0.05f * CurrentAlpha)), S(6.0f));

        window->DrawList->AddRect(bb.Min, bb.Max, borderCol, S(6.0f), 0, S(1.0f));

        const char* currentText = (current && *current >= 0 && *current < count) ? items[*current] : "";

        if (*current >= 0) {
            window->DrawList->AddRectFilled(ImVec2(bb.Min.x + S(2), bb.Min.y + S(6)), ImVec2(bb.Min.x + S(4), bb.Max.y - S(6)), ColTheme(), S(2.0f));
        }

        window->DrawList->AddText(GetFont(), fs, ImVec2(bb.Min.x + S(12.0f), bb.GetCenter().y - fs * 0.5f), ColText(), currentText);

        float arrowSize = S(4.0f);
        ImVec2 arrowCenter(bb.Max.x - S(16.0f), bb.GetCenter().y);
        window->DrawList->AddTriangleFilled(
            ImVec2(arrowCenter.x - arrowSize, arrowCenter.y - arrowSize * 0.5f),
            ImVec2(arrowCenter.x + arrowSize, arrowCenter.y - arrowSize * 0.5f),
            ImVec2(arrowCenter.x, arrowCenter.y + arrowSize * 0.5f),
            ColTextDim()
        );

        if (pressed) ImGui::OpenPopup(label);

        bool value_changed = false;
        ImGui::SetNextWindowPos(ImVec2(bb.Min.x, bb.Max.y + S(4.0f)));
        ImGui::SetNextWindowSize(ImVec2(w, -1));
        ImGui::SetNextWindowSizeConstraints(ImVec2(w, 0), ImVec2(w, S(250.0f)));

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, S(ImVec2(6, 6)));
        ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, S(6.0f));
        ImGui::PushStyleColor(ImGuiCol_PopupBg, ImGui::ColorConvertU32ToFloat4(ColPopupBg()));
        ImGui::PushStyleColor(ImGuiCol_ScrollbarGrab, ImGui::ColorConvertU32ToFloat4(ColScrollGrab()));

        if (ImGui::BeginPopup(label, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings))
        {
            ImDrawList* popupDl = ImGui::GetWindowDrawList();
            for (int i = 0; i < count; i++)
            {
                bool is_selected = (*current == i);
                ImGui::PushID(i);

                ImVec2 p = ImGui::GetCursorScreenPos();
                float itemH = S(28.0f);
                ImVec2 itemSize(ImGui::GetContentRegionAvail().x, itemH);
                bool itemHovered = ImGui::IsMouseHoveringRect(p, ImVec2(p.x + itemSize.x, p.y + itemSize.y));

                if (ImGui::InvisibleButton(items[i], itemSize))
                {
                    *current = i;
                    value_changed = true;
                    ImGui::CloseCurrentPopup();
                }

                if (is_selected)
                {
                    popupDl->AddRectFilled(p, ImVec2(p.x + itemSize.x, p.y + itemSize.y), ImGui::GetColorU32(ImVec4(ThemeColor.x, ThemeColor.y, ThemeColor.z, 0.15f)), S(4.0f));
                    popupDl->AddRectFilled(ImVec2(p.x + S(2), p.y + S(4)), ImVec2(p.x + S(4), p.y + itemH - S(4)), ColTheme(), S(2.0f));
                }
                else if (itemHovered)
                {
                    popupDl->AddRectFilled(p, ImVec2(p.x + itemSize.x, p.y + itemSize.y), ColButtonBg(), S(4.0f));
                }

                ImU32 textCol = is_selected ? ColTheme() : ColText();
                popupDl->AddText(GetFont(), fs, ImVec2(p.x + S(14.0f), p.y + (itemH - fs) * 0.5f), textCol, items[i]);

                ImGui::PopID();
            }
            ImGui::EndPopup();
        }
        ImGui::PopStyleColor(2);
        ImGui::PopStyleVar(2);

        return value_changed;
    }

    inline bool Button(const char* label, float width = 0.0f, float height = 0.0f)
    {
        ImGuiWindow* window = ImGui::GetCurrentWindow();
        if (window->SkipItems) return false;
        ImGuiID id = window->GetID(label);
        ImVec2 pos = window->DC.CursorPos;
        float w = width > 0 ? width : ImGui::GetContentRegionAvail().x, h = height > 0 ? height : S(28.0f), fs = FontSize();
        ImRect bb(pos, ImVec2(pos.x + w, pos.y + h));
        ImGui::ItemSize(bb);
        if (!ImGui::ItemAdd(bb, id)) return false;
        bool hovered, held;
        bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held);

        ImU32 bgCol = ColButtonBg();
        if (held) bgCol = ColTheme();
        else if (hovered) {
            bgCol = ColButtonBg();
            window->DrawList->AddRectFilled(bb.Min, bb.Max, ColText() & 0x11FFFFFF, S(6.0f));
        }
        else bgCol = ColButtonBg();

        window->DrawList->AddRectFilled(bb.Min, bb.Max, bgCol, S(6.0f));
        window->DrawList->AddRect(bb.Min, bb.Max, (hovered || held) ? ColTheme() : ColButtonBorder(), S(6.0f), 0, S(1.0f));

        ImVec2 labelSize = ImGui::CalcTextSize(label);
        window->DrawList->AddText(GetFont(), fs, ImVec2(bb.GetCenter().x - labelSize.x * 0.5f, bb.GetCenter().y - labelSize.y * 0.5f), held ? A(IM_COL32(255, 255, 255, 255)) : ColText(), label);
        return pressed;
    }

    inline bool IconButton(const char* icon, ImVec2 pos, float size)
    {
        ImGuiWindow* window = ImGui::GetCurrentWindow();
        if (window->SkipItems) return false;
        ImGuiID id = window->GetID(icon);
        ImRect bb(pos, ImVec2(pos.x + size, pos.y + size));
        if (!ImGui::ItemAdd(bb, id)) return false;

        bool hovered, held;
        bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held);
        float hoverAnim = GetAnim(id, hovered, 0.15f);

        ImU32 iconColor = held ? ColTheme() : (hovered ? ColTheme() : ColTextDim());
        if (hoverAnim > 0.01f)
        {
            window->DrawList->AddCircleFilled(bb.GetCenter(), size * 0.6f, ImGui::GetColorU32(ImVec4(ThemeColor.x, ThemeColor.y, ThemeColor.z, 0.15f * hoverAnim * CurrentAlpha)));
        }

        ImVec2 iconSize = Fonts::FontAwesome->CalcTextSizeA(FontSize() * 1.2f, FLT_MAX, 0.0f, icon);
        window->DrawList->AddText(Fonts::FontAwesome, FontSize() * 1.2f, ImVec2(bb.GetCenter().x - iconSize.x * 0.5f, bb.GetCenter().y - iconSize.y * 0.5f), iconColor, icon);

        return pressed;
    }

    inline bool InputText(const char* label, char* buf, size_t bufSize)
    {
        ImGui::PushID(label);
        float w = CurrentPanelContentWidth > 0 ? CurrentPanelContentWidth : ImGui::GetContentRegionAvail().x;
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, S(4.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, S(ImVec2(10, 6)));
        ImGui::PushStyleColor(ImGuiCol_FrameBg, ImGui::ColorConvertU32ToFloat4(ColFrameBg()));
        ImGui::PushStyleColor(ImGuiCol_Text, ImGui::ColorConvertU32ToFloat4(ColText()));
        ImGui::SetNextItemWidth(w);
        bool ret = ImGui::InputText("##input", buf, bufSize);
        ImGui::PopStyleColor(2);
        ImGui::PopStyleVar(2);
        ImGui::PopID();
        return ret;
    }

    inline bool InputInt(const char* label, int* v)
    {
        ImGui::PushID(label);
        float w = CurrentPanelContentWidth > 0 ? CurrentPanelContentWidth : ImGui::GetContentRegionAvail().x;
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, S(4.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, S(ImVec2(10, 6)));
        ImGui::PushStyleColor(ImGuiCol_FrameBg, ImGui::ColorConvertU32ToFloat4(ColFrameBg()));
        ImGui::PushStyleColor(ImGuiCol_Text, ImGui::ColorConvertU32ToFloat4(ColText()));
        ImGui::SetNextItemWidth(w);
        bool ret = ImGui::InputInt("##input", v, 0, 0);
        ImGui::PopStyleColor(2);
        ImGui::PopStyleVar(2);
        ImGui::PopID();
        return ret;
    }

    inline void ColorEdit(const char* label, ImVec4* col)
    {
        ImGuiWindow* window = ImGui::GetCurrentWindow();
        if (window->SkipItems) return;
        ImGui::PushID(label);
        float scale = Scale(), fs = FontSize();
        ImVec2 pos = window->DC.CursorPos;
        float totalW = CurrentPanelContentWidth > 0 ? CurrentPanelContentWidth : ImGui::GetContentRegionAvail().x, squareSize = S(20.0f), rowH = S(24.0f);
        ImVec2 labelSize = ImGui::CalcTextSize(label);
        window->DrawList->AddText(GetFont(), fs, ImVec2(pos.x, pos.y + (rowH - labelSize.y * scale) * 0.5f), ColText(), label);
        ImVec2 squarePos(pos.x + totalW - squareSize, pos.y + (rowH - squareSize) * 0.5f);
        ImRect bb(squarePos, ImVec2(squarePos.x + squareSize, squarePos.y + squareSize));
        window->DrawList->AddRectFilled(bb.Min, bb.Max, ImGui::GetColorU32(ImVec4(col->x, col->y, col->z, col->w * CurrentAlpha)), S(4.0f));
        window->DrawList->AddRect(bb.Min, bb.Max, ColButtonBorder(), S(4.0f), 0, S(1.0f));
        if (ImGui::IsMouseHoveringRect(bb.Min, bb.Max) && ImGui::IsMouseClicked(0)) ImGui::OpenPopup("##colorpicker");
        ImGui::ItemSize(ImVec2(totalW, rowH));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, S(ImVec2(16, 16)));
        ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, S(10.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, S(ImVec2(10, 10)));
        ImGui::PushStyleColor(ImGuiCol_PopupBg, ImGui::ColorConvertU32ToFloat4(ColPopupBg()));
        if (ImGui::BeginPopup("##colorpicker"))
        {
            ImDrawList* dl = ImGui::GetWindowDrawList();
            dl->AddText(GetFont(), fs, ImGui::GetCursorScreenPos(), ColText(), "Theme Color Picker");
            ImGui::Dummy(ImVec2(0, fs + S(4.0f)));
            ImGui::Separator();
            ImGui::Spacing();

            float hue, sat, val;
            RGBtoHSV(*col, hue, sat, val);

            static float dragHue = -1.0f;
            if (ImGui::IsMouseDown(0) && dragHue >= 0.0f) hue = dragHue;
            else dragHue = -1.0f;

            float svSize = S(180.0f), hueBarW = S(20.0f), hueBarGap = S(12.0f);
            ImVec2 svPos = ImGui::GetCursorScreenPos();

            ImGui::InvisibleButton("##sv_area", ImVec2(svSize, svSize));
            if (ImGui::IsItemActive())
            {
                sat = ImClamp((ImGui::GetIO().MousePos.x - svPos.x) / svSize, 0.0f, 1.0f);
                val = 1.0f - ImClamp((ImGui::GetIO().MousePos.y - svPos.y) / svSize, 0.0f, 1.0f);
                *col = HSVtoRGB(hue, sat, val);
            }

            ImVec4 hueCol = HSVtoRGB(hue, 1.0f, 1.0f);
            dl->AddRectFilledMultiColor(svPos, ImVec2(svPos.x + svSize, svPos.y + svSize), IM_COL32(255, 255, 255, 255), ImGui::GetColorU32(hueCol), ImGui::GetColorU32(hueCol), IM_COL32(255, 255, 255, 255));
            dl->AddRectFilledMultiColor(svPos, ImVec2(svPos.x + svSize, svPos.y + svSize), IM_COL32(0, 0, 0, 0), IM_COL32(0, 0, 0, 0), IM_COL32(0, 0, 0, 255), IM_COL32(0, 0, 0, 255));
            dl->AddRect(svPos, ImVec2(svPos.x + svSize, svPos.y + svSize), IM_COL32(255, 255, 255, 30), S(4.0f));
            float svCursorX = svPos.x + sat * svSize, svCursorY = svPos.y + (1.0f - val) * svSize;
            dl->AddCircle(ImVec2(svCursorX, svCursorY), S(6.0f), IM_COL32(255, 255, 255, 255), 0, S(2.0f));
            dl->AddCircle(ImVec2(svCursorX, svCursorY), S(5.0f), IM_COL32(0, 0, 0, 150), 0, S(1.0f));

            ImVec2 huePos(svPos.x + svSize + hueBarGap, svPos.y);
            ImGui::SetCursorScreenPos(huePos);
            ImGui::InvisibleButton("##hue_bar", ImVec2(hueBarW, svSize));
            if (ImGui::IsItemActive())
            {
                hue = ImClamp((ImGui::GetIO().MousePos.y - huePos.y) / svSize, 0.0f, 0.999f);
                dragHue = hue;
                *col = HSVtoRGB(hue, sat, val);
            }

            for (int i = 0; i < 6; i++)
            {
                float y1 = huePos.y + (i / 6.0f) * svSize, y2 = huePos.y + ((i + 1) / 6.0f) * svSize;
                ImVec4 c1 = HSVtoRGB(i / 6.0f, 1.0f, 1.0f), c2 = HSVtoRGB((i + 1) / 6.0f, 1.0f, 1.0f);
                dl->AddRectFilledMultiColor(ImVec2(huePos.x, y1), ImVec2(huePos.x + hueBarW, y2), ImGui::GetColorU32(c1), ImGui::GetColorU32(c1), ImGui::GetColorU32(c2), ImGui::GetColorU32(c2));
            }
            dl->AddRect(huePos, ImVec2(huePos.x + hueBarW, huePos.y + svSize), IM_COL32(255, 255, 255, 30), S(4.0f));
            float hueCursorY = huePos.y + hue * svSize;
            dl->AddRectFilled(ImVec2(huePos.x - S(2), hueCursorY - S(3)), ImVec2(huePos.x + hueBarW + S(2), hueCursorY + S(3)), IM_COL32(255, 255, 255, 255), S(2.0f));

            ImGui::Dummy(ImVec2(svSize + hueBarGap + hueBarW, 0.0f));
            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();
            dl->AddText(GetFont(), fs * 0.9f, ImGui::GetCursorScreenPos(), ColTextDim(), "Presets");
            ImGui::Dummy(ImVec2(0, fs * 0.9f + S(4.0f)));
            float presetSize = S(24.0f), spacing = S(6.0f);
            for (int i = 0; i < PresetCount; i++)
            {
                if (i > 0 && i % 4 != 0) ImGui::SameLine(0, spacing);
                ImGui::PushID(i);
                ImVec2 pPos = ImGui::GetCursorScreenPos();
                if (ImGui::IsMouseHoveringRect(pPos, ImVec2(pPos.x + presetSize, pPos.y + presetSize)) && ImGui::IsMouseClicked(0)) *col = Presets[i].color;
                ImGui::GetWindowDrawList()->AddRectFilled(pPos, ImVec2(pPos.x + presetSize, pPos.y + presetSize), ImGui::GetColorU32(Presets[i].color), S(4.0f));
                ImGui::Dummy(ImVec2(presetSize, presetSize));
                ImGui::PopID();
            }
            ImGui::EndPopup();
        }
        ImGui::PopStyleColor();
        ImGui::PopStyleVar(3);
        ImGui::PopID();
    }

    inline bool CategoryButton(int index, const char* label, bool selected, float w, float h, bool isLast)
    {
        ImGuiWindow* window = ImGui::GetCurrentWindow();
        if (window->SkipItems) return false;
        char fullId[64];
        snprintf(fullId, 64, "%s##cat%d", label, index);
        ImGuiID id = window->GetID(fullId);
        ImVec2 pos = window->DC.CursorPos;
        float scale = Scale(), fs = FontSize();
        ImRect bb(pos, ImVec2(pos.x + w, pos.y + h));
        ImGui::ItemSize(bb);
        if (!ImGui::ItemAdd(bb, id)) return false;
        bool hovered, held;
        bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held);
        float selectAnim = GetAnim(id, selected, 0.15f);
        float hoverAnim = GetAnim(id + 10000, hovered && !selected, 0.25f);
        ImDrawList* dl = window->DrawList;
        if (selected) dl->AddRectFilled(ImVec2(bb.Min.x + S(2), bb.Min.y + S(2)), ImVec2(bb.Max.x - S(2), bb.Max.y - S(2)), ImGui::GetColorU32(ImVec4(ThemeColor.x, ThemeColor.y, ThemeColor.z, 0.08f * CurrentAlpha)), S(4.0f));
        else if (hoverAnim > 0.01f) dl->AddRectFilled(ImVec2(bb.Min.x + S(4), bb.Min.y + S(3)), ImVec2(bb.Max.x - S(4), bb.Max.y - S(3)), IM_COL32(255, 255, 255, (int)(12 * hoverAnim * CurrentAlpha)), S(3.0f));
        if (selectAnim > 0.01f)
        {
            float barH = h * 0.45f * selectAnim, barY = bb.GetCenter().y - barH * 0.5f;
            dl->AddRectFilled(ImVec2(bb.Min.x + S(2), barY), ImVec2(bb.Min.x + S(4), barY + barH), ColTheme(), S(2.0f));
        }
        ImVec2 labelSize = ImGui::CalcTextSize(label);
        dl->AddText(GetFont(), fs, ImVec2(bb.Min.x + S(12), bb.GetCenter().y - (labelSize.y * scale) * 0.5f), selected ? ColText() : ColTextDim(), label);
        if (!isLast) dl->AddLine(ImVec2(bb.Min.x + S(8), bb.Max.y), ImVec2(bb.Max.x - S(8), bb.Max.y), ColSeparator(), 1.0f);
        if (pressed)
        {
            ActiveTab = index;
            ActiveSubTab = 0;
        }
        return pressed;
    }

    inline void BeginChildPanel(const char* id, const char* icon, const char* title, ImVec2 pos, ImVec2 size)
    {
        ImDrawList* dl = ImGui::GetWindowDrawList();
        float scale = Scale(), fs = FontSize();
        float headerH = S(36.0f), cornerR = S(8.0f), lineH = S(2.0f);
        float padX = S(16.0f);
        float padTop = S(12.0f), padBottom = S(8.0f);

        float scrollW = S(6.0f);
        float scrollPad = S(6.0f);
        float padRight = S(12.0f);

        dl->AddRectFilled(pos, ImVec2(pos.x + size.x, pos.y + size.y), ColPanel(), cornerR);
        dl->AddRectFilled(pos, ImVec2(pos.x + size.x, pos.y + headerH), ColPanelHeader(), cornerR, ImDrawFlags_RoundCornersTop);
        dl->AddRectFilledMultiColor(ImVec2(pos.x, pos.y + headerH), ImVec2(pos.x + size.x, pos.y + headerH + lineH),
            ColThemeLight(), ColTheme(), ColTheme(), ColThemeLight());

        float textY = pos.y + (headerH - fs) * 0.5f;
        dl->AddText(Fonts::FontAwesome, fs, ImVec2(pos.x + padX, textY), ColTheme(), icon);
        float iconW = Fonts::FontAwesome->CalcTextSizeA(fs, FLT_MAX, 0.0f, icon).x;
        dl->AddText(GetFont(), fs, ImVec2(pos.x + padX + iconW + S(8), textY), ColText(), title);

        float totalAvailableW = size.x - padX - padRight;
        CurrentPanelContentWidth = totalAvailableW - scrollW - scrollPad;

        ImGui::SetCursorScreenPos(ImVec2(pos.x + padX, pos.y + headerH + lineH + padTop));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, S(ImVec2(6, 8)));
        ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarSize, scrollW);
        ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarRounding, scrollW * 0.5f);

        ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0, 0, 0, 0));

        ImVec4 trackColor = ImGui::ColorConvertU32ToFloat4(ColPanel());
        trackColor.w = 0.15f;
        ImGui::PushStyleColor(ImGuiCol_ScrollbarBg, trackColor);

        ImVec4 grabColor = ImGui::ColorConvertU32ToFloat4(ColTheme());
        grabColor.w = 0.4f;
        ImGui::PushStyleColor(ImGuiCol_ScrollbarGrab, grabColor);

        ImVec4 grabHoverColor = ImGui::ColorConvertU32ToFloat4(ColTheme());
        grabHoverColor.w = 0.6f;
        ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabHovered, grabHoverColor);

        ImVec4 grabActiveColor = ImGui::ColorConvertU32ToFloat4(ColTheme());
        grabActiveColor.w = 0.8f;
        ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabActive, grabActiveColor);

        ImGui::BeginChild(id, ImVec2(totalAvailableW, size.y - headerH - lineH - padTop - padBottom),
            false, ImGuiWindowFlags_NoBackground);
    }

    inline void BeginChildPanel_Minimal(const char* id, const char* icon, const char* title, ImVec2 pos, ImVec2 size)
    {
        ImDrawList* dl = ImGui::GetWindowDrawList();
        float scale = Scale(), fs = FontSize();
        float headerH = S(36.0f), cornerR = S(8.0f), lineH = S(2.0f);
        float padX = S(16.0f);
        float padTop = S(12.0f), padBottom = S(8.0f);

        float scrollW = S(4.0f);
        float scrollPad = S(8.0f);
        float padRight = S(12.0f);

        dl->AddRectFilled(pos, ImVec2(pos.x + size.x, pos.y + size.y), ColPanel(), cornerR);
        dl->AddRectFilled(pos, ImVec2(pos.x + size.x, pos.y + headerH), ColPanelHeader(), cornerR, ImDrawFlags_RoundCornersTop);
        dl->AddRectFilledMultiColor(ImVec2(pos.x, pos.y + headerH), ImVec2(pos.x + size.x, pos.y + headerH + lineH),
            ColThemeLight(), ColTheme(), ColTheme(), ColThemeLight());

        float textY = pos.y + (headerH - fs) * 0.5f;
        dl->AddText(Fonts::FontAwesome, fs, ImVec2(pos.x + padX, textY), ColTheme(), icon);
        float iconW = Fonts::FontAwesome->CalcTextSizeA(fs, FLT_MAX, 0.0f, icon).x;
        dl->AddText(GetFont(), fs, ImVec2(pos.x + padX + iconW + S(8), textY), ColText(), title);

        float totalAvailableW = size.x - padX - padRight;
        CurrentPanelContentWidth = totalAvailableW - scrollW - scrollPad;

        ImGui::SetCursorScreenPos(ImVec2(pos.x + padX, pos.y + headerH + lineH + padTop));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, S(ImVec2(6, 8)));
        ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarSize, scrollW);
        ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarRounding, scrollW * 0.5f);

        ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0, 0, 0, 0));
        ImGui::PushStyleColor(ImGuiCol_ScrollbarBg, ImVec4(0, 0, 0, 0));

        ImVec4 grabColor = ImGui::ColorConvertU32ToFloat4(ColTheme());
        grabColor.w = 0.0f;
        ImGui::PushStyleColor(ImGuiCol_ScrollbarGrab, grabColor);

        grabColor.w = 0.5f;
        ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabHovered, grabColor);

        grabColor.w = 0.75f;
        ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabActive, grabColor);

        ImGui::BeginChild(id, ImVec2(totalAvailableW, size.y - headerH - lineH - padTop - padBottom),
            false, ImGuiWindowFlags_NoBackground);
    }

    inline void BeginChildPanel_macOS(const char* id, const char* icon, const char* title, ImVec2 pos, ImVec2 size)
    {
        ImDrawList* dl = ImGui::GetWindowDrawList();
        float scale = Scale(), fs = FontSize();
        float headerH = S(36.0f), cornerR = S(8.0f), lineH = S(2.0f);
        float padX = S(16.0f);
        float padTop = S(12.0f), padBottom = S(8.0f);

        float scrollW = S(8.0f);
        float scrollPad = S(4.0f);
        float padRight = S(10.0f);

        dl->AddRectFilled(pos, ImVec2(pos.x + size.x, pos.y + size.y), ColPanel(), cornerR);
        dl->AddRectFilled(pos, ImVec2(pos.x + size.x, pos.y + headerH), ColPanelHeader(), cornerR, ImDrawFlags_RoundCornersTop);
        dl->AddRectFilledMultiColor(ImVec2(pos.x, pos.y + headerH), ImVec2(pos.x + size.x, pos.y + headerH + lineH),
            ColThemeLight(), ColTheme(), ColTheme(), ColThemeLight());

        float textY = pos.y + (headerH - fs) * 0.5f;
        dl->AddText(Fonts::FontAwesome, fs, ImVec2(pos.x + padX, textY), ColTheme(), icon);
        float iconW = Fonts::FontAwesome->CalcTextSizeA(fs, FLT_MAX, 0.0f, icon).x;
        dl->AddText(GetFont(), fs, ImVec2(pos.x + padX + iconW + S(8), textY), ColText(), title);

        float totalAvailableW = size.x - padX - padRight;
        CurrentPanelContentWidth = totalAvailableW - scrollW - scrollPad;

        ImGui::SetCursorScreenPos(ImVec2(pos.x + padX, pos.y + headerH + lineH + padTop));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, S(ImVec2(6, 8)));
        ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarSize, scrollW);
        ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarRounding, S(4.0f));

        ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0, 0, 0, 0));

        ImGui::PushStyleColor(ImGuiCol_ScrollbarBg, ImVec4(0.0f, 0.0f, 0.0f, 0.1f));

        ImGui::PushStyleColor(ImGuiCol_ScrollbarGrab, ImVec4(0.8f, 0.8f, 0.8f, 0.3f));
        ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabHovered, ImVec4(0.9f, 0.9f, 0.9f, 0.5f));
        ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabActive, ImVec4(1.0f, 1.0f, 1.0f, 0.6f));

        ImGui::BeginChild(id, ImVec2(totalAvailableW, size.y - headerH - lineH - padTop - padBottom),
            false, ImGuiWindowFlags_NoBackground);
    }

    inline void EndChildPanel()
    {
        ImGui::EndChild();
        ImGui::PopStyleColor(5);
        ImGui::PopStyleVar(3);
        CurrentPanelContentWidth = 0.0f;
    }
    inline void UnlockAllAchievements() {
        HMODULE steamDll = GetModuleHandleA("steam_api64.dll");
        if (!steamDll) return;

        auto getSteamUserStats = (void* (*)())GetProcAddress(steamDll, "SteamAPI_SteamUserStats_v012");
        auto setAchievement = (bool(*)(void*, const char*))GetProcAddress(steamDll, "SteamAPI_ISteamUserStats_SetAchievement");
        auto storeStats = (bool(*)(void*))GetProcAddress(steamDll, "SteamAPI_ISteamUserStats_StoreStats");

        if (!getSteamUserStats || !setAchievement || !storeStats) return;

        void* stats = getSteamUserStats();
        if (!stats) return;

        const char* achievements[] = {
            "Boom", "Deathmatch", "Dominator", "Gang_Fight", "Gang_Fu",
            "Gunner", "Mad_Scientist", "Medic", "On_knives", "Screech_of_Steel",
            "Shooter", "Sniper", "The_King", "This_is_just_the_beginning",
            "Warrior", "Welcome_to_the_family"
        };

        for (const char* ach : achievements) {
            setAchievement(stats, ach);
        }

        storeStats(stats);
    }
    inline void Text(const char* fmt, ...)
    {
        va_list args;
        va_start(args, fmt);
        char buf[512];
        vsnprintf(buf, sizeof(buf), fmt, args);
        va_end(args);
        ImGui::GetWindowDrawList()->AddText(GetFont(), FontSize(), ImGui::GetCursorScreenPos(), ColText(), buf);
        ImGui::Dummy(ImVec2(ImGui::CalcTextSize(buf).x, FontSize()));
    }

    inline void TextDim(const char* fmt, ...)
    {
        va_list args;
        va_start(args, fmt);
        char buf[512];
        vsnprintf(buf, sizeof(buf), fmt, args);
        va_end(args);
        ImGui::GetWindowDrawList()->AddText(GetFont(), FontSize(), ImGui::GetCursorScreenPos(), ColTextDim(), buf);
        ImGui::Dummy(ImVec2(ImGui::CalcTextSize(buf).x, FontSize()));
    }
}
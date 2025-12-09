#pragma once

#include "Font/Byte.hpp"
#include "Font/FontAwesome.hpp"
#include <Obfusheader.hpp>
#include <string>
#include <vector>
#include <map>
#include <Windows.h>
#include <winreg.h>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <time.h>
#include <sstream>
#include <iomanip>

// User Placeholders
namespace stdvoid
{
    inline std::string username = "User";
    inline std::string subscriptionType = "Lifetime";
    inline std::string subscriptionExpiry = "Never";
}

// Temp Category Location
enum MainCategory {
    C_WEBSOCKET,
    C_SETTINGS
};

enum WebsocketSubCategory {
    S_BASIC,
    S_CLAN
};

enum SettingsSubCategory {
    S_MISC,
    S_TEST
};

// Snowflake 
struct SnowParticle {
    ImVec2 pos;
    float size;
    float speed;
    float sway_speed;
    float sway_offset;
    float depth;
    float wind_resistance;
};

// UI State
static std::vector<SnowParticle> snow_particles;
static float DimAlpha = 0.0f;
static float TargetDimAlpha = 0.8f;
static float MenuAlpha = 0.0f;

// Heplers
template <typename T>
T Clamp(const T& value, const T& min, const T& max) { return value < min ? min : (value > max ? max : value); }
float Lerp(float a, float b, float t) { return a + t * (b - a); }

void Backend::DrawImGui(ID3D11DeviceContext* context, ID3D11RenderTargetView* targetview) const
{
    if (!context || !targetview) return;

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    static MainCategory main_cat = C_WEBSOCKET;
    static int sub_cat = S_BASIC;
    static float content_anim = 0.f;
    static MainCategory prev_main_cat = main_cat;
    static int prev_sub_cat = sub_cat;
    static bool ConsoleInitialized = false;
    static bool DelayComplete = false;
    static float window_rounding = 8.0f;

    if (!ConsoleInitialized && !DelayComplete) { Sleep(1000); DelayComplete = true; }
    if (!ConsoleInitialized && DelayComplete) {
        AllocConsole();
        freopen("CONOUT$", "w", stdout);
        freopen("CONOUT$", "w", stderr);
        freopen("CONIN$", "r", stdin);
        std::cout << "Menu has opened Successfully!\n";
        ConsoleInitialized = true;
    }

    const float deltaTime = ImGui::GetIO().DeltaTime;
    const float openSpeed = 14.0f * deltaTime;
    const float closeSpeed = 9999.0f * deltaTime;

    // Animation State
    if (Features.OpenMenu) {
        DimAlpha = Clamp(DimAlpha + openSpeed, 0.0f, TargetDimAlpha);
        MenuAlpha = Clamp(MenuAlpha + openSpeed, 0.0f, 1.0f);
    }
    else {
        DimAlpha = Clamp(DimAlpha - closeSpeed, 0.0f, TargetDimAlpha);
        MenuAlpha = Clamp(MenuAlpha - closeSpeed, 0.0f, 1.0f);
    }

    // Background Dim
    if (MenuAlpha > 0.0f && DimAlpha > 0.0f /*&& Variables::Background*/) {
        ImGui::GetBackgroundDrawList()->AddRectFilled({ 0.f, 0.f }, ImGui::GetIO().DisplaySize, ImColor(0.0f, 0.0f, 0.0f, DimAlpha));
    }

    // Main Menu
    if (MenuAlpha > 0.0f)
    {
        if (prev_main_cat != main_cat || prev_sub_cat != sub_cat) {
            content_anim = 15.f;
            prev_main_cat = main_cat;
            prev_sub_cat = sub_cat;
        }
        content_anim = ImLerp(content_anim, 0.f, deltaTime * 10.f);

        // Color
        const ImU32 main_bg_col = IM_COL32(15, 15, 17, (int)(240 * MenuAlpha));
        const ImU32 sidebar_bg_col = IM_COL32(10, 10, 10, (int)(240 * MenuAlpha));
        const ImU32 separator_col = IM_COL32(45, 45, 45, (int)(250 * MenuAlpha));

        ImGui::SetNextWindowSize(ImVec2(950, 600));
        ImGui::PushStyleColor(ImGuiCol_WindowBg, main_bg_col);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, window_rounding);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

        ImGui::Begin("NOVA", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar);
        {
            auto draw = ImGui::GetWindowDrawList();
            auto pos = ImGui::GetWindowPos();
            auto size = ImGui::GetWindowSize();
            auto bg_draw = ImGui::GetBackgroundDrawList();

            // Glow
            const ImVec2 p_min = pos;
            const ImVec2 p_max = ImVec2(pos.x + size.x, pos.y + size.y);
            const ImU32 shadow_col = ImGui::ColorConvertFloat4ToU32(ImVec4(ImGui::ThemeColor.x, ImGui::ThemeColor.y, ImGui::ThemeColor.z, 0.5f * MenuAlpha));
            bg_draw->AddShadowRect(p_min, p_max, shadow_col, 50.0f, ImVec2(0, 0), ImDrawFlags_RoundCornersAll, window_rounding);

            const float navBarWidth = 160.f;
            const float sidebar_padding = 8.f;

            // Sidebar
            ImGui::SetCursorPos({ 0, 0 });
            ImGui::BeginChild("##LeftPane", ImVec2(navBarWidth, size.y), false, ImGuiWindowFlags_NoScrollbar);
            {
                auto nav_draw = ImGui::GetWindowDrawList();
                auto nav_pos = ImGui::GetCursorScreenPos();
                nav_draw->AddRectFilled(nav_pos, ImVec2(nav_pos.x + navBarWidth, nav_pos.y + size.y), sidebar_bg_col, window_rounding, ImDrawFlags_RoundCornersLeft);

                ImGui::SetCursorPos({ sidebar_padding, sidebar_padding });
                ImGui::BeginChild("##NavContentWrapper", ImVec2(navBarWidth - sidebar_padding * 2, size.y - sidebar_padding * 2));
                {
                    auto wrapper_draw = ImGui::GetWindowDrawList();
                    auto wrapper_pos = ImGui::GetCursorScreenPos();
                    auto wrapper_size = ImGui::GetWindowSize();
                    wrapper_draw->AddRect(wrapper_pos, ImVec2(wrapper_pos.x + wrapper_size.x, wrapper_pos.y + wrapper_size.y), separator_col, 6.f);

                    ImGui::SetCursorPos({ 15, 15 });
                    ImGui::BeginChild("##NavButtons", ImVec2(ImGui::GetContentRegionAvail().x - 5, ImGui::GetContentRegionAvail().y - 5), false, ImGuiWindowFlags_NoScrollbar);
                    {
                        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8.f, 8.f));

                        ImGui::CategoryHeader("Websocket", ImGui::ThemeColor, MenuAlpha);
                        if (ImGui::SubCategoryButton("Basic", sub_cat == S_BASIC && main_cat == C_WEBSOCKET, ImGui::ThemeColor)) { main_cat = C_WEBSOCKET; sub_cat = S_BASIC; }
                        if (ImGui::SubCategoryButton("Clan", sub_cat == S_CLAN && main_cat == C_WEBSOCKET, ImGui::ThemeColor)) { main_cat = C_WEBSOCKET; sub_cat = S_CLAN; }

                        ImGui::Dummy(ImVec2(0, 5));
                        ImGui::CategoryHeader("Settings", ImGui::ThemeColor, MenuAlpha);
                        if (ImGui::SubCategoryButton("Misc", sub_cat == S_MISC && main_cat == C_SETTINGS, ImGui::ThemeColor)) { main_cat = C_SETTINGS; sub_cat = S_MISC; }
                        if (ImGui::SubCategoryButton("Test Features", sub_cat == S_TEST && main_cat == C_SETTINGS, ImGui::ThemeColor)) { main_cat = C_SETTINGS; sub_cat = S_TEST; }

                        ImGui::PopStyleVar();
                    }
                    ImGui::EndChild();
                }
                ImGui::EndChild();
            }
            ImGui::EndChild();

            // Sidebar Separator Line
            draw->AddLine(ImVec2(pos.x + navBarWidth, pos.y), ImVec2(pos.x + navBarWidth, pos.y + size.y), separator_col);

            ImGui::SetCursorPos(ImVec2(navBarWidth, 0));
            ImGui::BeginChild("##ContentRegion", ImVec2(size.x - navBarWidth, size.y), false, ImGuiWindowFlags_NoBackground);
            {
                const float content_padding = 8.f;
                const float header_content_height = 65.f;
                const float separator_spacing = 8.f;
                const float total_header_area_height = header_content_height + separator_spacing;

                auto content_draw = ImGui::GetWindowDrawList();
                auto content_screen_pos = ImGui::GetCursorScreenPos();

                // Header Background
                content_draw->AddRectFilled(content_screen_pos, ImVec2(content_screen_pos.x + ImGui::GetContentRegionAvail().x, content_screen_pos.y + total_header_area_height), sidebar_bg_col, window_rounding, ImDrawFlags_RoundCornersTopRight);

                // Header Shit
                {
                    const float inner_padding_x = 15.f;
                    const float vertical_offset = 5.0f;

                    // Icon
                    ImGui::PushFont(Fonts::FontAwesome);
                    const char* icon_str = ICON_FA_USER;
                    ImGui::SetWindowFontScale(2.0f);
                    ImVec2 icon_size = ImGui::CalcTextSize(icon_str);
                    ImGui::SetWindowFontScale(1.0f);
                    ImVec2 icon_pos_screen = ImVec2(content_screen_pos.x + inner_padding_x, content_screen_pos.y + (header_content_height - icon_size.y) / 2.0f + vertical_offset);
                    content_draw->AddText(Fonts::FontAwesome, icon_size.y, icon_pos_screen, ImColor(ImGui::ThemeColor), icon_str);
                    ImGui::PopFont();

                    // User Text
                    float text_block_start_x_abs = icon_pos_screen.x + icon_size.x + 12.f;
                    std::string welcome_str = "Welcome back, " + stdvoid::username;
                    std::string sub_str = "Subscription: " + stdvoid::subscriptionType + " (Expires: " + stdvoid::subscriptionExpiry + ")";

                    ImVec2 welcome_size = ImGui::CalcTextSize(welcome_str.c_str());
                    ImVec2 sub_size = ImGui::CalcTextSize(sub_str.c_str());
                    float total_text_block_height = welcome_size.y + sub_size.y;
                    float block_start_y_abs = content_screen_pos.y + (header_content_height - total_text_block_height) / 2.0f + vertical_offset;

                    ImVec2 welcome_pos_screen = ImVec2(text_block_start_x_abs, block_start_y_abs);
                    ImVec2 sub_pos_screen = ImVec2(text_block_start_x_abs, welcome_pos_screen.y + welcome_size.y);

                    ImU32 start_col = ImColor(ImMin(1.0f, ImGui::ThemeColor.x * 1.3f), ImMin(1.0f, ImGui::ThemeColor.y * 1.3f), ImMin(1.0f, ImGui::ThemeColor.z * 1.3f), MenuAlpha);
                    ImU32 end_col = ImColor(ImGui::ThemeColor.x * 0.7f, ImGui::ThemeColor.y * 0.7f, ImGui::ThemeColor.z * 0.7f, MenuAlpha);

                    ImGui::RenderGradientText(welcome_str.c_str(), welcome_pos_screen, start_col, end_col);
                    content_draw->AddText(sub_pos_screen, ImGui::GetColorU32(ImGuiCol_TextDisabled), sub_str.c_str());

                    // Logo
                    const char* logo_text = "NOVA";
                    ImGui::PushFont(Fonts::Large);
                    ImGui::SetWindowFontScale(2.5f);
                    ImVec2 logo_size = ImGui::CalcTextSize(logo_text);
                    ImGui::SetWindowFontScale(1.0f);
                    ImGui::PopFont();
                    ImVec2 logo_pos_screen = ImVec2(content_screen_pos.x + ImGui::GetContentRegionAvail().x - logo_size.x - inner_padding_x, content_screen_pos.y + (header_content_height - logo_size.y) / 2.0f + vertical_offset);
                    ImGui::PushFont(Fonts::Large);
                    ImGui::SetWindowFontScale(2.5f);
                    ImGui::RenderGradientText(logo_text, logo_pos_screen, start_col, end_col);
                    ImGui::SetWindowFontScale(1.0f);
                    ImGui::PopFont();

                    // Stats
                    auto t = std::time(nullptr);
                    auto tm = *std::localtime(&t);
                    std::ostringstream time_ss; time_ss << std::put_time(&tm, "%I:%M:%S %p");
                    std::string time_str = "Time: " + time_ss.str();
                    std::string framerate_str = "Framerate: " + std::to_string((int)ImGui::GetIO().Framerate);
                    std::string version_str = "Game Version: " /*+ Variables::GameVersion*/;

                    ImVec2 time_size = ImGui::CalcTextSize(time_str.c_str());
                    ImVec2 framerate_size = ImGui::CalcTextSize(framerate_str.c_str());
                    ImVec2 version_size = ImGui::CalcTextSize(version_str.c_str());

                    float total_info_height = time_size.y + framerate_size.y + version_size.y;
                    float info_block_y_start = content_screen_pos.y + (header_content_height - total_info_height) / 2.f + vertical_offset;
                    float info_text_start_x = logo_pos_screen.x - 45.f;
                    ImU32 disabled_color = ImGui::GetColorU32(ImGuiCol_TextDisabled);

                    content_draw->AddText(ImVec2(info_text_start_x - time_size.x, info_block_y_start), disabled_color, time_str.c_str());
                    content_draw->AddText(ImVec2(info_text_start_x - framerate_size.x, info_block_y_start + time_size.y), disabled_color, framerate_str.c_str());
                    content_draw->AddText(ImVec2(info_text_start_x - version_size.x, info_block_y_start + time_size.y + framerate_size.y), disabled_color, version_str.c_str());
                }

                // Separator
                const float separator_y = content_screen_pos.y + total_header_area_height;
                content_draw->AddLine(ImVec2(content_screen_pos.x, separator_y), ImVec2(content_screen_pos.x + ImGui::GetContentRegionAvail().x, separator_y), separator_col);

                const float content_start_y = total_header_area_height + content_padding;
                ImGui::SetCursorPos(ImVec2(content_padding, content_start_y - content_anim));

                const float contentWidth = ImGui::GetContentRegionAvail().x - content_padding;
                const float contentHeight = ImGui::GetContentRegionAvail().y - content_padding;
                const float childGap = 10.f;
                const float colWidth = (contentWidth - childGap) / 2.f;

                switch (main_cat)
                {
                case C_WEBSOCKET:
                {
                    switch ((WebsocketSubCategory)sub_cat)
                    {
                    case S_BASIC:
                    {
                        ImGui::BeginGroup();
                        {
                            if (ImGui::BeginCustomChild("Currency", ImVec2(colWidth, 185)))
                            {
                                float availWidth = ImGui::GetContentRegionAvail().x;
                                float halfWidth = (availWidth - ImGui::GetStyle().ItemSpacing.x) / 2.f;

                                ImGui::Combo(OBF("Type"), &Variables::Websocket::m_iCurrencyType, Lists::m_ccCurrencies.data(), Lists::m_ccCurrencies.size(), 4);
                                ImGui::Text(OBF("Amount"));
                                ImGui::CustomInputInt(OBF("##currencyAmount"), "", &Variables::Websocket::m_iCurrencyAmount, 0, 0, ImGuiInputFlags_None, availWidth);
                                if (ImGui::CustomButton(OBF("##addCurrency"), OBF("Add Currency"), halfWidth, 30)) { Variables::Websocket::m_bAddCurrency = true; }
                                ImGui::SameLine();
                                if (ImGui::CustomButton(OBF("##spendCurrency"), OBF("Spend Currency"), halfWidth, 30)) { Variables::Websocket::m_bSpendCurrency = true; }
                            }
                            ImGui::EndCustomChild();

                            if (ImGui::BeginCustomChild("Consumables", ImVec2(colWidth, 185)))
                            {
                                float availWidth = ImGui::GetContentRegionAvail().x;

                                ImGui::Combo(OBF("Type"), &Variables::Websocket::m_iConsumableType, Lists::m_ccConsumables.data(), Lists::m_ccConsumables.size(), 4);
                                ImGui::Text(OBF("Amount"));
                                ImGui::CustomInputInt(OBF("##consumableAmount"), "", &Variables::Websocket::m_iConsumableAmount, 0, 0, ImGuiInputFlags_None, availWidth);
                                if (ImGui::CustomButton(OBF("##addConsumable"), OBF("Add Consumable"), availWidth, 30)) { Variables::Websocket::m_bAddConsumable = true; }
                            }
                            ImGui::EndCustomChild();
                        }
                        ImGui::EndGroup();
                    }
                    break;
                    case S_CLAN:
                    {
                        ImGui::BeginGroup();
                        {
   
                            ImGui::EndCustomChild();
                        }
                        ImGui::EndGroup();
                    }
                    break;
                    }
                }
                break;
                case C_SETTINGS:
                {
                    switch ((SettingsSubCategory)sub_cat)
                    {
                    case S_MISC:
                    {

                    }
                    break;
                    case S_TEST:
                    {
 
                    }
                    break;
                    }
                }
                break;
                }
            }
            ImGui::EndChild();
        }
        ImGui::End();
        ImGui::PopStyleVar(2);
        ImGui::PopStyleColor();
    }

    // Snowflakes Rendering
    if (MenuAlpha > 0.f /*&& Variables::Snowflakes*/)
    {
        auto bg_draw_list = ImGui::GetBackgroundDrawList();
        auto time = ImGui::GetTime();
        static bool initialized = false;
        static float wind_time = 0.0f;

        if (!initialized || snow_particles.empty()) {
            snow_particles.clear();
            snow_particles.reserve(1000);
            for (int i = 0; i < 1000; i++) {
                SnowParticle flake;
                flake.pos = ImVec2((float)(rand() % (int)ImGui::GetIO().DisplaySize.x), (float)(rand() % (int)ImGui::GetIO().DisplaySize.y));
                flake.depth = 0.2f + ((float)rand() / RAND_MAX) * 0.8f;
                float size_roll = (float)rand() / RAND_MAX;
                if (size_roll < 0.60f) { flake.size = 1.5f + ((float)rand() / RAND_MAX) * 1.5f; }
                else if (size_roll < 0.85f) { flake.size = 3.2f + ((float)rand() / RAND_MAX) * 1.3f; }
                else { flake.size = 4.8f + ((float)rand() / RAND_MAX) * 1.7f; }
                flake.speed = 40.0f + (flake.size * 12.0f) + ((float)rand() / RAND_MAX) * 30.0f;
                flake.sway_speed = 0.5f + ((float)rand() / RAND_MAX) * 1.2f;
                flake.sway_offset = ((float)rand() / RAND_MAX) * 6.28f;
                flake.wind_resistance = 0.3f + ((float)rand() / RAND_MAX) * 0.7f;
                snow_particles.push_back(flake);
            }
            initialized = true;
        }

        wind_time += deltaTime;
        float base_wind = 25.0f;
        float gust_1 = sinf(wind_time * 0.3f) * 20.0f;
        float gust_2 = sinf(wind_time * 0.8f) * 12.0f;
        float gust_3 = cosf(wind_time * 0.5f) * 8.0f;
        float total_wind = base_wind + gust_1 + gust_2 + gust_3;
        float mega_gust = sinf(wind_time * 0.15f) > 0.7f ? sinf(wind_time * 2.0f) * 15.0f : 0.0f;
        total_wind += mega_gust;
        float screenW = ImGui::GetIO().DisplaySize.x;
        float screenH = ImGui::GetIO().DisplaySize.y;

        for (auto& flake : snow_particles) {
            flake.pos.y += flake.speed * deltaTime;
            float wind_push = total_wind * flake.wind_resistance * flake.depth;
            flake.pos.x += wind_push * deltaTime;
            float swirl = sinf(time * flake.sway_speed + flake.sway_offset) * 12.0f;
            flake.pos.x += swirl * deltaTime;
            if (mega_gust > 5.0f) { flake.pos.y -= mega_gust * 0.3f * flake.wind_resistance * deltaTime; }

            if (flake.pos.y > screenH + 10) { flake.pos.y = -10; flake.pos.x = (float)(rand() % (int)screenW); }
            if (flake.pos.x > screenW + 10) { flake.pos.x = -10; flake.pos.y = (float)(rand() % (int)screenH); }
            if (flake.pos.x < -10) { flake.pos.x = screenW + 10; flake.pos.y = (float)(rand() % (int)screenH); }

            float flicker = 0.9f + sinf(time * 3.0f + flake.sway_offset) * 0.1f;
            float opacity = (0.65f + (flake.depth * 0.35f)) * flicker;
            ImU32 snow_color = IM_COL32(255, 255, 255, (int)(opacity * 255 * MenuAlpha));

            if (flake.size >= 4.8f) {
                ImGui::DrawDetailedSnowflake(bg_draw_list, flake.pos, flake.size, 0.0f, ImGui::ColorConvertU32ToFloat4(snow_color), 2);
            }
            else if (flake.size >= 3.2f) {
                ImGui::DrawDetailedSnowflake(bg_draw_list, flake.pos, flake.size, 0.0f, ImGui::ColorConvertU32ToFloat4(snow_color), 1);
            }
            else {
                bg_draw_list->AddCircleFilled(flake.pos, flake.size, snow_color, 8);
            }
        }
    }

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.f);
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(25.f / 255.f, 25.f / 255.f, 25.f / 255.f, 100.f / 255.f));
    ImGui::RenderNotifications();
    ImGui::PopStyleVar(1);
    ImGui::PopStyleColor(1);
    ImGui::EndFrame();
    ImGui::Render();

    context->OMSetRenderTargets(1, &targetview, NULL);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}
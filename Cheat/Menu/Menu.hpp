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
    if (MenuAlpha > 0.0f && DimAlpha > 0.0f && Variables::Background) {
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
                    std::string version_str = "Game Version: " + Variables::GameVersion;

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

                                ImGui::Combo(OBF("Type"), &Variables::CurrencyType, Lists::CurrencyList.data(), Lists::CurrencyList.size(), 4);
                                ImGui::Text(OBF("Amount"));
                                ImGui::CustomInputInt(OBF("##currencyAmount"), "", &Variables::CurrencyAmount, 0, 0, ImGuiInputFlags_None, availWidth);
                                if (ImGui::CustomButton(OBF("##addCurrency"), OBF("Add Currency"), halfWidth, 30)) { Variables::AddCurrency = true; }
                                ImGui::SameLine();
                                if (ImGui::CustomButton(OBF("##spendCurrency"), OBF("Spend Currency"), halfWidth, 30)) { Variables::SpendCurrency = true; }
                            }
                            ImGui::EndCustomChild();

                            if (ImGui::BeginCustomChild("Consumables", ImVec2(colWidth, 185)))
                            {
                                float availWidth = ImGui::GetContentRegionAvail().x;

                                ImGui::Combo(OBF("Type"), &Variables::ConsumableType, Lists::ConsumableList.data(), Lists::ConsumableList.size(), 4);
                                ImGui::Text(OBF("Amount"));
                                ImGui::CustomInputInt(OBF("##consumableAmount"), "", &Variables::ConsumableAmount, 0, 0, ImGuiInputFlags_None, availWidth);
                                if (ImGui::CustomButton(OBF("##addConsumable"), OBF("Add Consumable"), availWidth, 30)) { Variables::AddConsumable = true; }
                            }
                            ImGui::EndCustomChild();
                        }
                        ImGui::EndGroup();

                        ImGui::SameLine(0, childGap);

                        ImGui::BeginGroup();
                        {
                            if (ImGui::BeginCustomChild("Monthly Stats", ImVec2(colWidth, 230)))
                            {
                                float availWidth = ImGui::GetContentRegionAvail().x;

                                ImGui::Text(OBF("Kills")); ImGui::CustomInputInt(OBF("##kills"), "", &Variables::Kills, 0, 0, ImGuiInputFlags_None, availWidth);
                                ImGui::Text(OBF("Deaths")); ImGui::CustomInputInt(OBF("##deaths"), "", &Variables::Deaths, 0, 0, ImGuiInputFlags_None, availWidth);
                                // Removed other stats to fit, or you can make the child taller and others smaller
                                if (ImGui::CustomButton(OBF("##updateStats"), OBF("Update Stats"), availWidth, 30)) { Variables::MonthlyStats = true; }
                            }
                            ImGui::EndCustomChild();

                            if (ImGui::BeginCustomChild("Wins Adder", ImVec2(colWidth, 160)))
                            {
                                float availWidth = ImGui::GetContentRegionAvail().x;
                                ImGui::Combo(OBF("Game Mode"), &Variables::GameMode, Lists::GameModesList.data(), Lists::GameModesList.size(), 6);
                                ImGui::Text(OBF("Amount"));
                                ImGui::CustomInputInt(OBF("##winsAmount"), "", &Variables::WinsAmount, 0, 0, ImGuiInputFlags_None, availWidth);
                                if (ImGui::CustomButton(OBF("##addWins"), OBF("Add Wins"), availWidth, 30)) { Variables::AddWin = true; }
                            }
                            ImGui::EndCustomChild();

                            if (ImGui::BeginCustomChild("Upgrade Clan", ImVec2(colWidth, 90)))
                            {
                                float availWidth = ImGui::GetContentRegionAvail().x;
                                float halfWidth = (availWidth - ImGui::GetStyle().ItemSpacing.x) / 2.f;

                                ImGui::CustomSliderInt(OBF("Level"), &Variables::ClanLevel, 2, 10, "%d", ImGuiSliderFlags_None);
                                if (ImGui::CustomButton(OBF("##setFort"), OBF("Set Fort Level"), halfWidth, 30)) { Variables::SetClanFort = true; }
                                ImGui::SameLine();
                                if (ImGui::CustomButton(OBF("##setTank"), OBF("Set Tank Level"), halfWidth, 30)) { Variables::SetClanTank = true; }
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
                            if (ImGui::BeginCustomChild("Account", ImVec2(colWidth, 187)))
                            {
                                float availWidth = ImGui::GetContentRegionAvail().x;
                                ImGui::Combo(OBF("Clan Rank"), &Variables::ClanRank, Lists::ClanRanks.data(), Lists::ClanRanks.size(), 4);
                                ImGui::Text(OBF("Rank Points")); ImGui::CustomInputInt(OBF("##rankPoints"), "", &Variables::RankPoints, 0, 0, ImGuiInputFlags_None, availWidth);
                                if (ImGui::CustomButton(OBF("##setClanRank"), OBF("Set Rank"), availWidth, 30)) { Variables::SetRank = true; }
                            }
                            ImGui::EndCustomChild();

                            if (ImGui::BeginCustomChild("Clan Logo Editor", ImVec2(colWidth, 135)))
                            {
                                float availWidth = ImGui::GetContentRegionAvail().x;
                                ImGui::Combo(OBF("Type"), &Variables::LogoType, Lists::LogoList.data(), Lists::LogoList.size(), 4);
                                if (ImGui::CustomButton(OBF("##editClan"), OBF("Edit Clan Logo"), availWidth, 30)) { Variables::EditClanLogo = true; }
                            }
                            ImGui::EndCustomChild();

                            if (ImGui::BeginCustomChild("Clan Name Changer", ImVec2(colWidth, 170)))
                            {
                                float availWidth = ImGui::GetContentRegionAvail().x;
                                static ImVec4 sel_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
                                ImGui::Text(OBF("Clan Name")); if (ImGui::ColorEdit3(OBF("##clannamecolorpicker"), (float*)&sel_col)) {}
                                char code[9]; snprintf(code, sizeof(code), "[%02X%02X%02X]", (int)(sel_col.x * 255), (int)(sel_col.y * 255), (int)(sel_col.z * 255));
                                ImGui::CustomInputText(OBF("##clannamecolor"), "", availWidth, Variables::ClanNameColor, 5000, ImGuiInputFlags_None);
                                if (ImGui::CustomButton(OBF("##editClanName"), OBF("Edit Clan Name"), availWidth, 30))
                                {
                                    char final[21]; int rem = 20 - (int)strlen(code); if (rem > 0) snprintf(final, sizeof(final), "%s%.*s", code, rem, Variables::ClanNameColor); else strncpy(final, code, sizeof(final) - 1);
                                    final[sizeof(final) - 1] = '\0'; strncpy(Variables::ClanNameColor, final, sizeof(Variables::ClanNameColor) - 1); Variables::ClanNameColor[sizeof(Variables::ClanNameColor) - 1] = '\0'; Variables::EditClanName = true;
                                }
                                ImGui::Text(OBF("12 Letter Max"));
                            }
                            ImGui::EndCustomChild();
                        }
                        ImGui::EndGroup();

                        ImGui::SameLine(0, childGap);

                        ImGui::BeginGroup();
                        {
                            if (ImGui::BeginCustomChild("Modules", ImVec2(colWidth, 230)))
                            {
                                float availWidth = ImGui::GetContentRegionAvail().x;
                                float halfWidth = (availWidth - ImGui::GetStyle().ItemSpacing.x) / 2.f;

                                ImGui::Combo(OBF("Select Module"), &Variables::ModuleType, Lists::ModuleList.data(), Lists::ModuleList.size(), 8);
                                ImGui::Text(OBF("Module Amount"));
                                ImGui::CustomInputInt(OBF("##moduleAmount"), "", &Variables::ModuleAmount, 0, 0, ImGuiInputFlags_None, availWidth);
                                if (ImGui::CustomButton(OBF("##addModule"), OBF("Add Module"), halfWidth, 30)) { Variables::AddModule = true; }
                                ImGui::SameLine();
                                if (ImGui::CustomButton(OBF("##addAllModule"), OBF("Add All"), halfWidth, 30)) { Variables::AddAllModules = true; }
                                if (ImGui::CustomButton(OBF("##upgradeModule"), OBF("Upgrade Module"), halfWidth, 30)) { Variables::CheckSlots[1] = true; }
                                ImGui::SameLine();
                                if (ImGui::CustomButton(OBF("##upgradeAllModules"), OBF("Upgrade All"), halfWidth, 30)) { Variables::CheckSlots[2] = true; }
                                ImGui::CustomSliderInt(OBF("Level Increaser"), &Variables::ModuleLevel, 1, 9, "%d", ImGuiSliderFlags_None);
                            }
                            ImGui::EndCustomChild();

                            if (ImGui::BeginCustomChild("Level", ImVec2(colWidth, 128)))
                            {
                                float availWidth = ImGui::GetContentRegionAvail().x;
                                ImGui::CustomSliderInt(OBF("Level"), &Variables::AdderLevel, 1, 65, "%d", ImGuiSliderFlags_None);
                                if (ImGui::CustomButton(OBF("##setLevel"), OBF("Set Level"), availWidth, 30)) { Variables::SetLevel = true; }
                                ImGui::Text(OBF("Level Rewards after getting XP"));
                            }
                            ImGui::EndCustomChild();

                            if (ImGui::BeginCustomChild("Player Info", ImVec2(colWidth, 100)))
                            {
                                std::string PlayerLevelString = std::to_string(Variables::PlayerLevel);
                                ImGui::Text(OBF("Player ID: %s"), Variables::PlayerID.c_str());
                                ImGui::Text(OBF("Player Nick: %s"), Variables::PlayerUsername.c_str());
                                ImGui::Text(OBF("Player Level: %s"), PlayerLevelString.c_str());
                                ImGui::Text(OBF("Game Version: %s"), Variables::GameVersion.c_str());
                            }
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
                        if (ImGui::BeginCustomChild("Socket", ImVec2(contentWidth, 290)))
                        {
                            float availWidth = ImGui::GetContentRegionAvail().x;
                            ImGui::Text(OBF("Event Name")); ImGui::CustomInputText(OBF("##eventName"), "", availWidth, Variables::EventInput, 100, ImGuiInputFlags_None);
                            ImGui::Text(OBF("Command")); ImGui::CustomInputText(OBF("##command"), "", availWidth, Variables::CommandInput, 1000000, ImGuiInputFlags_None);
                            ImGui::Spacing();
                            if (ImGui::CustomButton(OBF("##sendCommand"), OBF("Send Command"), availWidth, 30)) { Variables::SendCommand = true; }
                            if (ImGui::CustomButton(OBF("##reload"), OBF("Reload Socket"), availWidth, 30)) { Variables::ReloadSocket = true; }
                            ImGui::Spacing();
                            ImGui::CustomCheckbox(OBF("Auto-Reload"), &Variables::AutoReload);
                            ImGui::CustomCheckbox(OBF("Log Websocket"), &Variables::LogWebsocket);
                            ImGui::TextDisabled(OBF("Do not use custom commands if you do not know what you are doing."));
                        }
                        ImGui::EndCustomChild();

                        if (ImGui::BeginCustomChild("Socials", ImVec2(contentWidth, 110)))
                        {
                            float availWidth = ImGui::GetContentRegionAvail().x;
                            if (ImGui::CustomButton(OBF("##yt"), OBF("YouTube"), availWidth, 30)) { ShellExecuteA(NULL, "open", "https://youtube.com/@mrbeast", 0, 0, SW_SHOWNORMAL); }
                            if (ImGui::CustomButton(OBF("##dsc"), OBF("Discord"), availWidth, 30)) { ShellExecuteA(NULL, "open", "https://discord.gg/voidpg", 0, 0, SW_SHOWNORMAL); }
                        }
                        ImGui::EndCustomChild();

                        if (ImGui::BeginCustomChild("Menu", ImVec2(contentWidth, 125)))
                        {
                            ImGui::CustomCheckbox(OBF("Draw Ash"), &Variables::Snowflakes);
                            ImGui::CustomCheckbox(OBF("Background Dim"), &Variables::Background);
                            ImGui::SliderFloat(OBF("Dim Amount"), &Variables::DimAmmount, 0.f, 2.f);
                        }
                        ImGui::EndCustomChild();
                    }
                    break;
                    case S_TEST:
                    {
                        if (ImGui::BeginCustomChild("Info", ImVec2(contentWidth, 75)))
                        {
                            ImGui::Text(OBF("Test Features, Working but will receive updates"));
                        }
                        ImGui::EndCustomChild();

                        ImGui::BeginGroup();
                        {
                            if (ImGui::BeginCustomChild("Clan Features", ImVec2(colWidth, 247)))
                            {
                                float availWidth = ImGui::GetContentRegionAvail().x;
                                float halfWidth = (availWidth - ImGui::GetStyle().ItemSpacing.x) / 2.f;

                                ImGui::CustomSliderInt(OBF("Valor Amount"), &Variables::ValorAmount, 1, 499, "%d", ImGuiSliderFlags_None);
                                ImGui::CustomSliderInt(OBF("Repeats"), &Variables::ValorRepeats, 1, 500000, "%d", ImGuiSliderFlags_None);
                                if (ImGui::CustomButton(OBF("##addValor"), OBF("Try to Add Valor"), halfWidth, 30)) { Variables::AddValor = true; }
                                if (ImGui::CustomButton(OBF("##clanTaskCompleter"), OBF("Clan Tasks Completer"), halfWidth, 30)) { Variables::CompleteClanTasks = true; }
                                ImGui::Text(OBF("WARNING!! Sabotage Features below"));
                                if (ImGui::CustomButton(OBF("bugclanfort"), OBF("Bug Clan Fort"), halfWidth, 30)) { Variables::BugClanFort = true; }
                                if (ImGui::CustomButton(OBF("unbugclanfort"), OBF("Unbug Clan Fort"), halfWidth, 30)) { Variables::UnBugClanFort = true; }
                            }
                            ImGui::EndCustomChild();
                        }
                        ImGui::EndGroup();

                        ImGui::SameLine(0, childGap);

                        ImGui::BeginGroup();
                        {
                            if (ImGui::BeginCustomChild("Goblin Adder", ImVec2(colWidth, 170)))
                            {
                                float halfWidth = (ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ItemSpacing.x) / 2.f;

                                ImGui::CustomSliderInt(OBF("Days of Clan Goblin"), &Variables::GoblinDays, 1, 100, "%d", ImGuiSliderFlags_None);
                                if (ImGui::CustomButton(OBF("addpositivegoblin"), OBF("Add Days"), halfWidth, 30)) { Variables::SetGoblinsPositive = true; }
                                ImGui::SameLine();
                                if (ImGui::CustomButton(OBF("addnegativegoblin"), OBF("Remove Days"), halfWidth, 30)) { Variables::SetGoblinsNegative = true; }
                            }
                            ImGui::EndCustomChild();
                        }
                        ImGui::EndGroup();
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
    if (MenuAlpha > 0.f && Variables::Snowflakes)
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
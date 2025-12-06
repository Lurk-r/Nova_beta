#pragma once

#include "Font/Byte.hpp"
#include "Font/FontAwesome.hpp"
#include "Snowflake.hpp"
#include <Obfusheader.hpp>
#include <fstream>
#include <string>
#include <Windows.h>
#include <Iphlpapi.h>
#include <iostream>
#include <random>

#pragma comment(lib, "iphlpapi.lib")

using json = nlohmann::json;

static float MenuAlpha = 0.0f;
static float DimAlpha = 0.0f;

constexpr int CHILD_X = 170;
constexpr float CHILD_SPACED_X = 458.5;
constexpr int FULL_CHILD_BASE_X = 567;
constexpr float HALF_CHILD_BASE_X = 278.5;
constexpr int FULL_BUTTON_LINE = 547;
constexpr float HALF_BUTTON_LINE = 258.5;
constexpr float QUART_BUTTON_LINE = 125.25;

template <typename T>
T Clamp(const T& value, const T& min, const T& max)
{
    return value < min ? min : (value > max ? max : value);
}

void Backend::DrawImGui(ID3D11DeviceContext* context, ID3D11RenderTargetView* targetview) const
{
    if (!context || !targetview) return;

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    static Heads Tab = Websocket;
    static float TabAnim = -10.0f;
    static int PreviousTab = -1;
    static bool ConsoleInitialized = false;
    static bool DelayComplete = false;

    if (!ConsoleInitialized && !DelayComplete)
    {
        Sleep(1000);
        DelayComplete = true;
    }

    if (!ConsoleInitialized && DelayComplete)
    {
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

    if (Features.OpenMenu)
    {
        DimAlpha = Clamp(DimAlpha + openSpeed, 0.0f, Variables::DimAmmount);
        MenuAlpha = Clamp(MenuAlpha + openSpeed, 0.0f, 1.0f);
    }
    else
    {
        DimAlpha = Clamp(DimAlpha - closeSpeed, 0.0f, Variables::DimAmmount);
        MenuAlpha = Clamp(MenuAlpha - closeSpeed, 0.0f, 1.0f);
    }

    if (DimAlpha > 0.0f)
    {
        if (Variables::Background)
            ImGui::GetBackgroundDrawList()->AddRectFilled({ 0, 0 }, { ImVec2(Features.WIDTH, Features.HEIGHT) }, ImColor(0.0f, 0.0f, 0.0f, DimAlpha));
    }

    if (MenuAlpha > 0.0f && Variables::Snowflakes)
    {
        Snowflake::Snowflakes();
    }

    if (MenuAlpha > 0.0f)
    {
        static WebsocketSubs SocketSubTab = Basic;
        static SettingsSubs MoreSubTab = Settings1;

        TabAnim = ImLerp(TabAnim, 0.0f, deltaTime * 10);

        ImGui::SetNextWindowSize(ImVec2(747, 635));
        ImGui::SetNextWindowBgAlpha(MenuAlpha);
        ImGui::Begin("ClanUtils", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollWithMouse);
        {
            auto draw = ImGui::GetWindowDrawList();
            auto pos = ImGui::GetWindowPos();
            auto size = ImGui::GetWindowSize();

            ImVec4 brightRedVec = ImVec4(0.1f, 0.6f, 1.0f, MenuAlpha);
            ImVec4 deepRedVec = ImVec4(0.1f, 0.3f, 0.8f, MenuAlpha);

            static float time = 0.0f;
            time += deltaTime * 3.0f;

            float t = (sin(time) * 0.5f) + 0.5f;
            t = t * t * (3 - 2 * t);

            ImVec4 flickerColorVec = ImVec4(brightRedVec.x + (deepRedVec.x - brightRedVec.x) * t, brightRedVec.y + (deepRedVec.y - brightRedVec.y) * t, brightRedVec.z + (deepRedVec.z - brightRedVec.z) * t, MenuAlpha);
            ImColor flickerColor = ImColor(flickerColorVec.x, flickerColorVec.y, flickerColorVec.z, flickerColorVec.w);

            const float newWidth = 747 * 1.02f;
            const float newHeight = 635 * 1.02f;
            const float offsetX = (newWidth - 747) * 0.5f;
            const float offsetY = (newHeight - 635) * 0.5f;
            const float rounding = 10.0f;
            const ImDrawFlags flags = ImDrawFlags_RoundCornersTopLeft | ImDrawFlags_RoundCornersTopRight | ImDrawFlags_RoundCornersBottomLeft | ImDrawFlags_RoundCornersBottomRight;

            ImGui::GetBackgroundDrawList()->AddShadowRect(ImVec2(pos.x - offsetX, pos.y - offsetY), ImVec2(pos.x + 747 + offsetX, pos.y + 635 + offsetY), flickerColor, 100, ImVec2(0, 0), flags, rounding);

            draw->AddRectFilled(ImVec2(pos.x + 10, pos.y + 10), ImVec2(pos.x + 160, pos.y + 70), ImColor(0.05f, 0.05f, 0.05f, MenuAlpha), 5.0f, ImDrawFlags_RoundCornersAll);
            draw->AddRectFilled(ImVec2(pos.x + 10, pos.y + 80), ImVec2(pos.x + 160, pos.y + 555), ImColor(0.05f, 0.05f, 0.05f, MenuAlpha), 5.0f, ImDrawFlags_RoundCornersAll);
            draw->AddRectFilled(ImVec2(pos.x + 10, pos.y + 565), ImVec2(pos.x + 160, pos.y + 625), ImColor(0.05f, 0.05f, 0.05f, MenuAlpha), 5.0f, ImDrawFlags_RoundCornersAll);
            draw->AddRectFilled(ImVec2(pos.x + 170, pos.y + 10), ImVec2(pos.x + CHILD_X + FULL_CHILD_BASE_X, pos.y + 70), ImColor(0.05f, 0.05f, 0.05f, MenuAlpha), 5.0f, ImDrawFlags_RoundCornersAll);

            draw->AddText(Fonts::Large, 22.0f, ImVec2(pos.x + 41, pos.y + 28), ImColor(60, 60, 255), OBF("Clan"));
            draw->AddText(Fonts::Large, 22.0f, ImVec2(pos.x + 88, pos.y + 28), ImColor(255, 255, 255), OBF("Utils"));
            draw->AddText(Fonts::Medium, 16.0f, ImVec2(pos.x + 597, pos.y + 30), ImColor(60, 60, 255), OBF(" discord.gg/"));
            draw->AddText(Fonts::Medium, 16.0f, ImVec2(pos.x + 680, pos.y + 30), ImColor(255, 255, 255), OBF("voidpg"));

            ImGui::SetCursorPos({ 20, 90 });
            ImGui::BeginGroup();
            {
                if (ImGui::Tab(Tab == Websocket, ICON_FA_CODE, OBF("Websocket"), ImVec2(131, 32))) { Tab = Websocket; }
                if (ImGui::Tab(Tab == Settings, ICON_FA_LIST, OBF("More"), ImVec2(131, 32))) { Tab = Settings; }
            }
            ImGui::EndGroup();

            if (PreviousTab != Tab)
            {
                TabAnim = 20.0f;
                PreviousTab = Tab;
            }

            switch (Tab)
            {
            case Websocket:
            {
                ImGui::SetCursorPos({ 180, 20 });
                ImGui::BeginGroup();
                {
                    if (ImGui::SubTab(SocketSubTab == Basic, OBF("Basic"), ImVec2(100, 40))) { SocketSubTab = Basic; }
                    ImGui::SameLine();
                    if (ImGui::SubTab(SocketSubTab == Clan, OBF("Clan"), ImVec2(100, 40))) { SocketSubTab = Clan; }
                }
                ImGui::EndGroup();

                switch (SocketSubTab)
                {
                case Basic:
                {
                    ImGui::SetCursorPos(ImVec2(CHILD_X, 80 - TabAnim));
                    ImGui::CustomChild(OBF("Currency"), ImVec2(HALF_CHILD_BASE_X, 185));
                    {
                        ImGui::Combo(OBF("Type"), &Variables::CurrencyType, Lists::CurrencyList.data(), Lists::CurrencyList.size(), 4);
                        ImGui::Text(OBF("Amount"));
                        ImGui::CustomInputInt(OBF("##currencyAmount"), "", &Variables::CurrencyAmount, 9, 0, ImGuiInputFlags_None, HALF_BUTTON_LINE);
                        if (ImGui::CustomButton(OBF("##addCurrency"), OBF("Add Currency"), QUART_BUTTON_LINE, 30)) { Variables::AddCurrency = true; }
                        ImGui::SameLine();
                        if (ImGui::CustomButton(OBF("##spendCurrency"), OBF("Spend Currency"), QUART_BUTTON_LINE, 30)) { Variables::SpendCurrency = true; }
                    }
                    ImGui::EndChild();

                    ImGui::SetCursorPos(ImVec2(CHILD_X, 275 - TabAnim));
                    ImGui::CustomChild(OBF("Monthly Stats"), ImVec2(HALF_CHILD_BASE_X, 350));
                    {
                        ImGui::Text(OBF("Kills"));
                        ImGui::CustomInputInt(OBF("##kills"), "", &Variables::Kills, 9, 0, ImGuiInputFlags_None, HALF_BUTTON_LINE);
                        ImGui::Text(OBF("Deaths"));
                        ImGui::CustomInputInt(OBF("##deaths"), "", &Variables::Deaths, 9, 0, ImGuiInputFlags_None, HALF_BUTTON_LINE);
                        ImGui::Text(OBF("Head Shots"));
                        ImGui::CustomInputInt(OBF("##headShots"), "", &Variables::HeadShots, 9, 0, ImGuiInputFlags_None, HALF_BUTTON_LINE);
                        ImGui::Text(OBF("Kill Streak"));
                        ImGui::CustomInputInt(OBF("##killStreak"), "", &Variables::KillStreak, 9, 0, ImGuiInputFlags_None, HALF_BUTTON_LINE);
                        ImGui::Text(OBF("Win Streak"));
                        ImGui::CustomInputInt(OBF("##winStreak"), "", &Variables::WinStreak, 9, 0, ImGuiInputFlags_None, HALF_BUTTON_LINE);
                        if (ImGui::CustomButton(OBF("##updateStats"), OBF("Update Stats"), HALF_BUTTON_LINE, 45)) { Variables::MonthlyStats = true; }
                    }
                    ImGui::EndChild();

                    ImGui::SetCursorPos(ImVec2(CHILD_SPACED_X, 80 - TabAnim));
                    ImGui::CustomChild(OBF("Consumables"), ImVec2(HALF_CHILD_BASE_X, 185));
                    {
                        ImGui::Combo(OBF("Type"), &Variables::ConsumableType, Lists::ConsumableList.data(), Lists::ConsumableList.size(), 4);
                        ImGui::Text(OBF("Amount"));
                        ImGui::CustomInputInt(OBF("##consumableAmount"), "", &Variables::ConsumableAmount, 9, 0, ImGuiInputFlags_None, HALF_BUTTON_LINE);
                        if (ImGui::CustomButton(OBF("##addConsumable"), OBF("Add Consumable"), HALF_BUTTON_LINE, 30)) { Variables::AddConsumable = true; }
                    }
                    ImGui::EndChild();

                    ImGui::SetCursorPos(ImVec2(CHILD_SPACED_X, 275 - TabAnim));
                    ImGui::CustomChild(OBF("Wins Adder"), ImVec2(HALF_CHILD_BASE_X, 215));
                    {
                        ImGui::Combo(OBF("Game Mode"), &Variables::GameMode, Lists::GameModesList.data(), Lists::GameModesList.size(), 6);
                        ImGui::Text(OBF("Amount"));
                        ImGui::CustomInputInt(OBF("##winsAmount"), "", &Variables::WinsAmount, 5, 0, ImGuiInputFlags_None, HALF_BUTTON_LINE);
                        if (ImGui::CustomButton(OBF("##addWins"), OBF("Add Wins"), HALF_BUTTON_LINE, 30)) { Variables::AddWin = true; }
                        ImGui::Text(OBF("Do not use values over 1000 as it may\nfreeze/lag the game, use smaller ones."));
                    }
                    ImGui::EndChild();

                    ImGui::SetCursorPos(ImVec2(CHILD_SPACED_X, 500 - TabAnim));
                    ImGui::CustomChild(OBF("Upgrade Clan"), ImVec2(HALF_CHILD_BASE_X, 125));
                    {
                        ImGui::CustomSliderInt(OBF("Level"), &Variables::ClanLevel, 2, 10, "%d", ImGuiSliderFlags_None);
                        if (ImGui::CustomButton(OBF("##setFort"), OBF("Set Fort Level"), QUART_BUTTON_LINE, 30)) { Variables::SetClanFort = true; }
                        ImGui::SameLine();
                        if (ImGui::CustomButton(OBF("##setTank"), OBF("Set Tank Level"), QUART_BUTTON_LINE, 30)) { Variables::SetClanTank = true; }
                        ImGui::Text(OBF("You must be either officer or leader"));
                    }
                    ImGui::EndChild();
                }
                break;

                case Clan:
                {
                    ImGui::SetCursorPos(ImVec2(CHILD_X, 80 - TabAnim));
                    ImGui::CustomChild(OBF("Account"), ImVec2(HALF_CHILD_BASE_X, 187));
                    {
                        ImGui::Combo(OBF("Clan Rank"), &Variables::ClanRank, Lists::ClanRanks.data(), Lists::ClanRanks.size(), 4);
                        ImGui::Text(OBF("Rank Points"));
                        ImGui::CustomInputInt(OBF("##rankPoints"), "", &Variables::RankPoints, 50, 0, ImGuiInputFlags_None, HALF_BUTTON_LINE);
                        if (ImGui::CustomButton(OBF("##setClanRank"), OBF("Set Rank"), HALF_BUTTON_LINE, 30)) { Variables::SetRank = true; }
                    }
                    ImGui::EndChild();

                    ImGui::SetCursorPos(ImVec2(CHILD_X, 277 - TabAnim));
                    ImGui::CustomChild(OBF("Modules"), ImVec2(HALF_CHILD_BASE_X, 257));
                    {
                        ImGui::Combo(OBF("Select Module"), &Variables::ModuleType, Lists::ModuleList.data(), Lists::ModuleList.size(), 8);
                        ImGui::Text(OBF("Module Amount"));
                        ImGui::CustomInputInt(OBF("##moduleAmount"), "", &Variables::ModuleAmount, 50, 0, ImGuiInputFlags_None, HALF_BUTTON_LINE);
                        if (ImGui::CustomButton(OBF("##addModule"), OBF("Add Module"), QUART_BUTTON_LINE, 30)) { Variables::AddModule = true; }
                        ImGui::SameLine();
                        if (ImGui::CustomButton(OBF("##addAllModule"), OBF("Add All"), QUART_BUTTON_LINE, 30)) { Variables::AddAllModules = true; }
                        if (ImGui::CustomButton(OBF("##upgradeModule"), OBF("Upgrade Module"), QUART_BUTTON_LINE, 30)) { Variables::CheckSlots[1] = true; }
                        ImGui::SameLine();
                        if (ImGui::CustomButton(OBF("##upgradeAllModules"), OBF("Upgrade All"), QUART_BUTTON_LINE, 30)) { Variables::CheckSlots[2] = true; }
                        ImGui::CustomSliderInt(OBF("Level Increaser"), &Variables::ModuleLevel, 1, 9, "%d", ImGuiSliderFlags_None);
                    }
                    ImGui::EndChild();

                    ImGui::SetCursorPos(ImVec2(CHILD_SPACED_X, 80 - TabAnim));
                    ImGui::CustomChild(OBF("Clan Logo Editor"), ImVec2(HALF_CHILD_BASE_X, 135));
                    {
                        ImGui::Combo(OBF("Type"), &Variables::LogoType, Lists::LogoList.data(), Lists::LogoList.size(), 4);
                        if (ImGui::CustomButton(OBF("##editClan"), OBF("Edit Clan Logo"), HALF_BUTTON_LINE, 30)) { Variables::EditClanLogo = true; }
                    }
                    ImGui::EndChild();

                    ImGui::SetCursorPos(ImVec2(CHILD_SPACED_X, 225 - TabAnim));
                    ImGui::CustomChild(OBF("Clan Name Changer"), ImVec2(HALF_CHILD_BASE_X, 170));
                    {
                        static ImVec4 selectedColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
                        ImGui::Text(OBF("Clan Name"));
                        if (ImGui::ColorEdit3(OBF("##clannamecolorpicker"), (float*)&selectedColor)) {}
                        char colorCode[9];
                        snprintf(colorCode, sizeof(colorCode), "[%02X%02X%02X]", (int)(selectedColor.x * 255), (int)(selectedColor.y * 255), (int)(selectedColor.z * 255));
                        ImGui::CustomInputText(OBF("##clannamecolor"), OBF("Clan Name"), HALF_BUTTON_LINE, Variables::ClanNameColor, 5000, ImGuiInputFlags_None);
                        if (ImGui::CustomButton(OBF("##editClanName"), OBF("Edit Clan Name"), HALF_BUTTON_LINE, 30))
                        {
                            char finalClanName[21];
                            int remainingLength = 20 - (int)strlen(colorCode);
                            if (remainingLength > 0)
                            {
                                snprintf(finalClanName, sizeof(finalClanName), "%s%.*s", colorCode, remainingLength, Variables::ClanNameColor);
                            }
                            else
                            {
                                strncpy(finalClanName, colorCode, sizeof(finalClanName) - 1);
                            }
                            finalClanName[sizeof(finalClanName) - 1] = '\0';
                            strncpy(Variables::ClanNameColor, finalClanName, sizeof(Variables::ClanNameColor) - 1);
                            Variables::ClanNameColor[sizeof(Variables::ClanNameColor) - 1] = '\0';

                            Variables::EditClanName = true;
                        }
                        ImGui::Text(OBF("12 Letter Max"));
                    }
                    ImGui::EndChild();

                    ImGui::SetCursorPos(ImVec2(CHILD_SPACED_X, 405 - TabAnim));
                    ImGui::CustomChild(OBF("Level"), ImVec2(HALF_CHILD_BASE_X, 128));
                    {
                        ImGui::CustomSliderInt(OBF("Level"), &Variables::AdderLevel, 1, 65, "%d", ImGuiSliderFlags_None);
                        if (ImGui::CustomButton(OBF("##setLevel"), OBF("Set Level"), HALF_BUTTON_LINE, 30)) { Variables::SetLevel = true; }
                        ImGui::Text(OBF("Level Rewards after getting XP"));
                    }
                    ImGui::EndChild();

                    ImGui::SetCursorPos(ImVec2(CHILD_X, 544 - TabAnim));
                    ImGui::CustomChild(OBF("Player Info"), ImVec2(FULL_CHILD_BASE_X, 80));
                    {
                        std::string PlayerLevelString = std::to_string(Variables::PlayerLevel);
                        ImGui::Text(OBF("Player ID: %s"), Variables::PlayerID.c_str());
                        ImGui::SameLine(); ImGui::SetCursorPosX(HALF_CHILD_BASE_X + 20);
                        ImGui::Text(OBF("Player Nick: %s"), Variables::PlayerUsername.c_str());
                        ImGui::Text(OBF("Player Level: %s"), PlayerLevelString.c_str());
                        ImGui::SameLine(); ImGui::SetCursorPosX(HALF_CHILD_BASE_X + 20);
                        ImGui::Text(OBF("Game Version: %s"), Variables::GameVersion.c_str());
                    }
                    ImGui::EndChild();
                }
                break;
                }
            }
            break;

            case Settings:
            {
                ImGui::SetCursorPos({ 180, 20 });
                ImGui::BeginGroup();
                {
                    if (ImGui::SubTab(MoreSubTab == Settings1, OBF("Misc"), ImVec2(100, 40))) { MoreSubTab = Settings1; }
                    ImGui::SameLine();
                    if (ImGui::SubTab(MoreSubTab == Settings2, OBF("Test Features"), ImVec2(100, 40))) { MoreSubTab = Settings2; }
                }
                ImGui::EndGroup();

                switch (MoreSubTab)
                {
                case Settings1:
                {
                    ImGui::SetCursorPos(ImVec2(CHILD_X, 80 - TabAnim));
                    ImGui::CustomChild(OBF("Socket"), ImVec2(FULL_CHILD_BASE_X, 290));
                    {
                        ImGui::Text(OBF("Event Name"));
                        ImGui::CustomInputText(OBF("##eventName"), OBF("Event Name"), FULL_BUTTON_LINE, Variables::EventInput, 100, ImGuiInputFlags_None);
                        ImGui::Text(OBF("Command"));
                        ImGui::CustomInputText(OBF("##command"), OBF("Command"), FULL_BUTTON_LINE, Variables::CommandInput, 1000000, ImGuiInputFlags_None);
                        ImGui::Spacing();
                        if (ImGui::CustomButton(OBF("##sendCommand"), OBF("Send Command"), FULL_BUTTON_LINE, 30)) { Variables::SendCommand = true; }
                        if (ImGui::CustomButton(OBF("##reload"), OBF("Reload Socket"), FULL_BUTTON_LINE, 30)) { Variables::ReloadSocket = true; }
                        ImGui::Spacing();
                        ImGui::CustomCheckbox(OBF("Auto-Reload"), &Variables::AutoReload);
                        ImGui::CustomCheckbox(OBF("Log Websocket"), &Variables::LogWebsocket);
                        ImGui::Text(OBF("Do not use custom commands if you do not know what you are doing."));
                    }
                    ImGui::EndChild();

                    ImGui::SetCursorPos(ImVec2(CHILD_X, 380 - TabAnim));
                    ImGui::CustomChild(OBF("Socials"), ImVec2(FULL_CHILD_BASE_X, 110));
                    {
                        if (ImGui::CustomButton(OBF("##yt"), OBF("YouTube"), FULL_BUTTON_LINE, 30)) { ShellExecuteA(NULL, "open", "https://youtube.com/@mrbeast", 0, 0, SW_SHOWNORMAL); }
                        if (ImGui::CustomButton(OBF("##dsc"), OBF("Discord"), FULL_BUTTON_LINE, 30)) { ShellExecuteA(NULL, "open", "https://discord.gg/voidpg", 0, 0, SW_SHOWNORMAL); }
                    }
                    ImGui::EndChild();

                    ImGui::SetCursorPos(ImVec2(CHILD_X, 500 - TabAnim));
                    ImGui::CustomChild(OBF("Menu"), ImVec2(FULL_CHILD_BASE_X, 125));
                    {
                        ImGui::CustomCheckbox(OBF("Draw Ash"), &Variables::Snowflakes);
                        ImGui::CustomCheckbox(OBF("Background Dim"), &Variables::Background);
                        ImGui::SliderFloat(OBF("Dim Amount"), &Variables::DimAmmount, 0.f, 2.f);
                    }
                    ImGui::EndChild();
                }
                break;

                case Settings2:
                {
                    ImGui::SetCursorPos(ImVec2(CHILD_X, 80 - TabAnim));
                    ImGui::CustomChild(OBF("Info"), ImVec2(FULL_CHILD_BASE_X, 75));
                    {
                        ImGui::Text(OBF("Test Features, Working but will receive updates"));
                    }
                    ImGui::EndChild();

                    ImGui::SetCursorPos(ImVec2(CHILD_X, 165 - TabAnim));
                    ImGui::CustomChild(OBF("Clan Features"), ImVec2(HALF_CHILD_BASE_X, 247));
                    {
                        ImGui::CustomSliderInt(OBF("Valor Amount"), &Variables::ValorAmount, 1, 499, "%d", ImGuiSliderFlags_None);
                        ImGui::CustomSliderInt(OBF("Repeats"), &Variables::ValorRepeats, 1, 500000, "%d", ImGuiSliderFlags_None);
                        if (ImGui::CustomButton(OBF("##addValor"), OBF("Try to Add Valor"), HALF_BUTTON_LINE, 30)) { Variables::AddValor = true; }
                        if (ImGui::CustomButton(OBF("##clanTaskCompleter"), OBF("Clan Tasks Completer"), HALF_BUTTON_LINE, 30)) { Variables::CompleteClanTasks = true; }
                        ImGui::Text("WARNING!! Sabotage Features below");
                        if (ImGui::CustomButton(OBF("bugclanfort"), OBF("Bug Clan Fort"), HALF_BUTTON_LINE, 30)) { Variables::BugClanFort = true; }
                        if (ImGui::CustomButton(OBF("unbugclanfort"), OBF("Unbug Clan Fort"), HALF_BUTTON_LINE, 30)) { Variables::UnBugClanFort = true; }
                    }
                    ImGui::EndChild();

                    ImGui::SetCursorPos(ImVec2(CHILD_SPACED_X, 165 - TabAnim));
                    ImGui::CustomChild(OBF("Goblin Adder"), ImVec2(HALF_CHILD_BASE_X, 170));
                    {
                        ImGui::CustomSliderInt(OBF("Days of Clan Goblin"), &Variables::GoblinDays, 1, 100, "%d", ImGuiSliderFlags_None);
                        if (ImGui::CustomButton(OBF("addpositivegoblin"), OBF("Add Days"), QUART_BUTTON_LINE, 30)) { Variables::SetGoblinsPositive = true; }
                        ImGui::SameLine();
                        if (ImGui::CustomButton(OBF("addnegativegoblin"), OBF("Remove Days"), QUART_BUTTON_LINE, 30)) { Variables::SetGoblinsNegative = true; }
                    }
                    ImGui::EndChild();
                }
                break;
                }
            }
            break;
            }

            draw->AddText(Fonts::Medium, 16.0f, ImVec2(pos.x + 27, pos.y + 580), ImColor(60, 60, 255), "Logged In");
            draw->AddText(Fonts::Medium, 14.0f, ImVec2(pos.x + 27, pos.y + 600), ImColor(255, 255, 255), "Active");
        }
        ImGui::End();
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
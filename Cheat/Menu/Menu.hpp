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

namespace stdvoid {
    inline std::string username = "User";
    inline std::string subscriptionType = "Lifetime";
    inline std::string subscriptionExpiry = "Never";
}

enum MainCategory { C_WEBSOCKET, C_SETTINGS };
enum WebsocketSubCategory { S_BASIC, S_CLAN };
enum SettingsSubCategory { S_MISC, S_TEST };

inline const char* GetMainCategoryLabel(MainCategory category) {
    switch (category) {
    case C_WEBSOCKET:  return "Websocket";
    case C_SETTINGS: return "Settings";
    default: return "Menu";
    }
}

inline const char* GetWebsocketSubLabel(WebsocketSubCategory sub_index) {
    switch (sub_index) {
    case S_CLAN: return "Clan";
    case S_BASIC:
    default: return "Basic";
    }
}

inline const char* GetSettingsSubLabel(SettingsSubCategory sub_index) {
    switch (sub_index) {
    case S_TEST: return "Testing";
    case S_MISC:
    default: return "Misc";
    }
}

namespace ModernUI {
    constexpr float WINDOW_WIDTH = 720.f;
    constexpr float WINDOW_HEIGHT = 480.f;
    constexpr float WINDOW_ROUNDING = 16.0f;
    constexpr float SIDEBAR_WIDTH = 140.f;
    constexpr float HEADER_HEIGHT = 42.f;

    constexpr float ANIM_SPEED_OPEN = 10.0f;
    constexpr float ANIM_SPEED_INSTANT = 100.0f;
    constexpr float ANIM_SPEED_CLOSE = ANIM_SPEED_INSTANT;
    constexpr float ANIM_SPEED_CONTENT = ANIM_SPEED_INSTANT;
    constexpr float CONTENT_ANIM_START_PROGRESS = 0.0f;
    constexpr float CONTENT_SPEED_MULTIPLIER = 1.0f;
    constexpr float CONTENT_OFFSET_Y_MAX = 0.0f;

    constexpr float GRAD_PHASE_FREQ = 1.8f;
    constexpr float GRAD_LIFT = 0.15f;
    constexpr float GRAD_VARIANCE = 0.12f;
    constexpr float GRAD_Y_OFFSET = 0.06f;
    constexpr float GRAD_ACCENT_LIFT = 0.18f;
    constexpr float GRAD_BASE_SCALE = 0.70f;
    constexpr float GRAD_SCALE_VARIANCE = 0.15f;
    constexpr float GRAD_ACCENT_SCALE = 1.15f;

    constexpr int SNOW_PARTICLE_COUNT = 400;

    inline ImU32 BgPrimary(float alpha = 1.0f) { return IM_COL32(8, 8, 10, static_cast<int>(252 * alpha)); }
    inline ImU32 BgSecondary(float alpha = 1.0f) { return IM_COL32(4, 4, 5, static_cast<int>(255 * alpha)); }
    inline ImU32 BgTertiary(float alpha = 1.0f) { return IM_COL32(4, 4, 5, static_cast<int>(255 * alpha)); }
    inline ImU32 BorderColor(float alpha = 1.0f) { return IM_COL32(45, 45, 55, static_cast<int>(200 * alpha)); }
    inline ImU32 TextPrimary(float alpha = 1.0f) { return IM_COL32(230, 230, 240, static_cast<int>(255 * alpha)); }
    inline ImU32 TextSecondary(float alpha = 1.0f) { return IM_COL32(130, 130, 150, static_cast<int>(255 * alpha)); }

    inline float EaseOutCubic(float t) { return 1.0f - std::pow(1.0f - t, 3.0f); }
    inline float EaseOutQuart(float t) { return 1.0f - std::pow(1.0f - t, 4.0f); }
}

struct SnowParticle {
    ImVec2 pos;
    float size;
    float speed;
    float sway_speed;
    float sway_offset;
    float depth;
    float wind_resistance;
    float rotation;
    float rotation_speed;
};

struct AnimState {
    float content_offset_y = 0.0f;
    ImVec2 drag_offset = ImVec2(0, 0);
    bool is_dragging = false;
};

static std::vector<SnowParticle> snow_particles;
static float DimAlpha = 0.0f;
static float TargetDimAlpha = 0.75f;
static float MenuAlpha = 0.0f;
static float MenuScale = 0.96f;
static float OpenProgress = 0.0f;
static ImVec2 g_WindowPos = ImVec2(-1, -1);
static AnimState g_AnimState;

template <typename T>
constexpr T Clamp(const T& value, const T& min, const T& max) {
    return std::clamp(value, min, max);
}

inline float SmoothLerp(float a, float b, float t, float dt) {
    float smoothT = 1.0f - std::exp(-t * dt);
    return a + smoothT * (b - a);
}

void Backend::DrawImGui(ID3D11DeviceContext* context, ID3D11RenderTargetView* targetview) const {
    if (!context || !targetview) return;

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    using namespace ModernUI;

    static MainCategory main_cat = C_WEBSOCKET;
    static int sub_cat = S_BASIC;
    static float content_anim = 0.f;
    static MainCategory prev_main_cat = main_cat;
    static int prev_sub_cat = sub_cat;
    static bool ConsoleInitialized = false;
    static bool DelayComplete = false;
    static float pulse_time = 0.0f;

    if (!ConsoleInitialized && !DelayComplete) {
        Sleep(1000);
        DelayComplete = true;
    }

    if (!ConsoleInitialized && DelayComplete) {
        AllocConsole();
        freopen_s(reinterpret_cast<FILE**>(stdout), "CONOUT$", "w", stdout);
        freopen_s(reinterpret_cast<FILE**>(stderr), "CONOUT$", "w", stderr);
        freopen_s(reinterpret_cast<FILE**>(stdin), "CONIN$", "r", stdin);
        std::cout << "Nova Menu Initialized!\n";
        ConsoleInitialized = true;
    }

    const float dt = ImGui::GetIO().DeltaTime;
    pulse_time += dt;

    float target_open = Features.OpenMenu ? 1.0f : 0.0f;
    float open_speed = Features.OpenMenu ? ANIM_SPEED_OPEN : ANIM_SPEED_CLOSE;
    OpenProgress = SmoothLerp(OpenProgress, target_open, open_speed, dt);

    MenuAlpha = EaseOutCubic(OpenProgress);
    DimAlpha = TargetDimAlpha * MenuAlpha;
    MenuScale = 0.97f + 0.03f * MenuAlpha;

    if (!Features.OpenMenu && OpenProgress < 0.05f) {
        content_anim = 0.0f;
        g_AnimState.content_offset_y = 0.0f;
    }

    if (OpenProgress > 0.01f && DimAlpha > 0.01f) {
        auto bg = ImGui::GetBackgroundDrawList();
        ImVec2 display = ImGui::GetIO().DisplaySize;
        ImU32 dimCol = IM_COL32(0, 0, 5, static_cast<int>(DimAlpha * 255));
        bg->AddRectFilled({ 0.f, 0.f }, display, dimCol);
    }

    if (OpenProgress > 0.01f) {
        ImVec4 tc = ImGui::ThemeColor;

        if (prev_main_cat != main_cat || prev_sub_cat != sub_cat) {
            content_anim = CONTENT_ANIM_START_PROGRESS;
            g_AnimState.content_offset_y = CONTENT_OFFSET_Y_MAX;
            prev_main_cat = main_cat;
            prev_sub_cat = sub_cat;
        }
        const float contentSpeed = ANIM_SPEED_CONTENT * CONTENT_SPEED_MULTIPLIER;
        content_anim = SmoothLerp(content_anim, 0.f, contentSpeed, dt);
        float contentProgress = EaseOutQuart(1.0f - Clamp(content_anim, 0.0f, 1.0f));
        g_AnimState.content_offset_y = CONTENT_OFFSET_Y_MAX * (1.0f - contentProgress) * MenuAlpha;

        ImVec2 display = ImGui::GetIO().DisplaySize;
        if (g_WindowPos.x < 0 || g_WindowPos.y < 0) {
            g_WindowPos = ImVec2((display.x - WINDOW_WIDTH) * 0.5f, (display.y - WINDOW_HEIGHT) * 0.5f);
        }

        ImGui::SetNextWindowSize(ImVec2(WINDOW_WIDTH, WINDOW_HEIGHT));
        ImGui::SetNextWindowPos(g_WindowPos, ImGuiCond_Always);

        ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(0, 0, 0, 0));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, WINDOW_ROUNDING);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

        ImGui::Begin("##NovaMenu", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar);
        {
            auto draw = ImGui::GetWindowDrawList();
            auto pos = ImGui::GetWindowPos();
            auto size = ImGui::GetWindowSize();
            auto bg_draw = ImGui::GetBackgroundDrawList();

            ImVec2 mousePos = ImGui::GetMousePos();
            bool inDragArea = (mousePos.x >= pos.x && mousePos.x <= pos.x + SIDEBAR_WIDTH &&
                mousePos.y >= pos.y && mousePos.y <= pos.y + size.y) ||
                (mousePos.x >= pos.x + SIDEBAR_WIDTH && mousePos.x <= pos.x + size.x &&
                    mousePos.y >= pos.y && mousePos.y <= pos.y + HEADER_HEIGHT);

            if (inDragArea && ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !ImGui::IsAnyItemHovered()) {
                g_AnimState.is_dragging = true;
                g_AnimState.drag_offset = ImVec2(mousePos.x - pos.x, mousePos.y - pos.y);
            }
            if (!ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
                g_AnimState.is_dragging = false;
            }
            if (g_AnimState.is_dragging) {
                ImVec2 newPos = ImVec2(mousePos.x - g_AnimState.drag_offset.x, mousePos.y - g_AnimState.drag_offset.y);
                ImGuiViewport* viewport = ImGui::GetMainViewport();
                float minX = viewport->Pos.x;
                float minY = viewport->Pos.y;
                float maxX = viewport->Pos.x + viewport->Size.x - size.x;
                float maxY = viewport->Pos.y + viewport->Size.y - size.y;
                newPos.x = Clamp(newPos.x, minX, maxX);
                newPos.y = Clamp(newPos.y, minY, maxY);
                g_WindowPos = newPos;
            }

            float gradPhase = 0.5f + 0.5f * std::sinf(pulse_time * GRAD_PHASE_FREQ);
            ImVec4 gradStart4 = ImVec4(
                Clamp(tc.x + GRAD_LIFT + GRAD_VARIANCE * gradPhase, 0.0f, 1.0f),
                Clamp(tc.y + GRAD_Y_OFFSET * gradPhase, 0.0f, 1.0f),
                Clamp(tc.z + GRAD_ACCENT_LIFT * gradPhase, 0.0f, 1.0f),
                MenuAlpha);
            ImVec4 gradEnd4 = ImVec4(
                Clamp(tc.x * (GRAD_BASE_SCALE + GRAD_SCALE_VARIANCE * gradPhase), 0.0f, 1.0f),
                Clamp(tc.y * (GRAD_BASE_SCALE + GRAD_SCALE_VARIANCE * gradPhase), 0.0f, 1.0f),
                Clamp(tc.z * (GRAD_ACCENT_SCALE + GRAD_VARIANCE * gradPhase), 0.0f, 1.0f),
                MenuAlpha);
            ImU32 gradientStart = ImGui::ColorConvertFloat4ToU32(gradStart4);
            ImU32 gradientEnd = ImGui::ColorConvertFloat4ToU32(gradEnd4);

            draw->AddRectFilled(pos, ImVec2(pos.x + size.x, pos.y + size.y), BgPrimary(MenuAlpha), WINDOW_ROUNDING, ImDrawFlags_RoundCornersAll);

            float glowPulse = 0.85f + 0.15f * std::sinf(pulse_time * 2.0f);
            ImU32 glowCol = ImGui::ColorConvertFloat4ToU32(ImVec4(tc.x * glowPulse * 1.3f, tc.y * glowPulse * 1.3f, tc.z * glowPulse * 1.3f, 0.7f * MenuAlpha));
            bg_draw->AddShadowRect(ImVec2(pos.x + 8, pos.y + 8), ImVec2(pos.x + size.x - 8, pos.y + size.y - 8), glowCol, 80.0f, ImVec2(0, 4), ImDrawFlags_RoundCornersAll, WINDOW_ROUNDING);

            draw->AddRect(pos, ImVec2(pos.x + size.x, pos.y + size.y), BorderColor(MenuAlpha * 0.4f), WINDOW_ROUNDING, ImDrawFlags_RoundCornersAll, 1.0f);

            draw->AddRectFilled(
                ImVec2(pos.x + WINDOW_ROUNDING * 0.5f, pos.y),
                ImVec2(pos.x + size.x - WINDOW_ROUNDING * 0.5f, pos.y + 2.0f),
                BorderColor(MenuAlpha * 0.3f)
            );

            // Sidebar
            const float sidebarPad = 8.f;
            ImGui::SetCursorPos({ 0, 0 });
            ImGui::BeginChild("##Sidebar", ImVec2(SIDEBAR_WIDTH, size.y), false, ImGuiWindowFlags_NoScrollbar);
            {
                auto nav_draw = ImGui::GetWindowDrawList();
                auto nav_pos = ImGui::GetCursorScreenPos();

                nav_draw->AddRectFilled(nav_pos, ImVec2(nav_pos.x + SIDEBAR_WIDTH, nav_pos.y + size.y), BgSecondary(MenuAlpha), WINDOW_ROUNDING, ImDrawFlags_RoundCornersLeft);

                ImGui::SetCursorPos({ sidebarPad, sidebarPad + 4 - 11 });
                {
                    const char* logo_text = "NOVA";
                    ImGui::PushFont(Fonts::Large);
                    ImGui::SetWindowFontScale(1.8f);
                    ImVec2 logo_size = ImGui::CalcTextSize(logo_text);
                    float logo_center_x = (SIDEBAR_WIDTH - logo_size.x) * 0.5f;
                    ImVec2 logo_pos = ImVec2(nav_pos.x + logo_center_x, nav_pos.y + sidebarPad + 4 - 11);

                    ImGui::RenderGradientText(logo_text, logo_pos, gradientStart, gradientEnd);
                    ImGui::SetWindowFontScale(1.0f);
                    ImGui::PopFont();
                }

                ImGui::SetCursorPos({ sidebarPad, 43 });
                ImVec2 divStart = ImGui::GetCursorScreenPos();
                nav_draw->AddLine(divStart, ImVec2(divStart.x + SIDEBAR_WIDTH - sidebarPad * 2, divStart.y), BorderColor(MenuAlpha * 0.5f), 1.0f);

                ImGui::SetCursorPos({ sidebarPad, 51 });
                ImGui::BeginChild("##NavContent", ImVec2(SIDEBAR_WIDTH - sidebarPad * 2, size.y - 100), false, ImGuiWindowFlags_NoScrollbar);
                {
                    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4.f, 3.f));

                    ImGui::CategoryHeader("WEBSOCKET", ImGui::ThemeColor, MenuAlpha);
                    if (ImGui::SubCategoryButton(ICON_FA_BOLT " Basic", sub_cat == S_BASIC && main_cat == C_WEBSOCKET, ImGui::ThemeColor)) { main_cat = C_WEBSOCKET; sub_cat = S_BASIC; }
                    if (ImGui::SubCategoryButton(ICON_FA_USERS " Clan", sub_cat == S_CLAN && main_cat == C_WEBSOCKET, ImGui::ThemeColor)) { main_cat = C_WEBSOCKET; sub_cat = S_CLAN; }

                    ImGui::Dummy(ImVec2(0, 8));
                    ImGui::CategoryHeader("SETTINGS", ImGui::ThemeColor, MenuAlpha);
                    if (ImGui::SubCategoryButton(ICON_FA_GEAR " Misc", sub_cat == S_MISC && main_cat == C_SETTINGS, ImGui::ThemeColor)) { main_cat = C_SETTINGS; sub_cat = S_MISC; }
                    if (ImGui::SubCategoryButton(ICON_FA_FLASK " Testing", sub_cat == S_TEST && main_cat == C_SETTINGS, ImGui::ThemeColor)) { main_cat = C_SETTINGS; sub_cat = S_TEST; }

                    ImGui::PopStyleVar();
                }
                ImGui::EndChild();

                ImGui::SetCursorPos({ sidebarPad + 5, size.y - 50 + 8 });
                {
                    auto user_pos = ImGui::GetCursorScreenPos();
                    float avatarRadius = 14.0f;
                    ImVec2 avatarCenter(user_pos.x + avatarRadius + 2, user_pos.y + avatarRadius + 4);

                    ImU32 avatarBg = ImGui::ColorConvertFloat4ToU32(ImVec4(tc.x * 0.25f, tc.y * 0.25f, tc.z * 0.25f, MenuAlpha));
                    nav_draw->AddCircleFilled(avatarCenter, avatarRadius, avatarBg, 20);
                    nav_draw->AddCircle(avatarCenter, avatarRadius, ImGui::ColorConvertFloat4ToU32(ImVec4(tc.x, tc.y, tc.z, 0.4f * MenuAlpha)), 20, 1.0f);

                    ImGui::PushFont(Fonts::FontAwesome);
                    const char* userIcon = ICON_FA_USER;
                    ImVec2 iconSize = ImGui::CalcTextSize(userIcon);
                    nav_draw->AddText(Fonts::FontAwesome, 12.0f, ImVec2(avatarCenter.x - iconSize.x * 0.35f, avatarCenter.y - iconSize.y * 0.35f), TextPrimary(MenuAlpha), userIcon);
                    ImGui::PopFont();

                    ImVec2 textPos(user_pos.x + avatarRadius * 2 + 8, user_pos.y + 2);
                    nav_draw->AddText(textPos, TextPrimary(MenuAlpha), stdvoid::username.c_str());
                    nav_draw->AddText(ImVec2(textPos.x, textPos.y + 14), TextSecondary(MenuAlpha), stdvoid::subscriptionType.c_str());
                }
            }
            ImGui::EndChild();

            // Content Area
            ImGui::SetCursorPos(ImVec2(SIDEBAR_WIDTH, 0));
            ImGui::BeginChild("##Content", ImVec2(size.x - SIDEBAR_WIDTH, size.y), false, ImGuiWindowFlags_NoBackground);
            {
                const float contentPad = 10.f;
                auto content_draw = ImGui::GetWindowDrawList();
                auto content_pos = ImGui::GetCursorScreenPos();
                float contentWidth = ImGui::GetContentRegionAvail().x;

                content_draw->AddRectFilled(content_pos, ImVec2(content_pos.x + contentWidth, content_pos.y + HEADER_HEIGHT), BgTertiary(MenuAlpha), WINDOW_ROUNDING, ImDrawFlags_RoundCornersTopRight);

                {
                    const float px = contentPad;

                    std::string mainLabel = GetMainCategoryLabel(main_cat);
                    std::string subLabel = (main_cat == C_WEBSOCKET) ? GetWebsocketSubLabel(static_cast<WebsocketSubCategory>(sub_cat)) : GetSettingsSubLabel(static_cast<SettingsSubCategory>(sub_cat));
                    std::string sectionLine = mainLabel + " / " + subLabel;

                    ImGui::PushFont(Fonts::Large);

                    ImVec2 titleSize = ImGui::CalcTextSize(sectionLine.c_str());
                    ImVec2 titlePos(content_pos.x + px, content_pos.y + (HEADER_HEIGHT - titleSize.y) * 0.5f - 1);

                    ImGui::RenderGradientText(sectionLine.c_str(), titlePos, gradientStart, gradientEnd);

                    ImGui::PopFont();
                }

                const float contentStartY = HEADER_HEIGHT + contentPad - g_AnimState.content_offset_y;
                ImGui::SetCursorPos(ImVec2(contentPad, contentStartY));

                const float availWidth = contentWidth - contentPad * 2;
                const float childGap = 8.f;
                const float colWidth = (availWidth - childGap) / 2.f;

                float contentVisibility = contentProgress * MenuAlpha;
                ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * contentVisibility);

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
                            if (ImGui::BeginCustomChild("Currency", ImVec2(colWidth, 160)))
                            {
                                float w = ImGui::GetContentRegionAvail().x;
                                float halfW = (w - ImGui::GetStyle().ItemSpacing.x) / 2.f;

                                ImGui::Combo(OBF("Currencies"), &Variables::Websocket::m_iCurrencyType,
                                    Lists::Websocket::m_ccCurrencies.data(), Lists::Websocket::m_ccCurrencies.size(), 4);
                                ImGui::Text(OBF("Amount"));
                                ImGui::CustomInputInt(OBF("##currencyAmt"), "", &Variables::Websocket::m_iCurrencyAmount, 0, 0, ImGuiInputFlags_None, w);
                                if (ImGui::CustomButton(OBF("##addCur"), OBF("Add"), halfW, 26)) { Variables::Websocket::m_bAddCurrency = true; }
                                ImGui::SameLine();
                                if (ImGui::CustomButton(OBF("##spendCur"), OBF("Spend"), halfW, 26)) { Variables::Websocket::m_bSpendCurrency = true; }
                            }
                            ImGui::EndCustomChild();
                        }
                        ImGui::EndGroup();

                        ImGui::SameLine(0, childGap);

                        ImGui::BeginGroup();
                        {
                            if (ImGui::BeginCustomChild("Consumables", ImVec2(colWidth, 160)))
                            {
                                float w = ImGui::GetContentRegionAvail().x;

                                ImGui::Combo(OBF("Consumable"), &Variables::Websocket::m_iConsumableType,
                                    Lists::Websocket::m_ccConsumables.data(), Lists::Websocket::m_ccConsumables.size(), 4);
                                ImGui::Text(OBF("Amount"));
                                ImGui::CustomInputInt(OBF("##consumableAmt"), "", &Variables::Websocket::m_iConsumableAmount, 0, 0, ImGuiInputFlags_None, w);
                                if (ImGui::CustomButton(OBF("##addConsumable"), OBF("Add Consumable"), w, 26)) { Variables::Websocket::m_bAddConsumable = true; }
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
                            if (ImGui::BeginCustomChild("Clan Features", ImVec2(availWidth, 120)))
                            {
                                ImGui::TextWrapped("Clan features coming soon...");
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
                        ImGui::BeginGroup();
                        {
                            if (ImGui::BeginCustomChild("Miscellaneous", ImVec2(availWidth, 120)))
                            {
                                ImGui::TextWrapped("Miscellaneous settings coming soon...");
                            }
                            ImGui::EndCustomChild();
                        }
                        ImGui::EndGroup();
                    }
                    break;
                    case S_TEST:
                    {
                        ImGui::BeginGroup();
                        {
                            if (ImGui::BeginCustomChild("Test Features", ImVec2(availWidth, 120)))
                            {
                                ImGui::TextWrapped("Testing features coming soon...");
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

                ImGui::PopStyleVar();
            }
            ImGui::EndChild();
        }
        ImGui::End();
        ImGui::PopStyleVar(3);
        ImGui::PopStyleColor();
    }

    if (OpenProgress > 0.01f) {
        auto bg_draw_list = ImGui::GetBackgroundDrawList();
        const float time = ImGui::GetTime();
        static bool initialized = false;
        static float wind_time = 0.0f;

        static std::mt19937 rng{ std::random_device{}() };
        static std::uniform_real_distribution<float> dist01{ 0.0f, 1.0f };

        const auto screenW = ImGui::GetIO().DisplaySize.x;
        const auto screenH = ImGui::GetIO().DisplaySize.y;

        if (!initialized || snow_particles.empty()) {
            snow_particles.clear();
            snow_particles.reserve(SNOW_PARTICLE_COUNT);
            for (int i = 0; i < SNOW_PARTICLE_COUNT; ++i) {
                SnowParticle flake;
                flake.pos = ImVec2(dist01(rng) * screenW, dist01(rng) * screenH);
                flake.depth = 0.2f + dist01(rng) * 0.8f;
                const float size_roll = dist01(rng);
                flake.size = (size_roll < 0.6f) ? 1.2f + dist01(rng) * 1.3f
                    : (size_roll < 0.85f) ? 2.8f + dist01(rng) * 1.2f
                    : 4.2f + dist01(rng) * 1.5f;
                flake.speed = 35.0f + flake.size * 10.0f + dist01(rng) * 25.0f;
                flake.sway_speed = 0.4f + dist01(rng) * 1.0f;
                flake.sway_offset = dist01(rng) * 6.283185f;
                flake.wind_resistance = 0.3f + dist01(rng) * 0.7f;
                flake.rotation = dist01(rng) * 6.283185f;
                flake.rotation_speed = (dist01(rng) - 0.5f) * 2.0f;
                snow_particles.push_back(flake);
            }
            initialized = true;
        }

        wind_time += dt;
        const float base_wind = 20.0f;
        const float gust_1 = std::sinf(wind_time * 0.3f) * 15.0f;
        const float gust_2 = std::sinf(wind_time * 0.7f) * 10.0f;
        float total_wind = base_wind + gust_1 + gust_2;
        const float mega_gust = std::sinf(wind_time * 0.15f) > 0.7f ? std::sinf(wind_time * 2.0f) * 12.0f : 0.0f;
        total_wind += mega_gust;

        for (auto& flake : snow_particles) {
            flake.pos.y += flake.speed * dt;
            flake.pos.x += total_wind * flake.wind_resistance * flake.depth * dt;
            flake.pos.x += std::sinf(time * flake.sway_speed + flake.sway_offset) * 10.0f * dt;
            flake.rotation += flake.rotation_speed * dt;

            if (mega_gust > 5.0f)
                flake.pos.y -= mega_gust * 0.25f * flake.wind_resistance * dt;

            if (flake.pos.y > screenH + 10) { flake.pos.y = -10; flake.pos.x = dist01(rng) * screenW; }
            if (flake.pos.x > screenW + 10) { flake.pos.x = -10; flake.pos.y = dist01(rng) * screenH; }
            if (flake.pos.x < -10) { flake.pos.x = screenW + 10; flake.pos.y = dist01(rng) * screenH; }

            const float flicker = 0.85f + std::sinf(time * 2.5f + flake.sway_offset) * 0.15f;
            const float opacity = (0.5f + flake.depth * 0.4f) * flicker;
            const ImU32 snow_color = IM_COL32(255, 255, 255, static_cast<int>(opacity * 255 * MenuAlpha));

            if (flake.size >= 4.2f)
                ImGui::DrawDetailedSnowflake(bg_draw_list, flake.pos, flake.size, flake.rotation, ImGui::ColorConvertU32ToFloat4(snow_color), 2);
            else if (flake.size >= 2.8f)
                ImGui::DrawDetailedSnowflake(bg_draw_list, flake.pos, flake.size, flake.rotation, ImGui::ColorConvertU32ToFloat4(snow_color), 1);
            else
                bg_draw_list->AddCircleFilled(flake.pos, flake.size, snow_color, 6);
        }
    }

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 6.f);
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(18.f / 255.f, 18.f / 255.f, 22.f / 255.f, 0.94f));
    ImGui::RenderNotifications();
    ImGui::PopStyleVar(1);
    ImGui::PopStyleColor(1);

    ImGui::EndFrame();
    ImGui::Render();

    context->OMSetRenderTargets(1, &targetview, nullptr);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}
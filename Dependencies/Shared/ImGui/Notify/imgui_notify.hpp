#pragma once

#include "../imgui.hpp"
#include "../imgui_internal.hpp"
#include "../../../../Cheat/Menu/Font/FontAwesome.hpp"
#include "../../../../Cheat/Menu/Font/Byte.hpp"

#include <string>
#include <vector>
#include <chrono>
#include <functional>
#include <cstdarg>

constexpr int NOTIFY_MAX_MSG_LENGTH = 4096;
constexpr float NOTIFY_PADDING_X = 20.f;
constexpr float NOTIFY_PADDING_Y = 20.f;
constexpr float NOTIFY_PADDING_MESSAGE_Y = 10.f;
constexpr int NOTIFY_FADE_IN_OUT_TIME = 150;
constexpr int NOTIFY_DEFAULT_DISMISS = 3000;
constexpr float NOTIFY_BASE_ALPHA = 0.8f;
constexpr bool NOTIFY_USE_SEPARATOR = true;
constexpr bool NOTIFY_USE_DISMISS_BUTTON = false;
constexpr int NOTIFY_RENDER_LIMIT = 5;
constexpr bool NOTIFY_RENDER_OUTSIDE_MAIN_WINDOW = true;
constexpr float NOTIFY_ACCENT_BAR_WIDTH = 5.f;

constexpr ImGuiWindowFlags NOTIFY_DEFAULT_TOAST_FLAGS =
ImGuiWindowFlags_AlwaysAutoResize |
ImGuiWindowFlags_NoDecoration |
ImGuiWindowFlags_NoNav |
ImGuiWindowFlags_NoBringToFrontOnFocus |
ImGuiWindowFlags_NoFocusOnAppearing;

#define NOTIFY_NULL_OR_EMPTY(str) (!str || !strlen(str))
#define NOTIFY_FORMAT(fn, format, ...) if(format) { va_list args; va_start(args, format); fn(format, args); va_end(args); }

enum class ImGuiToastType : uint8_t {
    None, Success, Warning, Error, Info
};

enum class ImGuiToastPhase : uint8_t {
    FadeIn, Wait, FadeOut, Expired
};

class ImGuiToast {
private:
    ImGuiWindowFlags flags = NOTIFY_DEFAULT_TOAST_FLAGS;
    ImGuiToastType type = ImGuiToastType::None;
    std::string title;
    std::string content;
    int dismissTime = NOTIFY_DEFAULT_DISMISS;
    std::chrono::steady_clock::time_point creationTime = std::chrono::steady_clock::now();
    std::function<void()> onButtonPress;
    std::string buttonLabel;

    void setText(std::string& target, const char* format, va_list args) {
        char buffer[NOTIFY_MAX_MSG_LENGTH];
        vsnprintf(buffer, sizeof(buffer), format, args);
        target = buffer;
    }

public:
    ImGuiToast(ImGuiToastType type, int dismissTime = NOTIFY_DEFAULT_DISMISS)
        : type(type), dismissTime(dismissTime) {
    }

    ImGuiToast(ImGuiToastType type, const char* format, ...) : ImGuiToast(type) {
        NOTIFY_FORMAT([this](const char* fmt, va_list args) { setText(content, fmt, args); }, format);
    }

    ImGuiToast(ImGuiToastType type, int dismissTime, const char* format, ...) : ImGuiToast(type, dismissTime) {
        NOTIFY_FORMAT([this](const char* fmt, va_list args) { setText(content, fmt, args); }, format);
    }

    ImGuiToast(ImGuiToastType type, int dismissTime, const char* buttonLabel, std::function<void()> onButtonPress, const char* format, ...)
        : ImGuiToast(type, dismissTime) {
        NOTIFY_FORMAT([this](const char* fmt, va_list args) { setText(content, fmt, args); }, format);
        this->onButtonPress = std::move(onButtonPress);
        this->buttonLabel = buttonLabel;
    }

    void setTitle(const char* format, ...) {
        NOTIFY_FORMAT([this](const char* fmt, va_list args) { setText(title, fmt, args); }, format);
    }

    void setContent(const char* format, ...) {
        NOTIFY_FORMAT([this](const char* fmt, va_list args) { setText(content, fmt, args); }, format);
    }

    void setType(ImGuiToastType t) { type = t; }
    void setWindowFlags(ImGuiWindowFlags f) { flags = f; }
    void setOnButtonPress(std::function<void()> f) { onButtonPress = std::move(f); }
    void setButtonLabel(const char* label) { buttonLabel = label; }

    const char* getTitle() const { return title.c_str(); }
    const char* getDefaultTitle() const {
        if (!title.empty()) return title.c_str();
        switch ((int)type) {
        case (int)ImGuiToastType::Success: return "Success";
        case (int)ImGuiToastType::Warning: return "Warning";
        case (int)ImGuiToastType::Error: return "Error";
        case (int)ImGuiToastType::Info: return "Info";
        default: return nullptr;
        }
    }

    ImGuiToastType getType() const { return type; }

    ImVec4 getColor() const {
        switch ((int)type) {
        case (int)ImGuiToastType::Success: return { 0.1f, 0.8f, 0.2f, 1.0f };
        case (int)ImGuiToastType::Warning: return { 1.0f, 0.8f, 0.0f, 1.0f };
        case (int)ImGuiToastType::Error: return { 0.9f, 0.2f, 0.2f, 1.0f };
        case (int)ImGuiToastType::Info: return { 0.2f, 0.6f, 1.0f, 1.0f };
        default: return { 1.0f,1.0f,1.0f,1.0f };
        }
    }

    const char* getIcon() const {
        switch ((int)type) {
        case (int)ImGuiToastType::Success: return ICON_FA_CIRCLE_CHECK;
        case (int)ImGuiToastType::Warning: return ICON_FA_TRIANGLE_EXCLAMATION;
        case (int)ImGuiToastType::Error: return ICON_FA_CIRCLE_EXCLAMATION;
        case (int)ImGuiToastType::Info: return ICON_FA_CIRCLE_INFO;
        default: return nullptr;
        }
    }

    const char* getContent() const { return content.c_str(); }

    std::chrono::milliseconds getElapsedTime() const {
        return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - creationTime);
    }

    ImGuiToastPhase getPhase() const {
        auto elapsed = getElapsedTime().count();
        if (elapsed > NOTIFY_FADE_IN_OUT_TIME + dismissTime + NOTIFY_FADE_IN_OUT_TIME)
            return ImGuiToastPhase::Expired;
        if (elapsed > NOTIFY_FADE_IN_OUT_TIME + dismissTime)
            return ImGuiToastPhase::FadeOut;
        if (elapsed > NOTIFY_FADE_IN_OUT_TIME)
            return ImGuiToastPhase::Wait;
        return ImGuiToastPhase::FadeIn;
    }

    float getFadePercent() const {
        auto phase = getPhase();
        auto elapsed = getElapsedTime().count();
        if (phase == ImGuiToastPhase::FadeIn)
            return float(elapsed) / float(NOTIFY_FADE_IN_OUT_TIME);
        if (phase == ImGuiToastPhase::FadeOut)
            return 1.f - float(elapsed - NOTIFY_FADE_IN_OUT_TIME - dismissTime) / float(NOTIFY_FADE_IN_OUT_TIME);
        return 1.f;
    }

    ImGuiWindowFlags getWindowFlags() const { return flags; }
    const std::function<void()>& getOnButtonPress() const { return onButtonPress; }
    const char* getButtonLabel() const { return buttonLabel.c_str(); }
};

namespace ImGui {

    inline std::vector<ImGuiToast> notifications;

    inline void InsertNotification(const ImGuiToast& toast) {
        notifications.push_back(toast);
    }

    inline void RemoveNotification(size_t index) {
        if (index < notifications.size())
            notifications.erase(notifications.begin() + index);
    }

    inline void RenderNotifications() {
        ImGui::PushFont(Fonts::FontAwesome);
        float height = 0.f;

        for (size_t i = 0; i < notifications.size(); ++i) {
            auto& toast = notifications[i];

            if (toast.getPhase() == ImGuiToastPhase::Expired) {
                RemoveNotification(i);
                --i;
                continue;
            }

#if NOTIFY_RENDER_LIMIT > 0
            if (i >= NOTIFY_RENDER_LIMIT) break;
#endif

            const float opacity = toast.getFadePercent();
            ImVec4 typeColor = toast.getColor(); typeColor.w = opacity;

            char windowName[50];
#ifdef _WIN32
            sprintf_s(windowName, "##TOAST%zu", i);
#else
            snprintf(windowName, sizeof(windowName), "##TOAST%zu", i);
#endif

            ImVec2 pos = GetMainViewport()->WorkPos;
            ImVec2 size = GetMainViewport()->WorkSize;
            SetNextWindowPos(ImVec2(pos.x + size.x - NOTIFY_PADDING_X, pos.y + size.y - NOTIFY_PADDING_Y - height), ImGuiCond_Always, ImVec2(1.f, 1.f));
            SetNextWindowBgAlpha(NOTIFY_BASE_ALPHA * opacity);

            PushStyleVar(ImGuiStyleVar_WindowRounding, 5.f);
            PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.f);
            Begin(windowName, nullptr, toast.getWindowFlags());
            {
                ImDrawList* drawList = GetWindowDrawList();
                ImVec2 winPos = GetWindowPos();
                ImVec2 winSize = GetWindowSize();
                drawList->AddRectFilled(winPos, ImVec2(winPos.x + NOTIFY_ACCENT_BAR_WIDTH, winPos.y + winSize.y), GetColorU32(typeColor), GetStyle().WindowRounding, ImDrawFlags_RoundCornersLeft);

                const float textPadding = NOTIFY_ACCENT_BAR_WIDTH + 15.f;
                SetCursorPosX(textPadding);
                PushTextWrapPos(GetMainViewport()->Size.x / 4.f);

                const char* icon = toast.getIcon();
                const char* title = toast.getTitle();
                const char* defaultTitle = toast.getDefaultTitle();
                const char* content = toast.getContent();

                bool hasTitle = !NOTIFY_NULL_OR_EMPTY(title) || !NOTIFY_NULL_OR_EMPTY(defaultTitle);

                if (hasTitle) {
                    if (!NOTIFY_NULL_OR_EMPTY(icon)) {
                        PushStyleColor(ImGuiCol_Text, typeColor);
                        TextUnformatted(icon);
                        PopStyleColor();
                        SameLine();
                    }
                    PushStyleColor(ImGuiCol_Text, { 1.f,1.f,1.f,opacity });
                    TextUnformatted(defaultTitle ? defaultTitle : title);
                    PopStyleColor();
                }

                if (NOTIFY_USE_SEPARATOR && hasTitle && !NOTIFY_NULL_OR_EMPTY(content)) {
                    Separator();
                }

                if (!NOTIFY_NULL_OR_EMPTY(content)) {
                    if (!hasTitle || !NOTIFY_USE_SEPARATOR) SetCursorPosX(textPadding);
                    PushStyleColor(ImGuiCol_Text, { 1.f,1.f,1.f,opacity });
                    TextUnformatted(content);
                    PopStyleColor();
                }

                auto onPress = toast.getOnButtonPress();
                if (onPress && ImGui::Button(toast.getButtonLabel())) {
                    onPress();
                }

                PopTextWrapPos();
            }
            height += GetWindowHeight() + NOTIFY_PADDING_MESSAGE_Y;
            End();
            PopStyleVar(2);
        }
        PopFont();
    }
}

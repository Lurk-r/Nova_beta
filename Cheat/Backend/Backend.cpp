#include "Backend.hpp"
#include "../Menu/Menu.hpp"
#include "../Menu/Snowflake.hpp"
#include "../Utils/Utils.hpp"
#include "../Utils/Hooks.hpp"

const HWND Backend::MAIN_WINDOW = FindWindowA(nullptr, "Pixel Gun 3D");

Backend::presentVariable originalPresent;
Backend::presentVariable hookedPresent;

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

static bool init = false;
Backend Features;

bool Backend::DirectXPresentHook()
{
    SwapChainDescription = {};
    SwapChainDescription.BufferCount = 2;
    SwapChainDescription.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    SwapChainDescription.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    SwapChainDescription.OutputWindow = GetForegroundWindow();
    SwapChainDescription.SampleDesc.Count = 1;
    SwapChainDescription.Windowed = TRUE;
    SwapChainDescription.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    if (FAILED(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, FeatureLevels, 2, D3D11_SDK_VERSION, &SwapChainDescription, &SwapChain, &Device, nullptr, nullptr)))
        return false;

    void** DX11Vtable = *reinterpret_cast<void***>(SwapChain);
    hookedPresent = reinterpret_cast<Backend::presentVariable>(DX11Vtable[8]);

    return true;
}

static LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    ImGuiIO& io = ImGui::GetIO();
    Features.CursorFix(Features.OpenMenu);
    io.MouseDrawCursor = Features.OpenMenu;

    if (Features.OpenMenu)
    {
        // Pass event to ImGui
        ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);

        // FIX: Only block INPUT messages. 
        // Previously, returning 'true' for everything blocked System messages (Paint, Resize, etc), causing the freeze.
        switch (uMsg)
        {
        case WM_MOUSEMOVE:
        case WM_LBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_RBUTTONDOWN:
        case WM_RBUTTONUP:
        case WM_MBUTTONDOWN:
        case WM_MBUTTONUP:
        case WM_MOUSEWHEEL:
        case WM_XBUTTONDOWN:
        case WM_XBUTTONUP:
        case WM_KEYDOWN:
        case WM_KEYUP:
        case WM_SYSKEYDOWN:
        case WM_SYSKEYUP:
        case WM_CHAR:
        case WM_DEVICECHANGE:
            return true; // Block input from reaching the game
        }
        // Let other messages (WM_PAINT, WM_ACTIVATE, etc.) pass through!
    }

    return CallWindowProc(Features.originalWndProc, hWnd, uMsg, wParam, lParam);
}

void Backend::LoadImGui(HWND window, ID3D11Device* device, ID3D11DeviceContext* context)
{
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
    ImGui_ImplWin32_Init(window);
    ImGui_ImplDX11_Init(device, context);
    InitializeNovaFonts();
    ImGui::StyleColorsDark();

    // Load Fonts
    ImFontConfig font_config;
    font_config.PixelSnapH = false;
    font_config.OversampleH = 8;
    font_config.OversampleV = 8;
    font_config.RasterizerMultiply = 1.2f;

    static const ImWchar ranges[] =
    {
        0x0020, 0x00FF, // Basic Latin + Latin Supplement
        0x0400, 0x052F, // Cyrillic + Cyrillic Supplement
        0x2DE0, 0x2DFF, // Cyrillic Extended-A
        0xA640, 0xA69F, // Cyrillic Extended-B
        0xE000, 0xE226, // icons
        0,
    };

    font_config.GlyphRanges = ranges;
    Fonts::Medium = io.Fonts->AddFontFromMemoryTTF(StolzlFont, sizeof(StolzlFont), 15.0f, &font_config, ranges);
    static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_16_FA, 0 };
    ImFontConfig icons_config; icons_config.MergeMode = true; icons_config.PixelSnapH = true; icons_config.OversampleH = 8; icons_config.OversampleV = 8;

    Fonts::FontAwesome = io.Fonts->AddFontFromMemoryCompressedTTF(fa6_solid_compressed_data, fa6_solid_compressed_size, 16.0f, &icons_config, icons_ranges);
    Fonts::Large = io.Fonts->AddFontFromMemoryTTF(StolzlFont, sizeof(StolzlFont), 23.0f, &font_config, ranges);
    Fonts::Bold = io.Fonts->AddFontFromMemoryTTF(StolzlBold, sizeof(StolzlBold), 17.0f, &font_config, ranges);
    Fonts::ArrowFont = io.Fonts->AddFontFromMemoryTTF(ArrowFont, sizeof(ArrowFont), 17.0f, &font_config, ranges);

}

static long PresentHook(IDXGISwapChain* pointerSwapChain, UINT sync, UINT flags)
{
    if (!init && SUCCEEDED(pointerSwapChain->GetDevice(__uuidof(ID3D11Device), reinterpret_cast<void**>(&Features.Device))))
    {
        Features.Device->GetImmediateContext(&Features.PointerContext);
        pointerSwapChain->GetDesc(&Features.PresentHookSwapChain);
        Features.Window = Features.PresentHookSwapChain.OutputWindow;

        ID3D11Texture2D* backBuffer = nullptr;
        if (SUCCEEDED(pointerSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(&backBuffer))))
        {
            Features.Device->CreateRenderTargetView(backBuffer, nullptr, &Features.MainRenderTargetView);
            backBuffer->Release();
        }

        Features.LoadImGui(Features.Window, Features.Device, Features.PointerContext);
        Features.originalWndProc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(Features.Window, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WndProc)));
        init = true;
    }

    if (Utils::keyPressed(OPEN_MENU_KEY) && Utils::isGameWindowActive(Backend::MAIN_WINDOW))
        Features.OpenMenu = !Features.OpenMenu;

    if (init)
        Features.DrawImGui(Features.PointerContext, Features.MainRenderTargetView);

    return originalPresent(pointerSwapChain, sync, flags);
}

bool Backend::Load()
{
    Features.DirectXPresentHook();
    InitializeMinHook();
    AttachHook(hookedPresent, PresentHook, &originalPresent);
    return true;
}

void Backend::CursorFix(bool menuOpen)
{
    static bool lastState = false;
    if (menuOpen != lastState) {
        ShowCursor(menuOpen ? TRUE : FALSE);
        ClipCursor(NULL);
        lastState = menuOpen;
    }
}
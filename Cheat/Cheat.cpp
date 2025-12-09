#include "Cheat.hpp"

#include "Backend/Backend.hpp"
#include "Utils/Hooks.hpp"
#include "Utils/Il2CppHelper.hpp"

#include "Game/Data/Indexes.hpp"
#include "Game/Data/Patterns.hpp"
#include "Game/Data/Structs.hpp"

#include "Game/Pointers/Pointers.hpp"

#include "Game/Global.hpp"
#include "Game/WebSocket.hpp"

namespace Cheat
{
    void Init()
    {
        // Init
        IL2CPP::INIT();
        Logger::Info("Initializing hooks...");

        // Patterns
        Patterns::Init();

        // Pointers
        Pointers::Init();

        // Hooks
        Global::PixelTime::Init();
        WebSocket::Socket::Init();

        Logger::Info("Hooks initialized!");
        ImGui::InsertNotification({ ImGuiToastType::Info, 12000, "Nigger Loaded, press RCTRL to open/close menu" });
    }
}
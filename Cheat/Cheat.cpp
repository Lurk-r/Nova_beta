#include "Cheat.hpp"

#include "Backend/Backend.hpp"
#include "Utils/Hooks.hpp"
#include "Utils/Il2CppHelper.hpp"

#include "Game/Data/Indexes.hpp"
#include "Game/Data/Patterns.hpp"
#include "Game/Data/Structs.hpp"

#include "Game/Pointers/Pointers.hpp"

#include "Game/Login.hpp"
#include "Game/Extras.hpp"
#include "Game/Gameplay.hpp"
#include "Game/Global.hpp"
#include "Game/WebSocket.hpp"

#include <filesystem>
#include <fstream>
#include <Windows.h>

namespace Cheat
{
   

    void Init()
    {
        // Init
        IL2CPP::INIT();
        Logger::Info("Initializing hooks...");
        Patterns::Init();
        Pointers::Init();

#pragma region HOOKS

        // Extras 
        Login::Init();
        Extras::Init();

        // Global
        Global::PixelTime::Init();

        // Websocket
        WebSocket::Socket::Init();

        // Gameplay
        Gameplay::Player_move_c::Init();
        Gameplay::WeaponSounds::Init();
        Gameplay::System_String::Init();
#pragma endregion

        Logger::Info("Hooks initialized!");
        ImGui::InsertNotification({ ImGuiToastType::Info, 12000, "Nova Loaded, press RCTRL to open/close menu" });
    }
}
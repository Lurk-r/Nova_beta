#include "Global.hpp"

#include "WebSocket.hpp"
#include "Websocket/Commands.hpp"

#include "../Utils/Il2CppHelper.hpp"
#include "../Variables/Variables.hpp"

#include <ImGui/Notify/imgui_notify.hpp>
#include <Obfusheader.hpp>

namespace Global
{
	namespace PixelTime
	{
		void(*PixelTime_o)(IL2CPP::Object*) = nullptr;
		void PixelTime(IL2CPP::Object* instance)
		{
			if (instance)
			{
				if (Variables::Websocket::m_bReloadSocket)
				{
					WebSocket::Socket::SendCommand(Commands::Reload());
					ImGui::InsertNotification({ ImGuiToastType::Info, 3000, OBF("Reloading socket...") });
					Variables::Websocket::m_bReloadSocket = false;
				}
			}

			return PixelTime_o(instance);
		}

		void Init()
		{
			HookMethod("PixelTime", "Update", PixelTime, (LPVOID*)&PixelTime_o);
		}
	}
}
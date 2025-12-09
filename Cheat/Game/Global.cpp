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
		void(*Update_o)(IL2CPP::Object*) = nullptr;
		void Update(IL2CPP::Object* instance)
		{
			if (instance)
			{
				// Reload
				if (Variables::Websocket::m_bReloadSocket)
				{
					WebSocket::Socket::SendCommand(Commands::Reload());
					ImGui::InsertNotification({ ImGuiToastType::Info, 3000, OBF("Reloading socket...") });
					Variables::Websocket::m_bReloadSocket = false;
				}

				// Adders
				if (Variables::Websocket::m_bAddCurrency)
				{
					WebSocket::Socket::SendNotifCommand(OBF("Add Currency"), Commands::Currency());
					Variables::Websocket::m_bAddCurrency = false;
				}

				if (Variables::Websocket::m_bSpendCurrency)
				{
					WebSocket::Socket::SendNotifCommand(OBF("Spend Currency"), Commands::Currency(true));
					Variables::Websocket::m_bSpendCurrency = false;
				}
			}

			Update_o(instance);
		}

		void Init()
		{
			HookMethod("PixelTime", "Update", Update, (LPVOID*)&Update_o);
		}
	}
}
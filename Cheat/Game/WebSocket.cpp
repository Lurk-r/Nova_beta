#include "WebSocket.hpp"

#include <string_view>
#include <wtypes.h>
#include <iostream>

#include "Data/Structs.hpp"
#include "Pointers/Pointers.hpp"
#include "Websocket/WSHelpers.hpp"
#include "../Utils/Il2CppHelper.hpp"

#include <Obfusheader.hpp>
#include <ImGui/Notify/imgui_notify.hpp>

namespace WebSocket
{
    namespace Socket
    {
        static void LogSocket(std::string_view eventName, std::string_view args, bool isSend)
        {
            HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

            SetConsoleTextAttribute(hConsole, isSend ? FOREGROUND_BLUE : 14);
            std::cout << "[ SOCKET ] " << eventName << (isSend ? " -> " : " <- ");

            SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY);
            std::cout << args << '\n';
        }

        static IL2CPP::Array<IL2CPP::Object*>* CreateCommand(const json& data)
        {
            IL2CPP::Array<IL2CPP::Object*>* socketCommand = IL2CPP::Array<IL2CPP::Object*>::Create(IL2CPP::DefaultTypeClass::Object, 1);
            socketCommand->GetVectorPointer()[0] = Pointers::Json::Decode(IL2CPP::String::Create(data.dump().c_str()));
            return socketCommand;
        }

        void SendCommand(json data, const std::function<void(Structs::Socket::Response)>& OnRequestReceived, bool ForceExecute)
        {
            if (!data.is_array()) return;
            if (!Pointers::SocketInstance)
            {
                ImGui::InsertNotification({ ImGuiToastType::Warning, 3000, (OBF("Socket is not active!")) });
                return;
            }

            int req_id = WSFunctions::Random(1000, 10000);
            data[1][OBF("req_id")] = req_id;

            Structs::Socket::Response ResponseData;
            ResponseData.request = data[1];
            if (OnRequestReceived != nullptr) ResponseData.OnRequestReceived = OnRequestReceived;
            ResponseData.ForceExecute = ForceExecute;

            Structs::Socket::Responses.emplace_back(ResponseData);
            Emit(Pointers::SocketInstance, IL2CPP::String::Create(data[0].get<std::string>().c_str()), CreateCommand(data[1]));
        }

        void SendNotifCommand(const std::string& requestName, const json& command, bool autoReload)
        {
            ImGui::InsertNotification({ ImGuiToastType::Info, 3000, (OBF("Sent Request: ") + requestName).c_str() });
            SendCommand(command, [requestName, autoReload](Structs::Socket::Response packet)
            {
                ImGuiToastType toastType = (packet.ResponseStatus == OBF("OK")) ? ImGuiToastType::Success : ImGuiToastType::Error;
                if (packet.ResponseStatus == OBF("OK") && autoReload)
                {
                    
                }
                const std::string message = OBF("Received Request: ") + requestName + OBF(" | Status: ") + packet.ResponseStatus;
                ImGui::InsertNotification({ toastType, 3000, message.c_str() });
            });
        }

        void (*Emit_o)(IL2CPP::Object*, IL2CPP::String*, IL2CPP::Array<IL2CPP::Object*>*) = nullptr;
        void Emit(IL2CPP::Object* instance, IL2CPP::String* eventName, IL2CPP::Array<IL2CPP::Object*>* args)
        {
            if (instance)
            {
                Pointers::SocketInstance = instance; // we get this always since the instance changes when we switch characters

                const std::string eventStr = eventName->ToString();
                json encodedArgs = json::parse(Pointers::Json::Encode((IL2CPP::Object*)args)->ToString());
                json& data = encodedArgs[0];

                LogSocket(eventStr, data.dump(), true);
                return Emit_o(instance, eventName, CreateCommand(data));
            }

            Emit_o(instance, eventName, args);
        }

        void (*OnPacket_o)(IL2CPP::Object*, Structs::Socket::Packet*) = nullptr;
        void OnPacket(IL2CPP::Object* instance, Structs::Socket::Packet* packet)
        {
            OnPacket_o(instance, packet);

            if (packet->EventName && packet->IsDecoded)
            {
                const std::string eventName = packet->EventName->ToString();
                const std::string decodedArgs = Pointers::Json::Encode(packet->DecodedArgs)->ToString();
                json Data = json::parse(decodedArgs);

                // On Request Received
                int req_id = Data[0][OBF("req_id")].is_null() ? -1 : Data[0][OBF("req_id")].get<int>();
                if (req_id != -1)
                {
                    auto it = std::find_if(Structs::Socket::Responses.begin(), Structs::Socket::Responses.end(),
                        [req_id](const Structs::Socket::Response& response) { return response.request[OBF("req_id")].get<int>() == req_id; });

                    if (it != Structs::Socket::Responses.end())
                    {
                        Structs::Socket::Response& data = *it;
                        data.response = Data;
                        json status = Data[0][OBF("status")];
                        std::string fail = OBF("FAIL"), ok = OBF("OK");
                        data.ResponseStatus = ok;

                        if (!status.is_null() && status.get<std::string>() != OBF("ok"))
                        {
                            data.ResponseStatus = fail;
                            if (!data.ForceExecute) data.OnRequestReceived = nullptr;
                        }

                        if (data.OnRequestReceived != nullptr) data.OnRequestReceived(data);
                        Structs::Socket::Responses.erase(it);
                    }
                }

                LogSocket(eventName, Data[0].dump(), false);
            }
        }

        void Init()
        {
            HookMethod("BestHTTP.SocketIO.Socket", "Emit", Emit, (LPVOID*)&Emit_o);
            HookMethod("BestHTTP.SocketIO.Socket", "BestHTTP.SocketIO.ISocket.OnPacket", OnPacket, (LPVOID*)&OnPacket_o);
        }
    }
}

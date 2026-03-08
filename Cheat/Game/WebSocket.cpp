#include "WebSocket.hpp"

#include <string_view>
#include <wtypes.h>
#include <iostream>

#include "Data/Structs.hpp"
#include "Pointers/Pointers.hpp"
#include "Websocket/WSHelpers.hpp"
#include "../Utils/Il2CppHelper.hpp"
#include "../Variables/Variables.hpp"

#include <Obfusheader.hpp>
#include <ImGui/Notify/imgui_notify.hpp>

namespace WebSocket
{
    namespace Socket
    {
        static bool s_InSendCommand = false;

        static void LogSocket(std::string_view ev, std::string_view ag, bool tx, bool isCustom)
        {
            static std::map<int, std::pair<std::string, std::string>> queue;
            HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

            bool is_reload_cmd = (ev == "update_progress" && ag.find("\"p\":") == std::string::npos);

            if (isCustom && !is_reload_cmd && !Variables::Websocket::m_bDebugMode) return;

            auto print = [&](std::string_view e, std::string_view a, bool s, bool err, bool is_reload, bool is_custom_normal) {
                if (is_reload)
                {
                    SetConsoleTextAttribute(h, 14);
                    std::printf("%-9s[ %.*s | %.*s ]\n", s ? "RELOAD >>" : "RELOAD <<", (int)e.size(), e.data(), (int)a.size(), a.data());
                }
                else if (is_custom_normal)
                {
                    SetConsoleTextAttribute(h, 10);
                    std::printf("%-9s[ %.*s | %.*s ]\n", s ? "CUSTOM >>" : "CUSTOM <<", (int)e.size(), e.data(), (int)a.size(), a.data());
                }
                else
                {
                    if (err) {
                        SetConsoleTextAttribute(h, 12);
                        std::printf("%-9s[ %.*s | %.*s ]\n", s ? "SEND >>" : "RECV <<", (int)e.size(), e.data(), (int)a.size(), a.data());
                    }
                    else {
                        SetConsoleTextAttribute(h, s ? 10 : 13); std::printf("%-9s", s ? "SEND >>" : "RECV <<");
                        SetConsoleTextAttribute(h, 8);           std::printf("[ ");
                        SetConsoleTextAttribute(h, 11);          std::printf("%.*s", (int)e.size(), e.data());
                        SetConsoleTextAttribute(h, 8);           std::printf(" | ");
                        SetConsoleTextAttribute(h, 7);           std::printf("%.*s", (int)a.size(), a.data());
                        SetConsoleTextAttribute(h, 8);           std::printf(" ]\n");
                    }
                }
                SetConsoleTextAttribute(h, 7);
                };

            size_t p = ag.find("\"req_id\":");
            int id = (p != std::string_view::npos) ? std::atoi(ag.data() + p + 9) : -1;

            bool is_custom_normal = isCustom && !is_reload_cmd;

            if (tx && id != -1) {
                queue[id] = { std::string(ev), std::string(ag) };
            }
            else if (!tx && id != -1 && queue.count(id)) {
                bool was_reload = (queue[id].first == "update_progress" && queue[id].second.find("\"p\":") == std::string::npos);
                bool fail = ag.find("\"status\":\"fail\"") != std::string_view::npos;

                print(queue[id].first, queue[id].second, true, fail, was_reload, isCustom && !was_reload);
                print(ev, ag, false, fail, was_reload, isCustom && !was_reload);

                queue.erase(id);
            }
            else {
                print(ev, ag, tx, false, is_reload_cmd, is_custom_normal);
            }
        }

        static IL2CPP::Array<IL2CPP::Object*>* CreateCommand(const json& data)
        {
            IL2CPP::Array<IL2CPP::Object*>* socketCommand = IL2CPP::Array<IL2CPP::Object*>::Create(IL2CPP::DefaultTypeClass::Object, 1);
            socketCommand->GetVectorPointer()[0] = Pointers::Json::Decode(IL2CPP::String::Create(data.dump().c_str()));
            return socketCommand;
        }

        void (*Emit_o)(IL2CPP::Object*, IL2CPP::String*, IL2CPP::Array<IL2CPP::Object*>*) = nullptr;
        void Emit(IL2CPP::Object* instance, IL2CPP::String* eventName, IL2CPP::Array<IL2CPP::Object*>* args)
        {
            if (instance)
            {
                Pointers::SocketInstance = instance;

                const std::string eventStr = eventName->ToString();
                json encodedArgs = json::parse(Pointers::Json::Encode((IL2CPP::Object*)args)->ToString());
                json& data = encodedArgs[0];

                LogSocket(eventStr, data.dump(), true, s_InSendCommand);
                return Emit_o(instance, eventName, CreateCommand(data));
            }

            Emit_o(instance, eventName, args);
        }

        void SendCommand(json data, const std::function<void(Structs::Socket::Response)>& OnRequestReceived, bool ForceExecute)
        {
            if (data.is_null() || !data.is_array() || data.size() < 2 || !data[0].is_string() || !data[1].is_object())
            {
                ImGui::InsertNotification({ ImGuiToastType::Error, 3000, OBF("Json Invalid!") });
                return;
            }

            if (!Pointers::SocketInstance)
            {
                ImGui::InsertNotification({ ImGuiToastType::Warning, 3000, (OBF("Socket is not active!")) });
                return;
            }

            try
            {
                const int req_id = WSFunctions::Random(1000, 100000);
                data[1][OBF("req_id")] = req_id;

                Structs::Socket::Response ResponseData;
                ResponseData.request = data[1];
                if (OnRequestReceived != nullptr) ResponseData.OnRequestReceived = OnRequestReceived;
                ResponseData.ForceExecute = ForceExecute;

                Structs::Socket::Responses.emplace_back(ResponseData);

                s_InSendCommand = true;
                Emit(Pointers::SocketInstance, IL2CPP::String::Create(data[0].get<std::string>().c_str()), CreateCommand(data[1]));
                s_InSendCommand = false;
            }
            catch (...)
            {
                ImGui::InsertNotification({ ImGuiToastType::Error, 3000, OBF("Json Invalid!") });
            }
        }
        // Reload Json
        json reload()
        {
            return json::array({ OBF("update_progress"), json::object() });
        }
        // Reload Game (I KNOW this is so shittily done but its like my first time doing things without ai so if u are reading this source somehow just LEAVE ME ALONEEE
        void SendReload()
        {
            SendCommand(reload(), nullptr, false);
        }

        void SendNotifCommand(const std::string& requestName, const json& command, bool autoReload)
        {
            ImGui::InsertNotification({ ImGuiToastType::Info, 3000, (OBF("Sent Request: ") + requestName).c_str() });
            SendCommand(command, [requestName, autoReload](Structs::Socket::Response packet)
            {
                    ImGuiToastType toastType = (packet.ResponseStatus == OBF("OK")) ? ImGuiToastType::Success : ImGuiToastType::Error;
                    const std::string message = OBF("Received Request: ") + requestName + OBF(" | Status: ") + packet.ResponseStatus;
                    ImGui::InsertNotification({ toastType, 3000, message.c_str() });

                    if (packet.ResponseStatus == OBF("OK") && autoReload)
                    {
                        Variables::Miscellaneous::m_bForceSync = true;
                    }
            }, true);
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
                bool isCustom = false;

                int req_id = Data[0][OBF("req_id")].is_null() ? -1 : Data[0][OBF("req_id")].get<int>();
                if (req_id != -1)
                {
                    auto it = std::find_if(Structs::Socket::Responses.begin(), Structs::Socket::Responses.end(),
                        [req_id](const Structs::Socket::Response& response) { return response.request[OBF("req_id")].get<int>() == req_id; });

                    if (it != Structs::Socket::Responses.end())
                    {
                        isCustom = true;
                        Structs::Socket::Response data = *it;
                        Structs::Socket::Responses.erase(it);

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
                    }
                }

                LogSocket(eventName, Data[0].dump(), false, isCustom);
            }
        }

        void Init()
        {
            MakeAHookHerePls("BestHTTP.SocketIO.Socket", "Emit", Emit, (LPVOID*)&Emit_o);
            MakeAHookHerePls("BestHTTP.SocketIO.Socket", "BestHTTP.SocketIO.ISocket.OnPacket", OnPacket, (LPVOID*)&OnPacket_o);
        }
    }
}
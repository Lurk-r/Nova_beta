#pragma once
#include <json.hpp>
#include "Data/Structs.hpp"

using json = nlohmann::ordered_json;

namespace WebSocket
{
    namespace Socket
    {
        void SendCommand(const json& data, const std::function<void(Structs::Socket::Response)>& OnRequestReceived = nullptr, bool ForceExecute = true);
        void SendNotifCommand(const std::string& requestName, const json& command, bool autoReload = true);

        extern void (*Emit_o)(IL2CPP::Object*, IL2CPP::String*, IL2CPP::Array<IL2CPP::Object*>*);
        void Emit(IL2CPP::Object*, IL2CPP::String*, IL2CPP::Array<IL2CPP::Object*>*);

        extern void (*OnPacket_o)(IL2CPP::Object*, Structs::Socket::Packet*);
        void OnPacket(IL2CPP::Object*, Structs::Socket::Packet*);

        void Init();
    }
}
#include "SlotsHelper.hpp"
#include "../WebSocket.hpp"
#include <string>
#include <Obfusheader.hpp>
#include "../../Variables/Variables.hpp"
#define OBFS(x) std::string(OBF(x))

namespace SlotsHelper
{
    json MyParsedSlots = json::object();
    json OtherParsedSlots = json::object();

    bool EndsWith(const std::string& fullString, const std::string& ending)
    {
        if (fullString.length() >= ending.length())
        {
            return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
        }
        return false;
    }

    void ParseProgress(const json& root, bool isMine)
    {
        json& Target = isMine ? MyParsedSlots : OtherParsedSlots;
        Target.clear();

        if (!root.contains(OBFS("slots"))) return;
        const json& slots = root[OBFS("slots")];

        try
        {
            if (slots.contains(OBFS("version_slot")) && slots[OBFS("version_slot")].contains(OBFS("11")))
            {
                Target[OBFS("version")] = slots[OBFS("version_slot")][OBFS("11")];
            }

            if (slots.contains(OBFS("1")) && slots[OBFS("1")].contains(OBFS("n")))
            {
                Target[OBFS("name")] = slots[OBFS("1")][OBFS("n")];
            }

            if (slots.contains(OBFS("3")))
            {
                Target[OBFS("currencies")] = slots[OBFS("3")];
            }

            if (slots.contains(OBFS("4")))
            {
                Target[OBFS("level")] = slots[OBFS("4")];
            }

            if (slots.contains(OBFS("6")) && slots[OBFS("6")].is_array())
            {
                Target[OBFS("lobbyitems")] = json::array();
                for (const auto& item : slots[OBFS("6")])
                {
                    if (item.contains(OBFS("i")))
                    {
                        Target[OBFS("lobbyitems")].push_back(item[OBFS("i")]);
                    }
                }
            }

            if (slots.contains(OBFS("12")))
            {
                Target[OBFS("skins")] = json::array();
                const auto& slot12 = slots[OBFS("12")];
                for (auto it = slot12.items().begin(); it != slot12.items().end(); ++it)
                {
                    const std::string& key = it.key();
                    const auto& val = it.value();

                    if (key == "-1")
                    {
                        if (val.contains(OBFS("c")))
                            Target[OBFS("cape")] = val[OBFS("c")];
                    }
                    else
                    {
                        json skinObj;
                        if (val.contains(OBFS("n")))
                            skinObj[OBFS("name")] = val[OBFS("n")];
                        if (val.contains(OBFS("c")))
                            skinObj[OBFS("data")] = val[OBFS("c")];
                        Target[OBFS("skins")].push_back(skinObj);
                    }
                }
            }

            if (slots.contains(OBFS("13")) && slots[OBFS("13")].contains(OBFS("w")))
            {
                Target[OBFS("wear")] = json::array();
                const auto& wearArray = slots[OBFS("13")][OBFS("w")];
                for (const auto& item : wearArray)
                {
                    if (item.is_number())
                    {
                        Target[OBFS("wear")].push_back(item.get<int>());
                    }
                }
            }

            if (slots.contains(OBFS("16")))
            {
                Target[OBFS("gadgets")] = json::object();
                const auto& slot16 = slots[OBFS("16")];
                for (auto it = slot16.items().begin(); it != slot16.items().end(); ++it)
                {
                    const std::string& key = it.key();
                    const auto& val = it.value();
                    Target[OBFS("gadgets")][key] = val;
                }
            }

            if (slots.contains(OBFS("25")) && slots[OBFS("25")].contains(OBFS("s")))
            {
                Target[OBFS("weaponskins")] = json::array();
                const auto& slot25s = slots[OBFS("25")][OBFS("s")];
                for (auto it = slot25s.items().begin(); it != slot25s.items().end(); ++it)
                {
                    const std::string& index = it.key();
                    if (EndsWith(index, "014"))
                    {
                        Target[OBFS("weaponskins")].push_back(std::stoi(index));
                    }
                }
            }

            if (slots.contains(OBFS("25")) && slots[OBFS("25")].contains(OBFS("v")))
            {
                Target[OBFS("vehicles")] = json::object();
                const auto& slot25v = slots[OBFS("25")][OBFS("v")];
                for (auto it = slot25v.items().begin(); it != slot25v.items().end(); ++it)
                {
                    const std::string& key = it.key();
                    const auto& val = it.value();
                    Target[OBFS("vehicles")][key] = val;
                }
            }

            if (slots.contains(OBFS("28")) && slots[OBFS("28")].contains(OBFS("0")))
            {
                Target[OBFS("weapons")] = json::object();
                const auto& slot28_0 = slots[OBFS("28")][OBFS("0")];
                for (auto it = slot28_0.items().begin(); it != slot28_0.items().end(); ++it)
                {
                    const std::string& key = it.key();
                    const auto& val = it.value();

                    json weaponData;
                    if (val.contains(OBFS("l")))
                        weaponData[OBFS("l")] = val[OBFS("l")];
                    if (val.contains(OBFS("r")))
                        weaponData[OBFS("r")] = val[OBFS("r")];
                    Target[OBFS("weapons")][key] = weaponData;
                }
            }

            if (slots.contains(OBFS("37")))
            {
                Target[OBFS("modules")] = json::object();
                const auto& slot37 = slots[OBFS("37")];
                for (auto it = slot37.items().begin(); it != slot37.items().end(); ++it)
                {
                    const std::string& key = it.key();
                    const auto& val = it.value();
                    Target[OBFS("modules")][key] = val;
                }
            }

            if (slots.contains(OBFS("43")) && slots[OBFS("43")].contains(OBFS("0")))
            {
                Target[OBFS("armor")] = json::object();
                const auto& slot43_0 = slots[OBFS("43")][OBFS("0")];
                for (auto it = slot43_0.items().begin(); it != slot43_0.items().end(); ++it)
                {
                    const std::string& key = it.key();
                    const auto& val = it.value();
                    Target[OBFS("armor")][key] = val;
                }
            }

            if (slots.contains(OBFS("53")) && slots[OBFS("53")].contains(OBFS("p")))
            {
                Target[OBFS("pets")] = json::array();
                const auto& petsArray = slots[OBFS("53")][OBFS("p")];
                for (const auto& pet : petsArray)
                {
                    if (pet.contains(OBFS("i")))
                    {
                        Target[OBFS("pets")].push_back(pet[OBFS("i")].get<int>());
                    }
                }
            }

            if (slots.contains(OBFS("56")) && slots[OBFS("56")].contains(OBFS("t")))
            {
                Target[OBFS("tutorial")] = slots[OBFS("56")][OBFS("t")];
            }
        }
        catch (...) {}
    }

    void CheckAndParseAllSlots(std::string id)
    {
        json cmd = json::array({ OBFS("get_progress"), {{OBFS("player_id"), id}} });
        WebSocket::Socket::SendCommand(cmd, [id](Structs::Socket::Response packet) {
            if (packet.ResponseStatus == OBFS("OK") && !packet.response.empty())
            {
                ParseProgress(packet.response[0], id == Variables::Miscellaneous::m_sPlayerID);
            }
            }, true);
    }

    void CheckAndParseAllSlots(std::string id, std::function<void()> onComplete)
    {
        json cmd = json::array({ OBFS("get_progress"), {{OBFS("player_id"), id}} });
        WebSocket::Socket::SendCommand(cmd, [id, onComplete](Structs::Socket::Response packet) {
            if (packet.ResponseStatus == OBFS("OK") && !packet.response.empty())
            {
                ParseProgress(packet.response[0], id == Variables::Miscellaneous::m_sPlayerID);
                onComplete();
            }
            }, true);
    }

    json GetParsedData(bool isMine, std::string key)
    {
        json& Target = isMine ? MyParsedSlots : OtherParsedSlots;
        if (Target.contains(key))
            return Target[key];
        return json();
    }

    bool HasPet(int index)
    {
        json pets = GetParsedData(true, OBFS("pets"));
        if (pets.is_array())
        {
            for (const auto& pet : pets)
            {
                if (pet.is_number() && pet.get<int>() == index)
                    return true;
            }
        }
        return false;
    }

    bool HasWear(int index)
    {
        json wear = GetParsedData(true, OBFS("wear"));
        if (wear.is_array())
        {
            for (const auto& item : wear)
            {
                if (item.is_number() && item.get<int>() == index)
                    return true;
            }
        }
        return false;
    }

    bool HasArmor(int index)
    {
        json armor = GetParsedData(true, OBFS("armor"));
        if (armor.is_object())
        {
            std::string key = std::to_string(index);
            return armor.contains(key);
        }
        return false;
    }

    bool HasGadget(int index)
    {
        json gadgets = GetParsedData(true, OBFS("gadgets"));
        if (gadgets.is_object())
        {
            std::string key = std::to_string(index);
            return gadgets.contains(key);
        }
        return false;
    }

    bool HasModule(int index)
    {
        json modules = GetParsedData(true, OBFS("modules"));
        if (modules.is_object())
        {
            std::string key = std::to_string(index);
            return modules.contains(key);
        }
        return false;
    }

    bool HasVehicle(int index)
    {
        json vehicles = GetParsedData(true, OBFS("vehicles"));
        if (vehicles.is_object())
        {
            std::string key = std::to_string(index);
            return vehicles.contains(key);
        }
        return false;
    }

    int GetModuleLevel(int index)
    {
        json modules = GetParsedData(true, OBFS("modules"));
        if (modules.is_object())
        {
            std::string key = std::to_string(index);
            if (modules.contains(key) && modules[key].is_number())
            {
                return modules[key].get<int>();
            }
        }
        return 0;
    }

    int GetGadgetLevel(int index)
    {
        json gadgets = GetParsedData(true, OBFS("gadgets"));
        if (gadgets.is_object())
        {
            std::string key = std::to_string(index);
            if (gadgets.contains(key) && gadgets[key].is_number())
            {
                return gadgets[key].get<int>();
            }
        }
        return 0;
    }

    int GetArmorLevel(int index)
    {
        json armor = GetParsedData(true, OBFS("armor"));
        if (armor.is_object())
        {
            std::string key = std::to_string(index);
            if (armor.contains(key) && armor[key].is_number())
            {
                return armor[key].get<int>();
            }
        }
        return 0;
    }

    int GetVehicleLevel(int index)
    {
        json vehicles = GetParsedData(true, OBFS("vehicles"));
        if (vehicles.is_object())
        {
            std::string key = std::to_string(index);
            if (vehicles.contains(key) && vehicles[key].is_number())
            {
                return vehicles[key].get<int>();
            }
        }
        return 0;
    }

    std::vector<int> GetOwnedPets()
    {
        std::vector<int> result;
        json pets = GetParsedData(true, OBFS("pets"));
        if (pets.is_array())
        {
            for (const auto& pet : pets)
            {
                if (pet.is_number())
                    result.push_back(pet.get<int>());
            }
        }
        return result;
    }

    std::vector<int> GetOwnedWear()
    {
        std::vector<int> result;
        json wear = GetParsedData(true, OBFS("wear"));
        if (wear.is_array())
        {
            for (const auto& item : wear)
            {
                if (item.is_number())
                    result.push_back(item.get<int>());
            }
        }
        return result;
    }

    std::vector<int> GetOwnedArmor()
    {
        std::vector<int> result;
        json armor = GetParsedData(true, OBFS("armor"));
        if (armor.is_object())
        {
            for (auto it = armor.items().begin(); it != armor.items().end(); ++it)
            {
                try
                {
                    result.push_back(std::stoi(it.key()));
                }
                catch (...) {}
            }
        }
        return result;
    }

    std::vector<int> GetOwnedGadgets()
    {
        std::vector<int> result;
        json gadgets = GetParsedData(true, OBFS("gadgets"));
        if (gadgets.is_object())
        {
            for (auto it = gadgets.items().begin(); it != gadgets.items().end(); ++it)
            {
                try
                {
                    result.push_back(std::stoi(it.key()));
                }
                catch (...) {}
            }
        }
        return result;
    }

    std::vector<int> GetOwnedModules()
    {
        std::vector<int> result;
        json modules = GetParsedData(true, OBFS("modules"));
        if (modules.is_object())
        {
            for (auto it = modules.items().begin(); it != modules.items().end(); ++it)
            {
                try
                {
                    result.push_back(std::stoi(it.key()));
                }
                catch (...) {}
            }
        }
        return result;
    }

    std::vector<int> GetOwnedVehicles()
    {
        std::vector<int> result;
        json vehicles = GetParsedData(true, OBFS("vehicles"));
        if (vehicles.is_object())
        {
            for (auto it = vehicles.items().begin(); it != vehicles.items().end(); ++it)
            {
                try
                {
                    result.push_back(std::stoi(it.key()));
                }
                catch (...) {}
            }
        }
        return result;
    }
}
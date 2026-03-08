#pragma once
#include <json.hpp>
#include <string>
#include <vector>
#include <functional>

using json = nlohmann::ordered_json;

namespace SlotsHelper
{
    extern json MyParsedSlots;
    extern json OtherParsedSlots;

    void ParseProgress(const json& root, bool isMine);
    void CheckAndParseAllSlots(std::string id);
    void CheckAndParseAllSlots(std::string id, std::function<void()> onComplete);
    json GetParsedData(bool isMine, std::string key);

    bool HasPet(int index);
    bool HasWear(int index);
    bool HasArmor(int index);
    bool HasGadget(int index);
    bool HasModule(int index);
    bool HasVehicle(int index);

    int GetModuleLevel(int index);
    int GetGadgetLevel(int index);
    int GetArmorLevel(int index);
    int GetVehicleLevel(int index);

    std::vector<int> GetOwnedPets();
    std::vector<int> GetOwnedWear();
    std::vector<int> GetOwnedArmor();
    std::vector<int> GetOwnedGadgets();
    std::vector<int> GetOwnedModules();
    std::vector<int> GetOwnedVehicles();
}
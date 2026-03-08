#pragma once
#include "WSHelpers.hpp"
#include "../Data/Lists.hpp"
#include "../../Variables/Variables.hpp"
#include <json.hpp>
#include <set>
std::string RandomDigits(int hi);
std::string RandomHex(int hi);

using json = nlohmann::ordered_json;

namespace Commands
{
    json Reload();

    json Currency(bool spend = false);
    json AddConsumable(CommandsID CommandID, int consumable, int type);

    json AddWeapon(int index, int level, int rarity);
    json RemoveWeapon(int index);
    json AddWeaponParts(int index, int amount);
    json AddAllWeapons(const std::vector<int>& indices, int level, int rarity, bool useDefaultRarity);
    json RemoveAllWeapons(const std::vector<int>& indices);
    json AddAllWeaponParts(const std::vector<int>& indices, int amount);

    json CheckAllMySlots(int myid);
    json CheckAllOtherPlayerSlots(int playerid);

    json SetName(std::string name);
    json SetSkins(std::string id, std::string skin);
    json SetCape(std::string skin);

    json AddArmor(int index, int level);
    json AddWear(int index);
    json AddAvatar(int index);
    json AddGadget(int index, int level);
    json RemoveGadget(int index);
    json AddVehicle(int index, int level);
    json RemoveVehicle(int index);
    json AddTrail(int index);

    json AddPet(int index);
    json UpgradePet(int index, int level);
    json AddPetParts(int index, int amount);
    json RemovePetParts(int index, int amount);

    json UpgradeModule(int index);
    json AddModuleParts(int index, int amount);
    json RemoveModuleParts(int index, int amount);

    json AddItemSingle(int index);
    json RemoveItemSingle(int index);

    json AddFreeUpgrade(int index, int amount);
    json RemoveFreeUpgrade(int index, int amount);

    json SendFriendRequest(std::string targetId);
    json SendClanMessage(std::string message);

    json SetPlayerLevel(int targetLevel, int xp);
    json ClaimSafeRewards(const std::set<int>& alreadyClaimed = {});
}
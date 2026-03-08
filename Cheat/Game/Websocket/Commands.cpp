#include "Commands.hpp"
#include <Obfusheader.hpp>
#include "../Data/AdderLists.hpp"
#include "../Data/SlotsHelper.hpp"
#include "../WebSocket.hpp"

std::string RandomHex(int len)
{
    std::string chars = "0123456789abcdef";
    std::string res;
    for (int i = 0; i < len; ++i)
        res += chars[rand() % chars.length()];
    return res;
}

std::string RandomDigits(int len)
{
    std::string chars = "0123456789";
    std::string res;
    for (int i = 0; i < len; ++i)
        res += chars[rand() % chars.length()];
    return res;
}

namespace Commands
{
#pragma region MISC
    json Reload()
    {
        return json::array({ OBF("update_progress"), json::object() });
    }
#pragma endregion

#pragma region ACC_STUFF
    json SetName(std::string name)
    {
        json commands;
        commands.emplace_back(
            WSFunctions::CreateCommand(CommandsID::SetNick, { {"Nick", name} }));
        return WSFunctions::CreateSnapshot(commands);
    }

    json SetSkins(std::string id, std::string data)
    {
        json p;
        p["i"] = id;
        p["c"] = data;
        p["h"] = RandomHex(16);
        p["n"] = "leminare.dev";

        json cmd;
        cmd["id"] = 29;
        cmd["i"] = RandomHex(8);
        cmd["p"] = p;

        return json::array({ OBF("update_progress"), cmd });
    }

    json SetCape(std::string data)
    {
        json p;
        p["i"] = "-1";
        p["c"] = data;
        p["h"] = RandomHex(16);

        json cmd;
        cmd["id"] = 29;
        cmd["i"] = RandomHex(8);
        cmd["p"] = p;

        return json::array({ OBF("update_progress"), cmd });
    }

    json Currency(bool spend)
    {
        json commands;
        commands.emplace_back(
            WSFunctions::CreateCommand(spend ? CommandsID::SpendCurrency : CommandsID::AddCurrency, {
                {OBF("c"), Lists::Websocket::m_ccCurrencies[Variables::Websocket::m_iCurrencyType]},
                {OBF("v"), Variables::Websocket::m_iCurrencyAmount},
                {OBF("ca"), AddCurrencyCause::GameTask}
                }));

        return WSFunctions::CreateSnapshot(commands);
    }

    json AddConsumable(CommandsID CommandID, int consumable, int type)
    {
        json commands;
        commands.emplace_back(
            WSFunctions::CreateCommand(CommandID, {
                {OBF("c"), Variables::Websocket::m_iConsumableAmount},
                {OBF("i"), consumable},
                {OBF("t"), type}
                }));

        return WSFunctions::CreateSnapshot(commands);
    }
#pragma endregion

#pragma region WEAPONS
    json AddWeapon(int index, int level, int rarity)
    {
        json commands;
        commands.emplace_back(
            WSFunctions::CreateCommand(CommandsID::AddWeaponV17, {
                {OBF("i"), index},
                {OBF("u"), {
                    {OBF("l"), level},
                    {OBF("r"), rarity}
                }},
                {OBF("oc"), 9999}
                }));
        return WSFunctions::CreateSnapshot(commands);
    }

    json RemoveWeapon(int index)
    {
        json commands;
        commands.emplace_back(
            WSFunctions::CreateCommand(CommandsID::RemoveWeapon, {
                {OBF("i"), index}
                }));
        return WSFunctions::CreateSnapshot(commands);
    }

    json AddWeaponParts(int index, int amount)
    {
        json commands;
        commands.emplace_back(
            WSFunctions::CreateCommand(CommandsID::AddDetails, {
                {OBF("i"), index},
                {OBF("d"), amount}
                }));
        return WSFunctions::CreateSnapshot(commands);
    }

    json AddAllWeapons(const std::vector<int>& indices, int level, int rarity, bool useDefaultRarity)
    {
        json commands;
        for (int index : indices)
        {
            int finalRarity = rarity;
            if (useDefaultRarity)
            {
                const auto* weapon = AdderLists::FindByIndex(index);
                if (weapon)
                {
                    finalRarity = AdderLists::GetRarityValue(weapon->Rarity);
                }
            }
            commands.emplace_back(
                WSFunctions::CreateCommand(CommandsID::AddWeaponV17, {
                    {OBF("i"), index},
                    {OBF("u"), {
                        {OBF("l"), level},
                        {OBF("r"), finalRarity}
                    }},
                    {OBF("oc"), 9999}
                    }));
        }
        return WSFunctions::CreateSnapshot(commands);
    }

    json RemoveAllWeapons(const std::vector<int>& indices)
    {
        json commands;
        for (int index : indices)
        {
            commands.emplace_back(
                WSFunctions::CreateCommand(CommandsID::RemoveWeapon, {
                    {OBF("i"), index}
                    }));
        }
        return WSFunctions::CreateSnapshot(commands);
    }

    json AddAllWeaponParts(const std::vector<int>& indices, int amount)
    {
        json commands;
        for (int index : indices)
        {
            commands.emplace_back(
                WSFunctions::CreateCommand(CommandsID::AddDetails, {
                    {OBF("i"), index},
                    {OBF("d"), amount}
                    }));
        }
        return WSFunctions::CreateSnapshot(commands);
    }
#pragma endregion

#pragma region SLOTS
    json CheckAllMySlots(int myid)
    {
        return json::array({ "get_progress", {{"player_id", myid}, {"type", 1}} });
    }

    json CheckAllOtherPlayerSlots(int playerid)
    {
        return json::array({ "get_progress", {{"player_id", playerid}, {"type", 2}} });
    }
#pragma endregion

#pragma region ARMOR_AND_WEAR
    json AddArmor(int index, int level)
    {
        json commands;
        commands.emplace_back(
            WSFunctions::CreateCommand(CommandsID::BuyArmor, {
                {OBF("a"), index},
                {OBF("u"), level}
                }));
        return WSFunctions::CreateSnapshot(commands);
    }

    json AddWear(int index)
    {
        json commands;
        commands.emplace_back(
            WSFunctions::CreateCommand(CommandsID::AddPurchasedWear, {
                {OBF("w"), "a*" + std::to_string(index)}
                }));
        return WSFunctions::CreateSnapshot(commands);
    }

    json AddAvatar(int index)
    {
        json commands;
        commands.emplace_back(
            WSFunctions::CreateCommand(CommandsID::UpdateBattleRoyaleList, {
                {OBF("1"), index}
                }));
        return WSFunctions::CreateSnapshot(commands);
    }

    json AddGadget(int index, int level)
    {
        json commands;
        commands.emplace_back(
            WSFunctions::CreateCommand(CommandsID::AddPurhasedGadget, {
                {OBF("g"), index},
                {OBF("u"), level}
                }));
        return WSFunctions::CreateSnapshot(commands);
    }

    json RemoveGadget(int index)
    {
        json commands;
        commands.emplace_back(
            WSFunctions::CreateCommand(CommandsID::RemoveGadget, {
                {OBF("i"), index}
                }));
        return WSFunctions::CreateSnapshot(commands);
    }

    json AddVehicle(int index, int level)
    {
        json commands;
        commands.emplace_back(
            WSFunctions::CreateCommand(CommandsID::UpdateBRVehicle, {
                {OBF("i"), index},
                {OBF("u"), level}
                }));
        return WSFunctions::CreateSnapshot(commands);
    }

    json RemoveVehicle(int index)
    {
        json commands;
        commands.emplace_back(
            WSFunctions::CreateCommand(CommandsID::RemoveBRVehicle, {
                {OBF("i"), index}
                }));
        return WSFunctions::CreateSnapshot(commands);
    }

    json AddTrail(int index)
    {
        json commands;
        commands.emplace_back(
            WSFunctions::CreateCommand(CommandsID::UpdatePlayerEquipment, {
                {OBF("4"), {
                    {OBF("lb"), index}
                }}
                }));
        return WSFunctions::CreateSnapshot(commands);
    }
#pragma endregion

#pragma region PETS
    json AddPet(int index)
    {
        json commands;
        commands.emplace_back(
            WSFunctions::CreateCommand(CommandsID::AddPetV3, {
                {OBF("i"), index},
                {OBF("n"), "leminare.dev"}
                }));
        return WSFunctions::CreateSnapshot(commands);
    }

    json UpgradePet(int index, int level)
    {
        json commands;
        commands.emplace_back(
            WSFunctions::CreateCommand(CommandsID::UpgradePetV3, {
                {OBF("i"), index},
                {OBF("l"), level}
                }));
        return WSFunctions::CreateSnapshot(commands);
    }

    json AddPetParts(int index, int amount)
    {
        json commands;
        commands.emplace_back(
            WSFunctions::CreateCommand(CommandsID::AddPetCardsV3, {
                {OBF("i"), index},
                {OBF("a"), amount}
                }));
        return WSFunctions::CreateSnapshot(commands);
    }

    json RemovePetParts(int index, int amount)
    {
        json commands;
        commands.emplace_back(
            WSFunctions::CreateCommand(CommandsID::RemovePetCardsV3, {
                {OBF("i"), index},
                {OBF("a"), amount}
                }));
        return WSFunctions::CreateSnapshot(commands);
    }
#pragma endregion

#pragma region MODULES
    json UpgradeModule(int index)
    {
        json commands;
        commands.emplace_back(
            WSFunctions::CreateCommand(CommandsID::ModuleInfoIncreaseUp, {
                {OBF("i"), index}
                }));
        return WSFunctions::CreateSnapshot(commands);
    }

    json AddModuleParts(int index, int amount)
    {
        json commands;
        commands.emplace_back(
            WSFunctions::CreateCommand(CommandsID::InventoryAddItemConsumable, {
                {OBF("t"), 1155},
                {OBF("i"), index},
                {OBF("c"), amount}
                }));
        return WSFunctions::CreateSnapshot(commands);
    }

    json RemoveModuleParts(int index, int amount)
    {
        json commands;
        commands.emplace_back(
            WSFunctions::CreateCommand(CommandsID::InventoryRemoveItemConsumable, {
                {OBF("t"), 1155},
                {OBF("i"), index},
                {OBF("c"), amount}
                }));
        return WSFunctions::CreateSnapshot(commands);
    }
#pragma endregion

#pragma region ITEMS
    json AddItemSingle(int index)
    {
        json commands;
        commands.emplace_back(
            WSFunctions::CreateCommand(CommandsID::InventoryAddItemSingle, {
                {OBF("i"), index}
                }));
        return WSFunctions::CreateSnapshot(commands);
    }

    json RemoveItemSingle(int index)
    {
        json commands;
        commands.emplace_back(
            WSFunctions::CreateCommand(CommandsID::InventoryRemoveItemSingle, {
                {OBF("i"), index}
                }));
        return WSFunctions::CreateSnapshot(commands);
    }

    json AddFreeUpgrade(int index, int amount)
    {
        json commands;
        commands.emplace_back(
            WSFunctions::CreateCommand(CommandsID::InventoryAddItemConsumable, {
                {OBF("t"), 1380},
                {OBF("i"), index},
                {OBF("c"), amount}
                }));
        return WSFunctions::CreateSnapshot(commands);
    }

    json RemoveFreeUpgrade(int index, int amount)
    {
        json commands;
        commands.emplace_back(
            WSFunctions::CreateCommand(CommandsID::InventoryRemoveItemConsumable, {
                {OBF("t"), 1380},
                {OBF("i"), index},
                {OBF("c"), amount}
                }));
        return WSFunctions::CreateSnapshot(commands);
    }
#pragma endregion

#pragma region SOCIAL
    json SendFriendRequest(std::string targetId)
    {
        return json::array({ "friend_request", {
            {"whom", targetId},
            {"type", 0}
        } });
    }

    json SendClanMessage(std::string message)
    {
        return json::array({ "send_clan_message", {
            {"from", "69696969"},
            {"text", message},
            {"timeStamp", 1781287221},
            {"last_timeStamp", 0}
        } });
    }

    json SetPlayerLevel(int targetLevel, int xp)
    {
        if (targetLevel > 65) targetLevel = 65;
        if (targetLevel < 1) return WSFunctions::CreateSnapshot(json::array());

        json commands = json::array();

        for (int i = 1; i <= targetLevel; i++)
        {
            commands.emplace_back(
                WSFunctions::CreateCommand(CommandsID::UpdateLevel, {
                    {"l", i}
                    }, { 140, 6 })
            );
        }

        commands.emplace_back(
            WSFunctions::CreateCommand(CommandsID::UpdateExperience, {
                {"e", 0},
                {"ec", 4},
                {"ad", json::object()}
                }, { 140, 6 })
        );

        commands.emplace_back(
            WSFunctions::CreateCommand(CommandsID::AnalyticsProgress, {
                {"eid", 1043},
                {"params", {
                    {"ip1", 40},
                    {"ip2", 140},
                    {"ip3", 2040},
                    {"sp1", "MultiplayerMatchReward"},
                    {"jp1", {
                        {"v_ProgressRoad", 11},
                        {"v_Tutorial", 12}
                    }}
                }}
                }, { 140, 6 })
        );

        return WSFunctions::CreateSnapshot(commands);
    }

    json ClaimSafeRewards(const std::set<int>& alreadyClaimed)
    {
        json commands = json::array();

        const std::vector<std::pair<int, std::vector<std::string>>> rewards = {
            {395,   {"1015:140","2015:5"}},
            {515,   {"1015:140","2015:5"}},
            {640,   {"1015:150","2015:5"}},
            {770,   {"1015:130","2015:5"}},
            {1055,  {"1015:120","2015:5"}},
            {1210,  {"1015:110","2015:5"}},
            {1265,  {"1015:40"}},
            {1370,  {"1015:80","2015:5"}},
            {1540,  {"1015:90","2015:5"}},
            {1900,  {"1015:50","2015:5"}},
            {1965,  {"1015:50"}},
            {2095,  {"1015:30","2015:5"}},
            {2300,  {"1015:35","2015:5"}},
            {2515,  {"1015:40","2015:5"}},
            {2750,  {"1015:40","2015:10"}},
            {3000,  {"1015:45","2015:10"}},
            {3270,  {"1015:50","2015:10"}},
            {3560,  {"1015:50","2015:10"}},
            {3870,  {"1015:50","2015:10"}},
            {3980,  {"1015:50"}},
            {4195,  {"1015:50","2015:10"}},
            {4540,  {"1015:50","2015:10"}},
            {4905,  {"1015:50","2015:10"}},
            {5290,  {"1015:50","2015:10"}},
            {5690,  {"1015:50","2015:10"}},
            {6110,  {"1015:50","2015:10"}},
            {6550,  {"1015:50","2015:10"}},
            {7010,  {"1015:50","2015:10"}},
            {7485,  {"1015:55","2015:10"}},
            {7980,  {"1015:55","2015:10"}},
            {8495,  {"1015:55","2015:10"}},
            {9030,  {"1015:60","2015:10"}},
            {9580,  {"1015:60","2015:10"}},
            {10150, {"1015:60","2015:10"}},
            {10740, {"1015:65","2015:15"}},
            {11350, {"1015:65","2015:15"}},
            {11975, {"1015:65","2015:15"}},
            {12620, {"1015:70","2015:15"}},
            {13285, {"1015:70","2015:15"}},
            {13970, {"1015:70","2015:15"}},
            {14670, {"1015:75","2015:20"}},
            {15390, {"1015:75","2015:20"}},
            {16130, {"1015:75","2015:20"}},
            {16890, {"1015:75","2015:20"}},
            {17150, {"1015:50"}},
            {17665, {"1015:75","2015:20"}},
            {18460, {"1015:75","2015:20"}},
            {19275, {"1015:75","2015:20"}},
            {20110, {"1015:75","2015:20"}},
            {20960, {"1015:75","2015:20"}},
            {21830, {"1015:75","2015:20"}},
            {22130, {"1015:50"}},
            {22720, {"1015:75","2015:20"}},
            {23630, {"1015:80","2015:20"}},
            {24555, {"1015:80","2015:20"}},
            {25500, {"1015:80","2015:20"}},
            {25825, {"1015:50"}},
            {26465, {"1015:80","2015:20"}},
            {27450, {"1015:100","2015:30"}},
            {28450, {"1015:100","2015:30"}},
            {29470, {"1015:100","2015:30"}},
            {30510, {"1015:100","2015:30"}},
            {31570, {"1015:100","2015:30"}}
        };

        for (int i = 0; i < rewards.size(); i++)
        {
            int rp = rewards[i].first;
            if (alreadyClaimed.count(rp)) continue;

            const std::vector<std::string>& rr = rewards[i].second;
            json rrArray = json::array();
            for (const auto& r : rr) rrArray.emplace_back(r);

            commands.emplace_back(WSFunctions::CreateCommand(CommandsID::ProgressRoadSetTakenRewards,
                { {"rp", rp}, {"rr", rrArray} }, { 6, 405 }));
            commands.emplace_back(WSFunctions::CreateCommand(CommandsID::ProgressRoadUpdateRewardsProgress,
                { {"srp", rp}, {"tr", rrArray} }, { 6, 405 }));
        }

        return WSFunctions::CreateSnapshot(commands);
    }

    // TODO COMMANDS
    // unlock_characters {"count":9999} (unlocks extra characters)
    // create_character {} (creates character) 
#pragma endregion
}
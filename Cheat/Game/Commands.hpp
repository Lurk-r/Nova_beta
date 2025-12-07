#include "Data/Lists.hpp"
#include "Socket.hpp"
#include <Obfusheader.hpp>

using namespace Lists;

// Helpers
std::string RandHex(int hexLength = 4)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, 255);

    std::stringstream ss; for (int i = 0; i < hexLength; i++)
    {
        ss << std::hex << std::setw(2) << std::setfill('0') << distrib(gen);
    }
    
    return ss.str();
}

int Random(int min, int max)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(min, max);
    return distrib(gen);
}

std::string GetLocalDate()
{
    std::time_t ActualTime = std::time(nullptr);
    std::tm* LocalDate = std::localtime(&ActualTime);

    std::ostringstream oss;
    oss << (LocalDate->tm_mon + 1) << '-' << LocalDate->tm_mday << '-' << (LocalDate->tm_year + 1900);

    return oss.str();
}

json CreateCommand(const CommandsID& id, const json& params, const std::vector<int>& u = {0})
{
    return {{OBF("id"), id}, {OBF("ci"), RandHex()}, {OBF("p"), params}, {OBF("h"), json::object()}, {OBF("u"), u}};
}

json CreateSnapshot(const json& commands)
{
    return json::array({OBF("update_progress"), {{OBF("id"), CommandsID::Snapshot}, {OBF("i"), RandHex()}, {OBF("p"), {{OBF("c"), commands}}}}});
}

// Level Experiences
const int BlackMaketExp[7] = {0, 0, 900, 1800, 5400, 6300, 9375};

// Constants
namespace Constants
{
    const json EmptyCMD = CreateCommand(CommandsID::EmptyCommand, json::object());

    const std::unordered_map<int, std::pair<int, int>> ConsumableMapping = {
        {0, {1, 10}}, {1, {2, 10}}, {2, {3, 10}}, // Chests
        {3, {1030, 1520}}, {4, {2030, 1520}}, {5, {3030, 1520}}, {6, {4030, 1520}}, {7, {6030, 1520}}, // Boosters
        {8, {24015, 1450}}, // VIP
        {9, {17018, 1230}}, // War Hero Chest
        {10, {1024, 1380}}, {11, {2024, 1380}}, {12, {3024, 1380}}, {13, {4024, 1380}}, {14, {5024, 1380}}, {15, {6024, 1380}} // Free Upgrades
    };
    const std::map<int, std::pair<int, int>> FortDataMap = {
        {1, {0, 14}}, {2, {1, 124}}, {3, {2, 4159}}, {4, {3, 7869}}, {5, {4, 20239}},
        {6, {5, 40469}}, {7, {6, 80939}}, {8, {7, 170999}}, {9, {8, 341999}}, {10, {9, 342000}}
    };
    const std::map<int, std::pair<int, int>> TankDataMap = {
        {1, {0, 29}}, {2, {1, 499}}, {3, {2, 1349}}, {4, {3, 5649}}, {5, {4, 10199}},
        {6, {5, 18749}}, {7, {6, 38999}}, {8, {7, 141999}}, {9, {8, 219999}}, {10, {9, 220000}}
    };
}

// Commands
namespace Commands
{
    // Misc
    json Reload()
    {
        return json::array({OBF("update_progress"), json::object()});
    }

    json MySlots(const std::vector<int>& slots = {})
    {
        json result = {
            {OBF("player_id"), Variables::PlayerID}, {OBF("slots"), slots}
        };
        return json::array({OBF("get_progress"), std::move(result)});
    }

    json GrabbedSlots(int SpoofPlayerID, const std::vector<int>& slots = {})
    {
        json result = {
            {OBF("player_id"), SpoofPlayerID}, {OBF("slots"), slots}
        };
        return json::array({ OBF("get_progress"), std::move(result)});
    }

    json EmptyCommand()
    {
        return json::array({OBF("update_progress"), Constants::EmptyCMD});
    }

    json Custom()
    {
        return json::array({Variables::EventInput, json::parse(Variables::CommandInput)});
    }
    

    // Acc Stuff
    json SetLevel(const int& SelectedLevel)
    {
        json commands;

        for (int level = 1; level <= SelectedLevel; ++level)
        {
            commands.push_back(CreateCommand(CommandsID::UpdateLevel, {{"l", level}}, {140, 6}));
        }
        
        commands.insert(commands.end(), {
            CreateCommand(CommandsID::UpdateExperience, {{"e", 0}, {"ec", 4}, {"ad", json::object()}}, {140, 6}),
            CreateCommand(CommandsID::AnalyticsProgress, {{"eid", 1043}, {"params", {{"ip1", 40}, {"ip2", 140}, {"ip3", 2040}, {"sp1", "MultiplayerMatchReward"}, {"jp1", {{"v_ProgressRoad", 11}, {"v_Tutorial", 12}}}}}}, {140, 6}),
        });

        return CreateSnapshot(commands);
    }

    json SetKeys(int count, const std::string& lotterynumber) {
        return json::array({ "update_progress", {
            {"id", 41}, {"ci", "c2961fdb"}, {"p", {{"i", lotterynumber}, {"ca", 37}, {"c", count}}}
        } });
    }

    json SetPayingUser1() {
        return json::array({ "update_progress", {
            {"id", 68}, {"ci", "92c32e72"}, {"p", {{"1", "4"}}}
        } });
    }

    json SetPayingUser2() {
        return json::array({ "update_progress", {
            {"id", 68}, {"ci", "92c32e72"}, {"p", {{"1", "38"}}}
        } });
    }

    json AddWeapon(int gunIndex, int level, int rarity)
    {
        json params = {
            {"i", gunIndex},
            {"u", {{"l", level}, {"r", rarity}}},
            {"oc", 9999}
        };
        return CreateCommand(CommandsID::AddWeaponV17, params);
    }

    json AddWeaponParts(int weaponIndex, int amount)
    {
        json params = {
            {"i", weaponIndex},
            {"d", amount}
        };
        return CreateCommand(CommandsID::AddDetails, params);
    }

    json RemoveWeapon(int gunIndex)
    {
        json params = { {"i", gunIndex} };
        return CreateCommand(CommandsID::RemoveWeapon, params);
    }
    //Weapon Commands, we can do this once we add the Item Record Dumping and put them in a list 
    /*
    json AddAllWeaponParts(int amount)
    {
        if (!WeaponData::isReady || !WeaponData::allCraftableWeapons) return CreateSnapshot({});

        json commands;
        for (const auto& weapon : *WeaponData::allCraftableWeapons)
        {
            commands.emplace_back(AddWeaponPartsJson(weapon.Index, amount));
        }
        return CreateSnapshot(commands);
    }

    json AddDlcWeapons(int level, int rarity, bool useDefaultRarity)
    {
        json commands;
        const std::vector<int> dlc_indices = { 1703001, 1704001, 1702001 };

        for (int index : dlc_indices)
        {
            int finalRarity = rarity;
            if (useDefaultRarity) {
                const WeaponData::WeaponInfo* weapon = WeaponData::FindByIndex(index);
                if (weapon) {
                    finalRarity = weapon->Rarity;
                }
            }
            commands.emplace_back(AddWeaponJson(index, level, finalRarity));
        }

        return CreateSnapshot(commands);
    }
    
    json AddAllWeapons(int level, int rarity, bool useDefaultRarity)
    {
        if (!WeaponData::isReady) return CreateSnapshot({});
        json commands;
        for (const auto& weapon : *WeaponData::allWeapons)
        {
            int finalRarity = useDefaultRarity ? weapon.Rarity : rarity;
            commands.emplace_back(AddWeaponJson(weapon.Index, level, finalRarity));
        }
        return CreateSnapshot(commands);
    }
    
    json RemoveAllWeapons()
    {
        if (!WeaponData::isReady) return CreateSnapshot({});
        json commands;
        for (const auto& weapon : *WeaponData::allWeapons)
        {
            commands.emplace_back(RemoveWeaponJson(weapon.Index));
        }
        return CreateSnapshot(commands);
    }
    
    json AddSelectedWeapons(int startIndex, int endIndex, int level, int rarity, bool useDefaultRarity)
    {
        if (!WeaponData::isReady) return CreateSnapshot({});
        json commands;
        int max_index = WeaponData::allWeapons->empty() ? 0 : (int)WeaponData::allWeapons->size() - 1;
        int clampedStart = (0 > startIndex) ? 0 : startIndex;
        int clampedEnd = (max_index < endIndex) ? max_index : endIndex;

        for (int i = clampedStart; i <= clampedEnd; ++i)
        {
            const auto& weapon = (*WeaponData::allWeapons)[i];
            int finalRarity = useDefaultRarity ? weapon.Rarity : rarity;
            commands.emplace_back(AddWeaponJson(weapon.Index, level, finalRarity));
        }
        return CreateSnapshot(commands);
    }
    
    json RemoveSelectedWeapons(int startIndex, int endIndex)
    {
        if (!WeaponData::isReady) return CreateSnapshot({});
        json commands;
        int max_index = WeaponData::allWeapons->empty() ? 0 : (int)WeaponData::allWeapons->size() - 1;
        int clampedStart = (0 > startIndex) ? 0 : startIndex;
        int clampedEnd = (max_index < endIndex) ? max_index : endIndex;

        for (int i = clampedStart; i <= clampedEnd; ++i)
        {
            const auto& weapon = (*WeaponData::allWeapons)[i];
            commands.emplace_back(RemoveWeaponJson(weapon.Index));
        }
        return CreateSnapshot(commands);
    }
    */

    json SetLoadout(int loadoutIndex)
    {
        std::string loadoutName = "Loadout " + std::to_string(loadoutIndex + 1);

        json payload = {
            {OBF("id"), 288},
            {OBF("p"), {
                {OBF("n"), loadoutIndex},
                {OBF("na"), loadoutName},
                {OBF("pw"), 3001},
                {OBF("bw"), 1001},
                {OBF("mw"), 9001},
                {OBF("spw"), 333001},
                {OBF("snw"), 67001},
                {OBF("prw"), 162001},
                {OBF("ar"), 75002},
                {OBF("ht"), 136002},
                {OBF("ms"), 139002},
                {OBF("cp"), 135002},
                {OBF("bt"), 138002},
                {OBF("sk"), 1004},
                {OBF("thg"), 12007},
                {OBF("vh"), 138005}
            }}
        };

        return json::array({ OBF("update_progress"), std::move(payload) });
    }

    json ResetLoadout()
    {
        json payload = {
            {OBF("id"), 287},
            {OBF("p"), {
                {OBF("n"), 0}
            }}
        };

        return json::array({ OBF("update_progress"), std::move(payload) });
    }

    json Currency(const bool& Spend = false)
    {
        json commands;
        commands.emplace_back(CreateCommand(Spend ? CommandsID::SpendCurrency : CommandsID::AddCurrency, {{"c", Lists::CurrencyList[Variables::CurrencyType]}, {"v", Variables::CurrencyAmount}, {"ca", AddCurrencyCause::GameTask}}));

        return CreateSnapshot(commands);
    }

    json AddModule(const int& Type)
    {
        json commands;
        commands.emplace_back(CreateCommand(CommandsID::InventoryAddItemConsumable, {{"t", 1155}, {"i", Type}, {"c", Variables::ModuleAmount}}));

        return CreateSnapshot(commands);
    }

    json ChangeNickname(std::string name)
    {
        json payload = {
            {OBF("id"), 1},
            {OBF("p"), {{OBF("Nick"), name}} }
        };

        return json::array({ OBF("update_progress"), std::move(payload)});
    }

    json SkipBrTutorial()
    {
        json payload = {
            { OBF("id"), 67 },
            { OBF("p"), {
                { OBF("1"), {
                    { "169", -1 }
                }}
            }}
        };

        return json::array({ OBF("update_progress"), payload});
    }

    json AddAllModules(const int& Amount = 2500)
    {
        json commands;

        for (size_t i = 0; i < Lists::ModuleList.size(); ++i)
        {
            commands.emplace_back(CreateCommand(CommandsID::InventoryAddItemConsumable, {{"t", 1155}, {"i", 346009 + i * 1000}, {"c", Amount}, {"sc", 1}}));
        }

        return CreateSnapshot(commands);
    }

    json UpgradeModule(const int& Type)
    {
        int current_level = Variables::ModuleSlot.value(std::to_string(Type), 1);
        int increase_times = 10 - current_level;

        json commands;

        for (short i = 1; i <= (std::min)(Variables::ModuleLevel, increase_times); ++i)
        {
            commands.emplace_back(CreateCommand(CommandsID::SpendCurrency, {{"c", "Coins"}, {"v", 0}, {"ca", SpendCurrencyCause::UpgradeModule}})); // Saitama does this
            commands.emplace_back(CreateCommand(CommandsID::ModuleInfoIncreaseUp, {{"i", Type}}));
        }
        commands.emplace_back(Constants::EmptyCMD); // In case no upgrades needed

        return CreateSnapshot(commands);
    }

    json UpgradeAllModules()
    {
        json commands;

        for (int module_index = 304; module_index <= 345; ++module_index)
        {
            int current_module = module_index * 1000 + 9;
            int current_level = Variables::ModuleSlot.value(std::to_string(current_module), 1);
            int increase_times = 10 - current_level;

            int max_increase = (std::min)(Variables::ModuleLevel, increase_times);

            for (short i = 0; i < max_increase; ++i)
            {
                commands.emplace_back(CreateCommand(CommandsID::SpendCurrency, {{"c", "Coins"}, {"v", 0}, {"ca", SpendCurrencyCause::UpgradeModule}}));
                commands.emplace_back(CreateCommand(CommandsID::ModuleInfoIncreaseUp, {{"i", current_module}}));
            }
        }
        commands.emplace_back(Constants::EmptyCMD); // In case no upgrades needed

        return CreateSnapshot(commands);
    }

    json AddConsumable(const CommandsID& CommandID, const int& Consumable, const int& Type)
    {
        json commands;
        commands.emplace_back(CreateCommand(CommandID, {{"c", Variables::ConsumableAmount}, {OBF("i"), Consumable}, {OBF("t"), Type} }));

        return CreateSnapshot(commands);
    }

    json SetRank(const int& Rank, const int& Amount)
    {
        json commands;
        commands.emplace_back(CreateCommand(CommandsID::UpdateClanLevelAndExperience, {{OBF("cr"), Rank}, {OBF("cre"), Amount}}));

        return CreateSnapshot(commands);
    }
    json BugClanFort()
    {
        json commands;
        commands.emplace_back(CreateCommand(CommandsID::InventoryRemoveItemConsumable, { {"t", 1460}, {"i", 56027}, {"c", 1} }));

        const short Reserve = 2000;

        for (int i = 0; i < Reserve; ++i)
        {
            const std::time_t currentTime = std::time(nullptr);
            std::stringstream ss; ss << currentTime;
            const std::string TIME = ss.str();

            const std::string ID = Variables::PlayerID + "." + TIME + ".1";

            commands.emplace_back(CreateCommand(CommandsID::CreateClanItems, { {"items", {{{"id", ID}, {"clan", Variables::ClanID}, {"item_index", i * 1000 + 1017}, {"count", 36737}, {"buy", false}}}} }));
        }

        return CreateSnapshot(commands);
    }
    json UnBugClanFort()
    {
        json commands;
        commands.emplace_back(CreateCommand(CommandsID::InventoryRemoveItemConsumable, { {"t", 1460}, {"i", 56027}, {"c", 1} }));

        const short Reserve = 2000;

        for (int i = 0; i < Reserve; ++i)
        {
            const std::time_t currentTime = std::time(nullptr);
            std::stringstream ss; ss << currentTime;
            const std::string TIME = ss.str();

            const std::string ID = Variables::PlayerID + "." + TIME + ".1";

            commands.emplace_back(CreateCommand(CommandsID::CreateClanItems, { {"items", {{{"id", ID}, {"clan", Variables::ClanID}, {"item_index", i * 1000 + 1017}, {"count", -1064800}, {"buy", false}}}} }));
        }

        return CreateSnapshot(commands);
    }
    json SetGoblinsNegative(const int& sec)
    {
        json commands;

        commands.emplace_back(CreateCommand(
            CommandsID::InventoryAddItemExpiryOnServer,
            {
                { OBF("index"), 2020 },
                { OBF("type"), 1320 },
                { OBF("sec"), -Variables::GoblinDays * 86400}
            }
        ));
        return CreateSnapshot(commands);
    }
    json SetGoblinsPositive(const int& sec)
    {
        json commands;
        commands.emplace_back(CreateCommand(
            CommandsID::InventoryAddItemExpiryOnServer,
            {
                { OBF("index"), 2020 },
                { OBF("type"), 1320 },
                { OBF("sec"), Variables::GoblinDays * 86400}
            }
        ));
        return CreateSnapshot(commands); 
    }
    json CompleteTutorial()
    {
        json commands;
        auto blacklist = Variables::TutorialSlot.empty() ? std::unordered_set<std::string>() : Variables::TutorialSlot.get<std::unordered_set<std::string>>();

        for (short t = 1100; t <= 2800; t += 100)
        {
            std::string tutorial_slot = std::to_string(t);
            if (blacklist.find(tutorial_slot) == blacklist.end())
            {
                commands.emplace_back(CreateCommand(CommandsID::TutorialComplete, {{OBF("t"), tutorial_slot}}));
            }
        }
        commands.emplace_back(Constants::EmptyCMD); // In case slots are full

        return CreateSnapshot(commands);
    }

    json VeteranBadge(const bool& Remove = false)
    {
        json commands;
        commands.emplace_back(CreateCommand(Remove ? CommandsID::InventoryRemoveItemSingle : CommandsID::InventoryAddItemSingle, {{"i", 700015}, {"ca", 115}}));

        return CreateSnapshot(commands);
    }

    // Stats
    json AddWin(const short& Mode)
    {
        json result = {
            {OBF("mode"), Mode}
        };
        return json::array({OBF("update_player"), std::move(result)});
    }

    json AddWinRaid(const short& Difficulty)
    {
        json result = {
            {OBF("mode"), 22}, {OBF("raid_diff"), Difficulty}
        };
        return json::array({OBF("update_player"), std::move(result)});
    }

    json AddWinTournament()
    {
        json result = {
            {OBF("tournament"), 1}
        };
        return json::array({OBF("update_player"), std::move(result)});
    }

    json UpdateMonthlyStats(const int& Kills, const int& Deaths, const int& HeadShots, const int& KillStreak, const int& WinStreak, const bool& Win = true)
    {
        json commands;
        commands.emplace_back(CreateCommand(CommandsID::UpdatePlayerStatsV2, {{"k", Kills}, {"d", Deaths}, {"w", Win}, {"hds", HeadShots}, {"h", HeadShots}, {"s", KillStreak}, {"ks", KillStreak}, {"c", false}, {"tw", WinStreak}, {"sw", WinStreak}, {"sid", RandHex(16)}, {"dk", GetLocalDate()}}));

        return CreateSnapshot(commands);
    }

    // Clan
    json SetClanFortLevel(const int& Level)
    {
        auto it = Constants::FortDataMap.find(Level);
        if (it != Constants::FortDataMap.end())
        {
            const auto& data = it->second;
            json result = {{OBF("field_level"), data.first}, {OBF("field_points"), data.second}};
            return json::array({OBF("set_clan_building_levels"), result});
        }

        return json::array();
    }

    json SetClanTankLevel(const int& Level)
    {
        auto it = Constants::TankDataMap.find(Level);
        if (it != Constants::TankDataMap.end())
        {
            const auto& data = it->second;
            json result = {{OBF("tank_level"), data.first}, {OBF("tank_points"), data.second} };
            return json::array({OBF("set_clan_building_levels"), result});
        }

        return json::array();
    }

    json EditClan(const std::string& Logo)
    {
        json result = {
            {"logo", Logo}
        };
        return json::array({OBF("change_clan_params"), std::move(result)});
    }

    json EditClanName(const std::string& Name)
    {
        json result = {
            {"name", Name}
        };
        return json::array({OBF("change_clan_params"), std::move(result)});
    }

    json AddClanValor()
    {
        json result = {
            {OBF("clan_id"), Variables::ClanID}, {OBF("war_id"), Variables::ClanWarID}, {OBF("shields"), Variables::ValorAmount}, {OBF("task_id"), Random(-100000000, 10000000)}
        };
        return json::array({OBF("warV2_obtain_clan_shields"), std::move(result)});
    }

    json CompleteClanTasks()
    {
        json commands;
        json result = {{OBF("id"), CommandsID::GameTaskChangeInfoInBattle}, {OBF("i"), RandHex()}};

        for (int i = OBF(1); i <= OBF(13); ++i)
        {
            for (int j = OBF(1000); j <= OBF(1200); ++j)
            {
                std::string key = std::to_string(i) + "_" + std::to_string(j);
                commands[key] = {{OBF("p"), 696969}};
            }
        }

        result[OBF("p")] = commands;
        return json::array({OBF("update_progress"), std::move(result)});
    }

    //Pixel Pass
    json WipePassSlot()
    {
        json payload = {
            {OBF("id"), 331}
        };

        return json::array({ OBF("update_progress"), std::move(payload) });
    }

    json NovaPixelPass(int dayCount, int maxTier)
    {
        auto future_time = std::chrono::system_clock::now() + std::chrono::hours(24 * dayCount);
        std::time_t end_time = std::chrono::system_clock::to_time_t(future_time);
        std::tm tm_buf; localtime_s(&tm_buf, &end_time);
        std::stringstream ss; ss << std::put_time(&tm_buf, "%Y-%m-%d");
        std::string raw_config_json = R"({"c":{"i":420691,"sn":"PLACEHOLDER_NAME","s":"2023-01-01T00:00:00","e":"PLACEHOLDER_DATE","vc":-999,"etr":["1347005:1","471014:1","5038:1"],"etp":["1347005:1","834001:1"],"tp":["834001:1","5038:1"],"lp":"11015:1","pp":"11015:1","po":"0:0","ps":0,"ppb":"2015:1","ppo":"2015:1","dpb":20,"pbl":16,"hc":259200,"owc":900,"bt":"PixelPassBannerForB"},"p":[{"p":1,"pi":["136029:1","167029:1","86029:1","1634001:1"]},{"p":2,"pi":["136029:1","167029:1","86029:1","1634001:1"]},{"p":3,"pi":["136029:1","167029:1","86029:1","1634001:1"]},{"p":4,"pi":["136029:1","167029:1","86029:1","1634001:1"]},{"p":5,"pi":["136029:1","167029:1","86029:1","1634001:1"]},{"p":6,"pi":["136029:1","167029:1","86029:1","1634001:1"]},{"p":7,"pi":["136029:1","167029:1","86029:1","1634001:1"]},{"p":8,"pi":["136029:1","167029:1","86029:1","1634001:1"]},{"p":9,"pi":["136029:1","167029:1","86029:1","1634001:1"]}],"l":[{"l":0,"t":1,"p":1,"r":["834001:1","796005:1"]},{"l":1,"t":3,"p":1,"e":1,"r":["17018:50"]},{"l":0,"t":3,"p":1,"e":1,"r":["500015:1"]},{"l":1,"t":3,"p":1,"e":50,"r":["501015:1"]},{"l":2,"t":3,"p":1,"e":100,"r":["502015:1"]},{"l":3,"t":3,"p":1,"e":150,"r":["503015:1"]},{"l":4,"t":3,"p":1,"e":250,"r":["1027:10"]},{"l":5,"t":3,"p":2,"e":350,"r":["275027:5"]},{"l":6,"t":3,"p":2,"e":450,"r":["279027:60"]},{"l":7,"t":3,"p":2,"e":550,"r":["38027:5"]},{"l":8,"t":3,"p":2,"e":650,"r":["34027:5"]},{"l":9,"t":3,"p":2,"e":800,"r":["30027:5"]},{"l":10,"t":3,"p":2,"e":950,"r":["26027:5"]},{"l":11,"p":3,"e":1100,"r":["25002:1"]},{"l":12,"t":3,"p":3,"e":1250,"r":["155027:10"]},{"l":13,"t":3,"p":3,"e":1400,"r":["156027:10"]},{"l":14,"t":3,"p":3,"e":1600,"r":["157027:10"]},{"l":15,"t":3,"p":3,"e":1800,"r":["158027:10"]},{"l":16,"t":3,"p":3,"e":2000,"r":["159027:10"]},{"l":17,"p":4,"e":2200,"r":["1856001:1"]},{"l":18,"p":4,"e":2400,"r":["1840001:1"]},{"l":19,"p":4,"e":2650,"r":["1823001:1"]},{"l":20,"p":4,"e":2900,"r":["1832001:1"]},{"l":21,"p":4,"e":3150,"r":["1753001:1"]},{"l":22,"p":4,"e":3400,"r":["1748001:1"]},{"l":23,"p":5,"e":3650,"r":["1712001:1"]},{"l":24,"p":5,"e":3650,"r":["1790001:1"]},{"l":25,"p":5,"e":3650,"r":["1794001:1"]},{"l":26,"p":5,"e":3650,"r":["1792001:1"]},{"l":27,"p":5,"e":3650,"r":["828001:1"]},{"l":28,"p":5,"e":3650,"r":["1514001:1"]},{"l":29,"p":6,"e":5500,"r":["1854001:1"]},{"l":30,"p":6,"e":5850,"r":["1746001:1"]},{"l":31,"p":6,"e":6200,"r":["1759001:1"]},{"l":32,"p":6,"e":6550,"r":["1197001:1"]},{"l":33,"p":6,"e":6900,"r":["1342001:1"]},{"l":34,"p":6,"e":7350,"r":["1411001:1"]},{"l":35,"p":7,"e":7800,"r":["222006:1"]},{"l":36,"p":7,"e":8250,"r":["218006:1"]},{"l":37,"p":7,"e":8700,"r":["215006:1"]},{"l":38,"p":7,"e":9150,"r":["217006:1"]},{"l":39,"p":7,"e":9650,"r":["216006:1"]},{"l":40,"p":7,"e":10150,"r":["224006:1"]},{"l":41,"p":8,"e":10650,"r":["223006:1"]},{"l":42,"t":3,"p":8,"e":11150,"r":["2020:604000"]},{"l":43,"t":3,"p":8,"e":11650,"r":["1034:5"]},{"l":44,"t":3,"p":8,"e":12150,"r":["4030:5"]},{"l":45,"p":8,"e":12650,"r":["104003:500"]},{"l":46,"t":3,"p":8,"e":13150,"r":["27015:100"]},{"l":47,"t":3,"p":9,"e":13650,"r":["138027:1"]},{"l":48,"t":3,"p":9,"e":14150,"r":["267027:20"]},{"l":49,"t":3,"p":9,"e":14650,"r":["269027 :20"]},{"l":50,"t":3,"p":9,"e":15150,"r":["7015:100"]},{"l":51,"p":9,"e":15650,"r":["49028:1"]},{"l":69696969,"p":9,"e":15650,"r":["86029:1"]}],"prt":[301,303,305,308,309,312,314,317,320,322,325,329,333,336,339,3000,3001,3002,3003,3004],"of":[{"i":1,"e":1,"sd":1,"it":[],"pr":["10015:1"],"g":2,"mpn":"MainStorePromo_PixelPass_Currency_Layout","vv":"layout","im":"OfferIcons/ImageCurrencyGem_ChestSilver_02","si":"pixelcurrency"}]})";
        json pass_data = json::parse(raw_config_json);
        pass_data["c"]["sn"] = OBF("NovaClientSeason      \n\n\n\n\n\n\n\npass by discord.gg/pgcentral");
        pass_data["c"]["e"] = ss.str() + OBF("T10:00:00");

        for (auto& level : pass_data["l"]) {
            if (level.value("l", 0) == 69696969) { level["l"] = maxTier; break; }
        }

        return json::array({ OBF("update_progress"), {
            {OBF("id"), 322},
            {OBF("ci"), OBF("02fde2ec")},
            {OBF("p"), { {OBF("i"), 420691}, {OBF("c"), pass_data.dump()} }},
            {OBF("req_id"), 416601365}
        } });
    }

    json CustomGiftingPass(const std::vector<std::string>& giftItems)
    {
        auto now = std::chrono::system_clock::now();
        auto future_time = now + std::chrono::hours(24 * 30);
        std::time_t end_time = std::chrono::system_clock::to_time_t(future_time);
        std::tm tm_buf;
        localtime_s(&tm_buf, &end_time);
        std::stringstream ss;
        ss << std::put_time(&tm_buf, "%Y-%m-%d");
        std::string expiry_date_str = ss.str();

        std::string raw_config_json = R"({
        "c": { "i": 42069, "sn": "PLACEHOLDER_NAME", "s": "2023-01-01T00:00:00", "e": "2026-01-01T00:00:00", "vc": -999, "etr": [], "etp": [], "tp": [], "lp": "11015:1", "pp": "11015:1", "po": "0:0", "ps": 0, "ppb": "17018:1", "ppo": "17018:1", "dpb": 20, "pbl": 16, "hc": 259200, "owc": 900, "bt": "PixelPassBannerForB"},
       "p":[{"p":1,"pi":["1882001:1","1377005:1"]},{"p":2,"pi":["1882001:1","1377005:1"]},{"p":3,"pi":["1377005:1","1380005:1"]},{"p":4,"pi":["1882001:1","1377005:1"]},{"p":5,"pi":["1882001:1","1377005:1"]},{"p":6,"pi":["1882001:1","1377005:1"]},{"p":7,"pi":["1377005:1","583014:1"]},{"p":8,"pi":["1883001:1","1378005:1"]},{"p":9,"pi":["1883001:1","1378005:1"]}],"l":[{"l":0,"t":1,"p":1,"r":["1377005:1"]},{"l":1,"p":1,"e":50,"r":["1015:25"],"f":1},{"l":2,"p":1,"e":100,"r":["26015:90"],"c":true},{"l":3,"p":1,"e":150,"r":["1030:1"],"f":1},{"l":4,"p":1,"e":200,"r":["2015:25"],"c":true},{"l":5,"p":1,"e":250,"r":["500015:1"],"f":1},{"l":6,"p":2,"e":350,"r":["1015:35"],"c":true},{"l":7,"p":2,"e":450,"r":["3030:1"]},{"l":8,"p":2,"e":600,"r":["129027:1"],"f":1,"c":true},{"l":9,"p":2,"e":750,"r":["1015:50"]},{"l":10,"p":2,"e":900,"r":["219027:1"],"f":1},{"l":11,"p":2,"e":1050,"r":["501015:1"],"c":true},{"l":12,"p":3,"e":1250,"r":["26015:25"],"c":true},{"l":13,"p":3,"e":1450,"r":["2030:1"]},{"l":14,"p":3,"e":1650,"r":["500015:1"],"f":1},{"l":15,"p":3,"e":1850,"r":["1380005:1"],"c":true},{"l":16,"p":3,"e":2100,"r":["2015:35"],"f":1},{"l":17,"p":3,"e":2350,"r":["501015:1"],"c":true},{"l":18,"p":4,"e":2600,"r":["129027:1"],"f":1},{"l":19,"p":4,"e":2850,"r":["2015:45"],"c":true},{"l":20,"p":4,"e":3150,"r":["1015:60"],"f":1},{"l":21,"p":4,"e":3450,"r":["94027:1"],"c":true},{"l":22,"p":4,"e":3750,"r":["26015:15"],"f":1},{"l":23,"p":4,"e":4050,"r":["501015:1"],"c":true},{"l":24,"p":5,"e":4350,"r":["1015:75"],"c":true},{"l":25,"p":5,"e":4700,"r":["2030:1"],"f":1},{"l":26,"p":5,"e":5050,"r":["1882001:1"],"c":true},{"l":27,"p":5,"e":5400,"r":["2015:50"]},{"l":28,"p":5,"e":5750,"r":["26015:20"],"f":1},{"l":29,"p":5,"e":6100,"r":["501015:1"],"f":1,"c":true},{"l":30,"p":6,"e":6500,"r":["3030:1"],"c":true},{"l":31,"p":6,"e":6900,"r":["219027:1"],"f":1},{"l":32,"p":6,"e":7300,"r":["129027:1"]},{"l":33,"p":6,"e":7700,"r":["1030:2"],"f":1,"c":true},{"l":34,"p":6,"e":8100,"r":["2015:55"]},{"l":35,"p":6,"e":8550,"r":["26015:25"],"f":1},{"l":36,"p":7,"e":9000,"r":["129027:1"],"f":1,"c":true},{"l":37,"p":7,"e":9450,"r":["2015:60"]},{"l":38,"p":7,"e":9900,"r":["501015:1"],"f":1},{"l":39,"p":7,"e":10350,"r":["583014:1"],"c":true},{"l":40,"p":7,"e":10850,"r":["500015:1"]},{"l":41,"p":7,"e":11350,"r":["1015:100"],"f":1,"c":true},{"l":42,"p":8,"e":11850,"r":["26015:30"],"f":1},{"l":43,"p":8,"e":12350,"r":["62028:1"],"c":true},{"l":44,"p":8,"e":12850,"r":["94027:1"]},{"l":45,"p":8,"e":13350,"r":["501015:1"],"f":1},{"l":46,"p":8,"e":13900,"r":["129027:1"],"c":true},{"l":47,"p":8,"e":14450,"r":["5017018:1"]},{"l":48,"p":9,"e":15000,"r":["2015:70"],"f":1,"c":true},{"l":49,"p":9,"e":15550,"r":["5017018:1"],"f":1},{"l":50,"p":9,"e":16100,"r":["94027:1"]},{"l":51,"p":9,"e":16650,"r":["26015:50"]},{"l":52,"p":9,"e":17200,"r":["1883001:1"],"c":true},{"l":0,"t":3,"p":9,"e":17900,"r":["5027:1"],"f":1,"c":true}],
        "prt": [301,303,305,308,309,312,314,317,320,322,325,329,333,336,339,3000,3001,3002,3003,3004],
        "of": [{"i":1,"e":1,"sd":1,"it":[],"pr":["10015:1"],"g":2,"mpn":"MainStorePromo_PixelPass_Currency_Layout","vv":"layout","im":"OfferIcons/ImageCurrencyGem_ChestSilver_02","si":"pixelcurrency"}]
        })";

        json pass_data = json::parse(raw_config_json);
        pass_data["c"]["sn"] = OBF("NovaGiftingSeason");


        if (!pass_data["of"].empty()) {
            pass_data["of"][0]["pr"] = json::array({ "10015:1" });
            pass_data["of"][0]["it"] = giftItems;
        }

        json payload = {
            {OBF("id"), 322},
            {OBF("ci"), OBF("02fde2ec")},
            {OBF("p"), {
                {OBF("i"), 42069},
                {OBF("c"), pass_data.dump()}
            }},
            {OBF("req_id"), 416601366}
        };

        return json::array({ OBF("update_progress"), std::move(payload) });
    }

    json SetPassExperience()
    {
        json payload = {
            {OBF("id"), 326},
            {OBF("p"), {{OBF("e"), 15650}}}
        };

        return json::array({ OBF("update_progress"), std::move(payload) });
    }

    json ActivatePass()
    {
        json payload = {
            {OBF("id"), 324}
        };

        return json::array({ OBF("update_progress"), std::move(payload) });
    }

    //Sandbox
    json SetSandboxHeart(int count) {
        return json::array({ "update_progress", {
            {"id", 67}, {"i", "11f07e5c"}, {"p", {{"1", {{"18", count}}}}}
        } });
    }

    json SetSandboxHead(int count) {
        return json::array({ "update_progress", {
            {"id", 67}, {"i", "88f92fe6"}, {"p", {{"1", {{"14", count}}}}}
        } });
    }

    json SetSandboxWings(int count) {
        return json::array({ "update_progress", {
            {"id", 67}, {"i", "88f92fe6"}, {"p", {{"1", {{"15", count}}}}}
        } });
    }

    json SetSandboxDisco(int count) {
        return json::array({ "update_progress", {
            {"id", 67}, {"i", "fffe1f70"}, {"p", {{"1", {{"17", count}}}}}
        } });
    }

    json SetSandboxBear(int count) {
        return json::array({ "update_progress", {
            {"id", 67}, {"i", "619a8ad3"}, {"p", {{"1", {{"16", count}}}}}
        } });
    }
}
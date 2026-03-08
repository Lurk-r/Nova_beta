#include "OfferItemDumper.hpp"
#include <fstream>
#include <filesystem>
#include <map>
#include <string>
#include <vector>

using ojson = nlohmann::ordered_json;

namespace OfferItemDumper
{
    std::vector<ItemData> CachedItems;

    std::string GetCategoryName(int c) {
        switch (c) {
        case 0: return "Primary";
        case 1: return "Backup";
        case 2: return "Melee";
        case 3: return "Special";
        case 4: return "Sniper";
        case 5: return "Premium";
        case 6: return "Hats";
        case 7: return "Armor";
        case 8: return "Skins";
        case 9: return "Capes";
        case 10: return "Boots";
        case 11: return "Gear";
        case 12: return "Mask";
        case 1000: return "SkinsEditor";
        case 1100: return "SkinsMale";
        case 1200: return "SkinsFemale";
        case 1300: return "SkinsSpecial";
        case 1400: return "SkinsPremium";
        case 2000: return "LeagueWeaponSkins";
        case 2100: return "LeagueHats";
        case 2200: return "LeagueSkins";
        case 12500: return "Throwing";
        case 13000: return "Tools";
        case 13500: return "Support";
        case 25000: return "Pets";
        case 30000: return "Eggs";
        case 35000: return "BestWeapons";
        case 40000: return "BestWear";
        case 45000: return "BestGadgets";
        case 110000: return "WeaponCraft";
        case 135000: return "EventCraft";
        case 140000: return "SetsCraft";
        case 141000: return "RoyaleCraft";
        case 150000: return "Avatar";
        case 160000: return "GlidersRoyale";
        case 170000: return "ShovelsRoyale";
        case 180000: return "HatsRoyale";
        case 190000: return "RoyaleAvatars";
        case 200000: return "TrailRoyale";
        case 210000: return "TransportRoyale";
        case 220000: return "ItemModule";
        case 230000: return "ItemModulesChest";
        case 240000: return "WeaponSkins";
        case 250000: return "Graffiti";
        case 260000: return "ArmorSkin";
        case 270000: return "WearSkin";
        case 9990000: return "None";
        case -1: return "";
        default: return std::to_string(c);
        }
    }

    std::string EscapeJsonString(const std::string& input) {
        std::string output;
        for (char c : input) {
            switch (c) {
            case '"': output += "\\\""; break;
            case '\\': output += "\\\\"; break;
            case '\b': output += "\\b"; break;
            case '\f': output += "\\f"; break;
            case '\n': output += "\\n"; break;
            case '\r': output += "\\r"; break;
            case '\t': output += "\\t"; break;
            default: output += c; break;
            }
        }
        return output;
    }

    void LoadItemsToMemory()
    {
        CachedItems.clear();

        std::cout << "[PTR] ContentRegister::GetInstance     = " << (Pointers::ContentRegister::GetInstance ? "SET" : "NULL") << "\n";
        std::cout << "[PTR] ContentRegister::GetRegisterList = " << (Pointers::ContentRegister::GetRegisterList ? "SET" : "NULL") << "\n";
        std::cout << "[PTR] ContentRegister::GetItemIndex    = " << (Pointers::ContentRegister::GetItemIndex ? "SET" : "NULL") << "\n";
        std::cout << "[PTR] OfferItem::GetItemTyped          = " << (Pointers::OfferItem::GetItemTyped ? "SET" : "NULL") << "\n";
        std::cout << "[PTR] OfferItem::GetItemByString       = " << (Pointers::OfferItem::GetItemByString ? "SET" : "NULL") << "\n";
        std::cout << "[PTR] OfferItem::GetName               = " << (Pointers::OfferItem::GetName ? "SET" : "NULL") << "\n";
        std::cout << "[PTR] OfferItem::GetNameDefault        = " << (Pointers::OfferItem::GetNameDefault ? "SET" : "NULL") << "\n";
        std::cout << "[PTR] ItemDb::GetByIndex               = " << (Pointers::ItemDb::GetByIndex ? "SET" : "NULL") << "\n";
        std::cout << "[PTR] ItemDb::GetItemCategory          = " << (Pointers::ItemDb::GetItemCategory ? "SET" : "NULL") << "\n";
        std::cout << "[PTR] ItemRecord::get_Category         = " << (Pointers::ItemRecord::get_Category ? "SET" : "NULL") << "\n";
        std::cout << "[PTR] ItemRecord::get_Rarity           = " << (Pointers::ItemRecord::get_Rarity ? "SET" : "NULL") << "\n";
        std::cout << "[PTR] ItemRecord::get_IsCraft          = " << (Pointers::ItemRecord::get_IsCraft ? "SET" : "NULL") << "\n";
        std::cout << "[PTR] ItemRecord::get_WeaponNumber     = " << (Pointers::ItemRecord::get_WeaponNumber ? "SET" : "NULL") << "\n";
        std::cout << "[PTR] ItemRecord::get_IsRoyale         = " << (Pointers::ItemRecord::get_IsRoyale ? "SET" : "NULL") << "\n";
        std::cout << "[PTR] RoyaleOITClass::RoyaleDictionary = " << (Pointers::RoyaleOITClass::RoyaleDictionary ? "SET" : "NULL") << "\n";

        static const std::map<std::string, int> offerItemType = {
            {"None", 0}, {"Weapons", 10}, {"Armor", 20}, {"Mask", 30}, {"Hat", 40},
            {"Boots", 50}, {"Cape", 60}, {"Skin", 65}, {"Gadget", 70}, {"Pet", 80},
            {"Egg", 83}, {"LobbyItem", 85}, {"FortItem", 90}, {"Gems", 1000},
            {"Coins", 1010}, {"Leprechaun", 1020}, {"WeaponUpgrade", 1030},
            {"GachaFreeSpin", 1040}, {"EventCurrency", 1050}, {"VIP", 1060},
            {"Parts", 1070}, {"Royale", 1080}, {"BattlePassLevel", 1090},
            {"BattlePassExp", 1100}, {"BattlePassCurrency", 1110}, {"GoldenSkin", 1120},
            {"EventChest", 1130}, {"CraftCurrency", 1140}, {"Module", 1150},
            {"ModulePoint", 1155}, {"ModuleChest", 1160}, {"WeaponSkin", 1170},
            {"ClanCurrency", 1180}, {"Coupons", 1190}, {"Currency", 1200},
            {"Character", 1210}, {"ClanShields", 1220}, {"ClanLootBox", 1230},
            {"ClanPlaceable", 1240}, {"ClanPlaceablePoint", 1250}, {"Detail", 1300},
            {"WeaponLevelUpgrade", 1310}, {"PlayerBuff", 1320}, {"ClanBuff", 1330},
            {"WeaponQualityUpgrade", 1340}, {"ClanBuilding", 1360},
            {"ClanBuildingPoint", 1370}, {"FreeUpgrade", 1380}, {"Chest", 1390},
            {"Exp", 1400}, {"Stats", 1410}, {"ModeSlots", 1420}, {"Executable", 1430},
            {"Tank", 1440}, {"VIP20", 1450}, {"LootBox", 1460}, {"Graffiti", 1470},
            {"PixelPassExp", 1490}, {"ClanRankExperience", 1500}, {"WearSkin", 1510},
            {"Applicable", 1520}, {"CraftSet", 1530}, {"FeatureExp", 1540},
            {"PackagedItem", 1550}, {"Achievement", 1560}, {"ExpirySimple", 1570},
            {"Static", 1580}, {"GemsHarvester", 1590}, {"TreasureMap", 1600},
            {"ProfileBackgroundUI", 1610}, {"PortraitFrameUI", 1620}, {"PortraitUI", 1630}
        };

        if (!Pointers::ContentRegister::GetInstance)
        {
            std::cout << "[FATAL] GetInstance is null, aborting LoadItemsToMemory\n";
            return;
        }

        auto instance = Pointers::ContentRegister::GetInstance();
        if (!instance)
        {
            std::cout << "[FATAL] GetInstance() returned null, aborting LoadItemsToMemory\n";
            return;
        }
        std::cout << "[OK] ContentRegister instance = " << (void*)instance << "\n";

        std::map<int, int> royaleIndexToCategory;
        if (Pointers::RoyaleOITClass::RoyaleDictionary)
        {
            void* royaleDict = Pointers::RoyaleOITClass::RoyaleDictionary();
            if (royaleDict)
            {
                IL2CPP::String* serialized = Pointers::Json::SerializeObject((IL2CPP::Object*)royaleDict);
                if (serialized)
                {
                    std::string jsonRaw = serialized->ToString();
                    if (!jsonRaw.empty())
                    {
                        try
                        {
                            ojson royaleJson = ojson::parse(jsonRaw);
                            for (auto& [key, value] : royaleJson.items())
                            {
                                if (!value.is_object()) continue;
                                if (!value.contains("Category") || !value.contains("Index")) continue;
                                int category = static_cast<int>(value["Category"]);
                                int index = static_cast<int>(value["Index"]);
                                royaleIndexToCategory[index] = category;
                            }
                        }
                        catch (...) {}
                    }
                }
            }
        }

        for (const auto& pair : offerItemType)
        {
            const std::string& groupName = pair.first;
            int type = pair.second;

            auto list = Pointers::ContentRegister::GetRegisterList(instance, type);
            if (!list)
            {
                if (type == 10)
                    std::cout << "[WEAPONS] GetRegisterList returned null for TypeID=10\n";
                continue;
            }

            // Log weapon list found
            if (type == 10)
                std::cout << "[WEAPONS] GetRegisterList OK for TypeID=10, starting ForEach...\n";

            // Track first weapon to do a one-time deep pointer test
            bool didWeaponPointerTest = false;

            list->ForEach([&](IL2CPP::String* x) {
                ItemData newItem;
                newItem.GroupCategory = groupName;
                newItem.TypeID = type;
                newItem.Index = Pointers::ContentRegister::GetItemIndex(instance, type, x);
                newItem.ID = x ? x->ToString() : "NULL";

                newItem.Name = "not found";
                newItem.InGameCategory = "";
                newItem.Rarity = "";
                newItem.IsCraftable = false;
                newItem.GalleryNumber = -1;
                newItem.IsRoyale = false;
                newItem.IsShovel = false;

                // -------------------------------------------------------
                // ONE-TIME DEEP TEST ON FIRST WEAPON
                // -------------------------------------------------------
                if (type == 10 && !didWeaponPointerTest)
                {
                    didWeaponPointerTest = true;
                    std::cout << "\n--- FIRST WEAPON DEEP TEST ---\n";
                    std::cout << "  ID=" << (x ? x->ToString() : "NULL") << " Index=" << newItem.Index << "\n";

                    // Test GetItemTyped
                    IL2CPP::Object* typedObj = nullptr;
                    if (Pointers::OfferItem::GetItemTyped)
                    {
                        typedObj = Pointers::OfferItem::GetItemTyped(type, x, 1);
                        std::cout << "  GetItemTyped result = " << (void*)typedObj << "\n";
                    }
                    else std::cout << "  GetItemTyped = NULL PTR\n";

                    // Test GetItemByString
                    IL2CPP::Object* byStrObj = nullptr;
                    if (Pointers::OfferItem::GetItemByString)
                    {
                        byStrObj = Pointers::OfferItem::GetItemByString(x);
                        std::cout << "  GetItemByString result = " << (void*)byStrObj << "\n";
                    }
                    else std::cout << "  GetItemByString = NULL PTR\n";

                    // Test GetName on whichever obj we got
                    IL2CPP::Object* nameObj = typedObj ? typedObj : byStrObj;
                    if (nameObj)
                    {
                        if (Pointers::OfferItem::GetName)
                        {
                            auto* ns = Pointers::OfferItem::GetName(nameObj);
                            std::cout << "  GetName result = " << (ns ? ns->ToString() : "NULL STRING") << "\n";
                        }
                        else std::cout << "  GetName = NULL PTR\n";

                        if (Pointers::OfferItem::GetNameDefault)
                        {
                            auto* ns2 = Pointers::OfferItem::GetNameDefault(nameObj);
                            std::cout << "  GetNameDefault result = " << (ns2 ? ns2->ToString() : "NULL STRING") << "\n";
                        }
                        else std::cout << "  GetNameDefault = NULL PTR\n";
                    }
                    else std::cout << "  No nameObj available to test GetName\n";

                    // Test ItemDb::GetByIndex
                    if (Pointers::ItemDb::GetByIndex)
                    {
                        IL2CPP::Object* rec = Pointers::ItemDb::GetByIndex(newItem.Index);
                        std::cout << "  GetByIndex(" << newItem.Index << ") result = " << (void*)rec << "\n";

                        if (rec)
                        {
                            if (Pointers::ItemRecord::get_Category)
                                std::cout << "  get_Category = " << Pointers::ItemRecord::get_Category(rec) << "\n";
                            else std::cout << "  get_Category = NULL PTR\n";

                            if (Pointers::ItemRecord::get_Rarity)
                                std::cout << "  get_Rarity = " << Pointers::ItemRecord::get_Rarity(rec) << "\n";
                            else std::cout << "  get_Rarity = NULL PTR\n";

                            if (Pointers::ItemRecord::get_IsCraft)
                                std::cout << "  get_IsCraft = " << Pointers::ItemRecord::get_IsCraft(rec) << "\n";
                            else std::cout << "  get_IsCraft = NULL PTR\n";

                            if (Pointers::ItemRecord::get_WeaponNumber)
                                std::cout << "  get_WeaponNumber = " << Pointers::ItemRecord::get_WeaponNumber(rec) << "\n";
                            else std::cout << "  get_WeaponNumber = NULL PTR\n";

                            if (Pointers::ItemRecord::get_IsRoyale)
                                std::cout << "  get_IsRoyale = " << Pointers::ItemRecord::get_IsRoyale(rec) << "\n";
                            else std::cout << "  get_IsRoyale = NULL PTR\n";
                        }
                        else
                        {
                            std::cout << "  GetByIndex returned NULL - trying index/1000 = " << (newItem.Index / 1000) << "\n";
                            IL2CPP::Object* rec2 = Pointers::ItemDb::GetByIndex(newItem.Index / 1000);
                            std::cout << "  GetByIndex(" << (newItem.Index / 1000) << ") result = " << (void*)rec2 << "\n";
                        }
                    }
                    else std::cout << "  ItemDb::GetByIndex = NULL PTR\n";

                    // Test GetItemCategory
                    if (Pointers::ItemDb::GetItemCategory)
                    {
                        int cat = Pointers::ItemDb::GetItemCategory(x);
                        std::cout << "  GetItemCategory result = " << cat << " (" << GetCategoryName(cat) << ")\n";
                    }
                    else std::cout << "  ItemDb::GetItemCategory = NULL PTR\n";

                    std::cout << "--- END FIRST WEAPON DEEP TEST ---\n\n";
                }
                // -------------------------------------------------------

                if (x)
                {
                    IL2CPP::Object* nameObj = nullptr;

                    if (Pointers::OfferItem::GetItemTyped)
                        nameObj = Pointers::OfferItem::GetItemTyped(type, x, 1);

                    if (!nameObj && Pointers::OfferItem::GetItemByString)
                        nameObj = Pointers::OfferItem::GetItemByString(x);

                    if (nameObj) {
                        IL2CPP::String* nameStr = nullptr;
                        if (Pointers::OfferItem::GetName)
                            nameStr = Pointers::OfferItem::GetName(nameObj);

                        if (!nameStr && Pointers::OfferItem::GetNameDefault)
                            nameStr = Pointers::OfferItem::GetNameDefault(nameObj);

                        if (nameStr) newItem.Name = nameStr->ToString();
                    }
                }

                if (type == 1080)
                {
                    newItem.IsRoyale = true;
                    auto it = royaleIndexToCategory.find(newItem.Index);
                    if (it != royaleIndexToCategory.end())
                    {
                        newItem.InGameCategory = GetCategoryName(it->second);
                    }
                }
                else
                {
                    IL2CPP::Object* recordObj = nullptr;
                    if (Pointers::ItemDb::GetByIndex) {
                        recordObj = Pointers::ItemDb::GetByIndex(newItem.Index);
                    }

                    if (recordObj) {
                        if (Pointers::ItemRecord::get_Category) {
                            int categoryValue = Pointers::ItemRecord::get_Category(recordObj);
                            newItem.InGameCategory = GetCategoryName(categoryValue);
                        }
                        if (Pointers::ItemRecord::get_Rarity) {
                            int rarityValue = Pointers::ItemRecord::get_Rarity(recordObj);
                            newItem.Rarity = Pointers::ItemRecord::GetRarityStr(rarityValue);
                        }
                        if (Pointers::ItemRecord::get_IsCraft) {
                            newItem.IsCraftable = Pointers::ItemRecord::get_IsCraft(recordObj);
                        }
                        if (Pointers::ItemRecord::get_WeaponNumber) {
                            newItem.GalleryNumber = Pointers::ItemRecord::get_WeaponNumber(recordObj);
                        }
                        if (Pointers::ItemRecord::get_IsRoyale) {
                            newItem.IsRoyale = Pointers::ItemRecord::get_IsRoyale(recordObj);
                        }
                    }

                    if (newItem.InGameCategory.empty() && x && Pointers::ItemDb::GetItemCategory) {
                        int catVal = Pointers::ItemDb::GetItemCategory(x);
                        newItem.InGameCategory = GetCategoryName(catVal);
                    }
                }

                CachedItems.push_back(newItem);
                });
        }

        if (Pointers::ItemDb::GetByIndex && Pointers::ItemRecord::get_IsRoyale)
        {
            for (int i = 1; i <= 1900; i++)
            {
                int candidateIndex = i * 1000 + 1;
                IL2CPP::Object* recordObj = Pointers::ItemDb::GetByIndex(candidateIndex);
                if (!recordObj) continue;
                if (!Pointers::ItemRecord::get_IsRoyale(recordObj)) continue;

                for (auto& item : CachedItems) {
                    if (item.Index == candidateIndex) {
                        item.IsShovel = true;
                        break;
                    }
                }
            }
        }

        std::cout << "[DONE] LoadItemsToMemory complete. CachedItems total = " << CachedItems.size() << "\n";
    }

    void DumpAllItemsJson()
    {
        LoadItemsToMemory();

        std::string currentPath = std::filesystem::current_path().string();
        std::ofstream dumpStream(currentPath + "\\NovaOITDump.json", std::ios_base::trunc);

        if (CachedItems.empty()) {
            dumpStream << "[]";
            return;
        }

        dumpStream << "[\n";

        std::string currentGroup = "";
        bool firstGroup = true;
        bool firstItemInGroup = true;

        for (size_t i = 0; i < CachedItems.size(); ++i)
        {
            const auto& item = CachedItems[i];

            if (item.GroupCategory != currentGroup)
            {
                if (!firstGroup) {
                    dumpStream << "\n    ]\n  },\n";
                }

                dumpStream << "  {\n";
                dumpStream << "    \"Category\": \"" << item.GroupCategory << "\",\n";
                dumpStream << "    \"TypeID\": " << item.TypeID << ",\n";
                dumpStream << "    \"Items\": [\n";

                currentGroup = item.GroupCategory;
                firstGroup = false;
                firstItemInGroup = true;
            }

            if (!firstItemInGroup) {
                dumpStream << ",\n";
            }

            dumpStream << "      {";
            dumpStream << "\"Index\": " << item.Index << ", ";
            dumpStream << "\"ID\": \"" << EscapeJsonString(item.ID) << "\", ";
            dumpStream << "\"Name\": \"" << EscapeJsonString(item.Name) << "\"";

            if (!item.InGameCategory.empty()) {
                dumpStream << ", \"Category\": \"" << EscapeJsonString(item.InGameCategory) << "\"";
            }

            if (item.TypeID == 10) {
                if (!item.Rarity.empty()) dumpStream << ", \"Rarity\": \"" << EscapeJsonString(item.Rarity) << "\"";
                dumpStream << ", \"IsCraftable\": " << (item.IsCraftable ? "true" : "false");
                if (item.GalleryNumber != -1) dumpStream << ", \"GalleryNumber\": " << item.GalleryNumber;
                if (item.IsShovel) dumpStream << ", \"IsShovel\": true";
            }

            if (item.IsRoyale) {
                dumpStream << ", \"IsRoyale\": true";
            }

            dumpStream << "}";
            firstItemInGroup = false;
        }

        dumpStream << "\n    ]\n  }\n]";
        dumpStream.flush();
        dumpStream.close();
    }

    void DumpMemoryToFile()
    {
        std::string currentPath = std::filesystem::current_path().string();
        std::ofstream out(currentPath + "\\offermemorydump.txt", std::ios_base::trunc);

        out << "Total CachedItems: " << CachedItems.size() << "\n\n";

        std::map<int, int> typeCount;
        for (const auto& item : CachedItems)
            typeCount[item.TypeID]++;

        out << "=== TYPE ID COUNTS ===\n";
        for (const auto& [typeID, count] : typeCount)
            out << "TypeID " << typeID << " (" << count << " items)\n";

        out << "\n=== ALL ITEMS ===\n";
        for (const auto& item : CachedItems)
        {
            out << "[TypeID=" << item.TypeID << "] "
                << "Index=" << item.Index << " | "
                << "ID=" << item.ID << " | "
                << "Name=" << item.Name << " | "
                << "Category=" << item.InGameCategory << " | "
                << "Rarity=" << item.Rarity << " | "
                << "GalleryNum=" << item.GalleryNumber << " | "
                << "IsCraftable=" << (item.IsCraftable ? "true" : "false") << " | "
                << "IsRoyale=" << (item.IsRoyale ? "true" : "false") << " | "
                << "IsShovel=" << (item.IsShovel ? "true" : "false") << "\n";
        }

        out.flush();
        out.close();
    }
}
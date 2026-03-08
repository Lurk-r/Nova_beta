#include "Global.hpp"
#include "WebSocket.hpp"
#include "Websocket/Commands.hpp"
#include "../Utils/Il2CppHelper.hpp"
#include "../Variables/Variables.hpp"
#include "../Game/Data/SlotsHelper.hpp"
#include <ImGui/Notify/imgui_notify.hpp>
#include <Obfusheader.hpp>
#include "Data/OfferItemDumper.hpp"
#include "Data/AdderLists.hpp"
#include "Login.hpp"
#include "Extras.hpp"
#include "Gameplay.hpp"
#include <algorithm>
#include <set>

namespace V_W = Variables::Websocket;
namespace V_M = Variables::Miscellaneous;
namespace WS = WebSocket::Socket;
#define SNC SendNotifCommand

static std::vector<std::function<void()>> CallInUnityThread;

static int BannedIndexes[] = {
    1702001, 1703001, 1704001, 1201005,
    496014, 497014, 498014, 499014, 500014, 501014, 502014, 503014, 504014, 505014, 506014, 507014, 508014, 509014, 510014, 511014, 512014, 513014,
    531014, 532014, 534014, 535014, 537014, 538014, 539014, 540014, 541014, 547014
};

static int TutorialIndexes[] = {
    1100, 1200, 1300, 1400, 1500, 1600, 1700, 1800, 1900, 2000, 2100, 2200, 2300, 2400, 2500, 2600, 2700, 2800
};

static bool IsBannedIndex(int index)
{
    for (int bannedIndex : BannedIndexes)
    {
        if (index == bannedIndex)
            return true;
    }
    return false;
}

static bool IsTutorialComplete(int tutorialIndex)
{
    json tutorial = SlotsHelper::GetParsedData(true, OBF("tutorial"));
    if (tutorial.is_array())
    {
        std::string tutorialStr = std::to_string(tutorialIndex);
        for (const auto& item : tutorial)
        {
            if (item.is_string() && item.get<std::string>() == tutorialStr)
                return true;
        }
    }
    return false;
}

namespace Notifications
{
    void Success(std::string message, int time = 3000)
    {
        ImGui::InsertNotification({ ImGuiToastType::Success, time, message.c_str() });
    }
    void Info(std::string message, int time = 3000)
    {
        ImGui::InsertNotification({ ImGuiToastType::Info, time, message.c_str() });
    }
}

namespace Global
{
    namespace ProgressSyncController
    {
        void(*DoSyncSafe_o)(IL2CPP::Object*) = nullptr;

        void DoSyncSafe_Detour(IL2CPP::Object* arg)
        {
            DoSyncSafe_o(arg);
        }
    }

    namespace ExperienceController
    {
        void(*AddXP_o)(IL2CPP::Object*, int, int, IL2CPP::Object*, IL2CPP::Object*) = nullptr;
        int(*GetLevel_o)() = nullptr;

        void AddXP(IL2CPP::Object* instance, int a, int b, IL2CPP::Object* c, IL2CPP::Object* d)
        {
            return AddXP_o(instance, a, b, c, d);
        }

        int GetLevel()
        {
            return GetLevel_o();
        }
    }

    namespace PixelTime
    {
        void(*Update_o)(IL2CPP::Object*) = nullptr;

        void Update(IL2CPP::Object* instance)
        {
            if (instance)
            {
                for (auto& func : CallInUnityThread)
                    func();
                CallInUnityThread.clear();

                V_M::m_sGameVersion = Pointers::MainPointers::GetVersion()->ToString();
                V_M::m_sPlayerID = Pointers::MainPointers::GetID()->ToString();
                V_M::m_sPlayerUsername = Pointers::MainPointers::GetUsername()->ToString();
                V_M::m_iPlayerLevel = Pointers::MainPointers::GetLevel();

                if (V_W::m_bReloadSocket)
                {
                    WebSocket::Socket::SendCommand(Commands::Reload());
                    ImGui::InsertNotification({ ImGuiToastType::Info, 3000, OBF("Reloading socket...") });
                    V_W::m_bReloadSocket = false;
                }

                if (V_W::m_bSendCustomCommand)
                {
                    std::string eventName = V_W::m_cCustomEventName;
                    std::string payload = V_W::m_cCustomJson;
                    V_W::m_bSendCustomCommand = false;

                    if (eventName.empty())
                    {
                        ImGui::InsertNotification({ ImGuiToastType::Error, 3000, OBF("Event Name Invalid!") });
                    }
                    else
                    {
                        try
                        {
                            json parsedPayload = json::parse(payload);
                            json packet = json::array();
                            packet.push_back(eventName);
                            packet.push_back(parsedPayload);
                            WS::SNC(OBF("Custom Command"), packet, false);
                        }
                        catch (...)
                        {
                            ImGui::InsertNotification({ ImGuiToastType::Error, 3000, OBF("Json Invalid!") });
                        }
                    }
                }

                if (V_W::m_bAddCurrency)
                {
                    WS::SNC(OBF("Add Currency"), Commands::Currency());
                    V_W::m_bAddCurrency = false;
                }

                if (V_W::m_bSpendCurrency)
                {
                    WS::SNC(OBF("Spend Currency"), Commands::Currency(true));
                    V_W::m_bSpendCurrency = false;
                }

                if (V_W::m_bAddWeapon)
                {
                    const AdderLists::WeaponInfo* weapon = nullptr;
                    if (V_W::m_iWeaponSearchType == 0)
                        weapon = AdderLists::FindByName(V_W::m_cWeaponSearchBuffer);
                    else if (V_W::m_iWeaponSearchType == 1)
                        weapon = AdderLists::FindByGallery(atoi(V_W::m_cWeaponSearchBuffer));
                    else if (V_W::m_iWeaponSearchType == 2)
                        weapon = AdderLists::FindByIndex(atoi(V_W::m_cWeaponSearchBuffer));

                    if (weapon)
                    {
                        int finalRarity = V_W::m_bUseDefaultRarity
                            ? AdderLists::GetRarityValue(weapon->Rarity)
                            : V_W::m_iWeaponRarity;
                        WS::SNC(OBF("Add Weapon"),
                            Commands::AddWeapon(weapon->Index, V_W::m_iWeaponLevel, finalRarity));
                    }
                    V_W::m_bAddWeapon = false;
                }

                if (V_W::m_bRemoveWeapon)
                {
                    const AdderLists::WeaponInfo* weapon = nullptr;
                    if (V_W::m_iWeaponSearchType == 0)
                        weapon = AdderLists::FindByName(V_W::m_cWeaponSearchBuffer);
                    else if (V_W::m_iWeaponSearchType == 1)
                        weapon = AdderLists::FindByGallery(atoi(V_W::m_cWeaponSearchBuffer));
                    else if (V_W::m_iWeaponSearchType == 2)
                        weapon = AdderLists::FindByIndex(atoi(V_W::m_cWeaponSearchBuffer));

                    if (weapon)
                    {
                        WS::SNC(OBF("Remove Weapon"), Commands::RemoveWeapon(weapon->Index));
                    }
                    V_W::m_bRemoveWeapon = false;
                }

                if (V_W::m_bAddAllPrimary)
                {
                    std::vector<int> indices;
                    for (const auto& w : AdderLists::PrimaryWeapons) indices.push_back(w.Index);
                    WS::SNC(OBF("Add All Primary"),
                        Commands::AddAllWeapons(indices, V_W::m_iWeaponLevel,
                            V_W::m_iWeaponRarity, V_W::m_bUseDefaultRarity));
                    V_W::m_bAddAllPrimary = false;
                }

                if (V_W::m_bAddAllBackup)
                {
                    std::vector<int> indices;
                    for (const auto& w : AdderLists::BackupWeapons) indices.push_back(w.Index);
                    WS::SNC(OBF("Add All Backup"),
                        Commands::AddAllWeapons(indices, V_W::m_iWeaponLevel,
                            V_W::m_iWeaponRarity, V_W::m_bUseDefaultRarity));
                    V_W::m_bAddAllBackup = false;
                }

                if (V_W::m_bAddAllMelee)
                {
                    std::vector<int> indices;
                    for (const auto& w : AdderLists::MeleeWeapons) indices.push_back(w.Index);
                    WS::SNC(OBF("Add All Melee"),
                        Commands::AddAllWeapons(indices, V_W::m_iWeaponLevel,
                            V_W::m_iWeaponRarity, V_W::m_bUseDefaultRarity));
                    V_W::m_bAddAllMelee = false;
                }

                if (V_W::m_bAddAllSpecial)
                {
                    std::vector<int> indices;
                    for (const auto& w : AdderLists::SpecialWeapons) indices.push_back(w.Index);
                    WS::SNC(OBF("Add All Special"),
                        Commands::AddAllWeapons(indices, V_W::m_iWeaponLevel,
                            V_W::m_iWeaponRarity, V_W::m_bUseDefaultRarity));
                    V_W::m_bAddAllSpecial = false;
                }

                if (V_W::m_bAddAllSniper)
                {
                    std::vector<int> indices;
                    for (const auto& w : AdderLists::SniperWeapons) indices.push_back(w.Index);
                    WS::SNC(OBF("Add All Sniper"),
                        Commands::AddAllWeapons(indices, V_W::m_iWeaponLevel,
                            V_W::m_iWeaponRarity, V_W::m_bUseDefaultRarity));
                    V_W::m_bAddAllSniper = false;
                }

                if (V_W::m_bAddAllHeavy)
                {
                    std::vector<int> indices;
                    for (const auto& w : AdderLists::HeavyWeapons) indices.push_back(w.Index);
                    WS::SNC(OBF("Add All Heavy"),
                        Commands::AddAllWeapons(indices, V_W::m_iWeaponLevel,
                            V_W::m_iWeaponRarity, V_W::m_bUseDefaultRarity));
                    V_W::m_bAddAllHeavy = false;
                }

                if (V_W::m_bRemoveAllPrimary)
                {
                    std::vector<int> indices;
                    for (const auto& w : AdderLists::PrimaryWeapons) indices.push_back(w.Index);
                    WS::SNC(OBF("Remove All Primary"), Commands::RemoveAllWeapons(indices));
                    V_W::m_bRemoveAllPrimary = false;
                }

                if (V_W::m_bRemoveAllBackup)
                {
                    std::vector<int> indices;
                    for (const auto& w : AdderLists::BackupWeapons) indices.push_back(w.Index);
                    WS::SNC(OBF("Remove All Backup"), Commands::RemoveAllWeapons(indices));
                    V_W::m_bRemoveAllBackup = false;
                }

                if (V_W::m_bRemoveAllMelee)
                {
                    std::vector<int> indices;
                    for (const auto& w : AdderLists::MeleeWeapons) indices.push_back(w.Index);
                    WS::SNC(OBF("Remove All Melee"), Commands::RemoveAllWeapons(indices));
                    V_W::m_bRemoveAllMelee = false;
                }

                if (V_W::m_bRemoveAllSpecial)
                {
                    std::vector<int> indices;
                    for (const auto& w : AdderLists::SpecialWeapons) indices.push_back(w.Index);
                    WS::SNC(OBF("Remove All Special"), Commands::RemoveAllWeapons(indices));
                    V_W::m_bRemoveAllSpecial = false;
                }

                if (V_W::m_bRemoveAllSniper)
                {
                    std::vector<int> indices;
                    for (const auto& w : AdderLists::SniperWeapons) indices.push_back(w.Index);
                    WS::SNC(OBF("Remove All Sniper"), Commands::RemoveAllWeapons(indices));
                    V_W::m_bRemoveAllSniper = false;
                }

                if (V_W::m_bRemoveAllHeavy)
                {
                    std::vector<int> indices;
                    for (const auto& w : AdderLists::HeavyWeapons) indices.push_back(w.Index);
                    WS::SNC(OBF("Remove All Heavy"), Commands::RemoveAllWeapons(indices));
                    V_W::m_bRemoveAllHeavy = false;
                }

                if (V_W::m_bAddAllWeapons)
                {
                    std::vector<int> indices;
                    for (const auto& w : AdderLists::AllWeapons) indices.push_back(w.Index);

                    std::cout << "[Weapons] AllWeapons count: " << AdderLists::AllWeapons.size() << std::endl;
                    std::cout << "[Weapons] Indices to send (" << indices.size() << "): ";
                    for (int idx : indices) std::cout << idx << " ";
                    std::cout << std::endl;

                    WS::SNC(OBF("Add All Weapons"),
                        Commands::AddAllWeapons(indices, V_W::m_iWeaponLevel,
                            V_W::m_iWeaponRarity, V_W::m_bUseDefaultRarity));
                    V_W::m_bAddAllWeapons = false;
                }

                if (V_W::m_bRemoveAllWeapons)
                {
                    std::vector<int> indices;
                    for (const auto& w : AdderLists::AllWeapons) indices.push_back(w.Index);
                    WS::SNC(OBF("Remove All Weapons"), Commands::RemoveAllWeapons(indices));
                    V_W::m_bRemoveAllWeapons = false;
                }

                if (V_W::m_bAddAllCraftable)
                {
                    std::vector<int> indices;
                    for (const auto& w : AdderLists::CraftableWeapons) indices.push_back(w.Index);
                    WS::SNC(OBF("Add All Craftable"),
                        Commands::AddAllWeapons(indices, V_W::m_iWeaponLevel,
                            V_W::m_iWeaponRarity, V_W::m_bUseDefaultRarity));
                    V_W::m_bAddAllCraftable = false;
                }

                if (V_W::m_bAddAllWeaponParts)
                {
                    std::vector<int> indices;
                    for (const auto& w : AdderLists::CraftableWeapons) indices.push_back(w.Index);
                    WS::SNC(OBF("Add All Weapon Parts"),
                        Commands::AddAllWeaponParts(indices, V_W::m_iWeaponPartAmount));
                    V_W::m_bAddAllWeaponParts = false;
                }

                if (V_W::m_bAddAllArmor)
                {
                    V_W::m_bAddAllArmor = false;
                    Notifications::Info("Fetching slots...");
                    SlotsHelper::CheckAndParseAllSlots(V_M::m_sPlayerID, []() {
                        Notifications::Info("Adding all armor...");
                        auto ownedArmor = SlotsHelper::GetOwnedArmor();
                        json commands = json::array();
                        for (const auto& armor : AdderLists::Armor)
                        {
                            if (IsBannedIndex(armor.Index)) continue;
                            if (std::find(ownedArmor.begin(), ownedArmor.end(), armor.Index) == ownedArmor.end())
                                commands.emplace_back(WSFunctions::CreateCommand(CommandsID::BuyArmor, { {OBF("a"), armor.Index},{OBF("u"), V_M::m_iPlayerLevel} }));
                        }
                        if (!commands.empty()) WS::SNC(OBF("Add All Armor"), WSFunctions::CreateSnapshot(commands));
                        Notifications::Success("All armor added!");
                        });
                }

                if (V_W::m_bAddAllWear)
                {
                    V_W::m_bAddAllWear = false;
                    Notifications::Info("Fetching slots...");
                    SlotsHelper::CheckAndParseAllSlots(V_M::m_sPlayerID, []() {
                        Notifications::Info("Adding all wear...");
                        auto ownedWear = SlotsHelper::GetOwnedWear();
                        json commands = json::array();
                        for (const auto& wear : AdderLists::Boots)
                        {
                            if (IsBannedIndex(wear.Index)) continue;
                            if (std::find(ownedWear.begin(), ownedWear.end(), wear.Index) == ownedWear.end())
                                commands.emplace_back(WSFunctions::CreateCommand(CommandsID::AddPurchasedWear, { {OBF("w"), "a*" + std::to_string(wear.Index)} }));
                        }
                        for (const auto& wear : AdderLists::Capes)
                        {
                            if (IsBannedIndex(wear.Index)) continue;
                            if (std::find(ownedWear.begin(), ownedWear.end(), wear.Index) == ownedWear.end())
                                commands.emplace_back(WSFunctions::CreateCommand(CommandsID::AddPurchasedWear, { {OBF("w"), "a*" + std::to_string(wear.Index)} }));
                        }
                        for (const auto& wear : AdderLists::Hats)
                        {
                            if (IsBannedIndex(wear.Index)) continue;
                            if (std::find(ownedWear.begin(), ownedWear.end(), wear.Index) == ownedWear.end())
                                commands.emplace_back(WSFunctions::CreateCommand(CommandsID::AddPurchasedWear, { {OBF("w"), "a*" + std::to_string(wear.Index)} }));
                        }
                        for (const auto& wear : AdderLists::Masks)
                        {
                            if (IsBannedIndex(wear.Index)) continue;
                            if (std::find(ownedWear.begin(), ownedWear.end(), wear.Index) == ownedWear.end())
                                commands.emplace_back(WSFunctions::CreateCommand(CommandsID::AddPurchasedWear, { {OBF("w"), "a*" + std::to_string(wear.Index)} }));
                        }
                        if (!commands.empty()) WS::SNC(OBF("Add All Wear"), WSFunctions::CreateSnapshot(commands));
                        Notifications::Success("All wear added!");
                        });
                }

                if (V_W::m_bAddAllGadgets)
                {
                    V_W::m_bAddAllGadgets = false;
                    Notifications::Info("Fetching slots...");
                    SlotsHelper::CheckAndParseAllSlots(V_M::m_sPlayerID, []() {
                        Notifications::Info("Adding all gadgets...");
                        auto ownedGadgets = SlotsHelper::GetOwnedGadgets();
                        json commands = json::array();
                        for (const auto& gadget : AdderLists::Gadgets)
                        {
                            if (IsBannedIndex(gadget.Index)) continue;
                            if (std::find(ownedGadgets.begin(), ownedGadgets.end(), gadget.Index) == ownedGadgets.end())
                                commands.emplace_back(WSFunctions::CreateCommand(CommandsID::AddPurhasedGadget, { {OBF("g"), gadget.Index},{OBF("u"), V_M::m_iPlayerLevel} }));
                        }
                        if (!commands.empty()) WS::SNC(OBF("Add All Gadgets"), WSFunctions::CreateSnapshot(commands));
                        Notifications::Success("All gadgets added!");
                        });
                }

                if (V_W::m_bAddAllPets)
                {
                    V_W::m_bAddAllPets = false;
                    Notifications::Info("Fetching slots...");
                    SlotsHelper::CheckAndParseAllSlots(V_M::m_sPlayerID, []() {
                        Notifications::Info("Adding all pets...");
                        auto ownedPets = SlotsHelper::GetOwnedPets();
                        json commands = json::array();
                        for (const auto& pet : AdderLists::Pets)
                        {
                            if (IsBannedIndex(pet.Index)) continue;
                            if (std::find(ownedPets.begin(), ownedPets.end(), pet.Index) == ownedPets.end())
                                commands.emplace_back(WSFunctions::CreateCommand(CommandsID::AddPetV3, { {OBF("i"), pet.Index},{OBF("n"), "leminare.dev"} }));
                        }
                        if (!commands.empty()) WS::SNC(OBF("Add All Pets"), WSFunctions::CreateSnapshot(commands));
                        Notifications::Success("All pets added!");
                        });
                }

                if (V_W::m_bAddAllModules)
                {
                    V_W::m_bAddAllModules = false;
                    Notifications::Info("Fetching slots...");
                    SlotsHelper::CheckAndParseAllSlots(V_M::m_sPlayerID, []() {
                        Notifications::Info("Adding module parts...");
                        auto ownedModules = SlotsHelper::GetOwnedModules();

                        json addCommands = json::array();

                        for (int module_index = 346; module_index <= 387; ++module_index)
                        {
                            int moduleIndex = module_index * 1000 + 9;
                            addCommands.emplace_back(WSFunctions::CreateCommand(CommandsID::InventoryAddItemConsumable, {
                                {OBF("t"), 1155},
                                {OBF("i"), moduleIndex},
                                {OBF("c"), 1},
                                {OBF("sc"), 1}
                                }));
                        }

                        WebSocket::Socket::SendCommand(WSFunctions::CreateSnapshot(addCommands), [ownedModules](Structs::Socket::Response packet) {
                            if (packet.ResponseStatus != OBF("OK"))
                            {
                                Notifications::Info("Failed to add module parts!");
                                return;
                            }

                            Notifications::Success("Module parts added!");
                            Extras::HandleForceSync();

                            json upgradeCommands = json::array();
                            for (int module_index = 304; module_index <= 345; ++module_index)
                            {
                                int moduleIndex = module_index * 1000 + 9;
                                if (IsBannedIndex(moduleIndex)) continue;
                                bool isOwned = std::find(ownedModules.begin(), ownedModules.end(), moduleIndex) != ownedModules.end();
                                int upgrades = isOwned ? (10 - SlotsHelper::GetModuleLevel(moduleIndex)) : 9;
                                for (int i = 0; i < upgrades; ++i)
                                {
                                    upgradeCommands.emplace_back(WSFunctions::CreateCommand(CommandsID::SpendCurrency, {
                                        {OBF("c"), OBF("Coins")},
                                        {OBF("v"), 0},
                                        {OBF("ca"), SpendCurrencyCause::UpgradeModule}
                                        }));
                                    upgradeCommands.emplace_back(WSFunctions::CreateCommand(CommandsID::ModuleInfoIncreaseUp, { {OBF("i"), moduleIndex} }));
                                }
                            }

                            Notifications::Info("Upgrading all modules...");
                            WebSocket::Socket::SendCommand(WSFunctions::CreateSnapshot(upgradeCommands), [](Structs::Socket::Response packet) {
                                if (packet.ResponseStatus != OBF("OK"))
                                {
                                    Notifications::Info("Failed to upgrade modules!");
                                    return;
                                }
                                Notifications::Success("All Modules Added and Upgraded Successfully!");
                                Extras::HandleForceSync();
                                }, true);
                            }, true);
                        });
                }

                if (V_W::m_bAddAllVehicles)
                {
                    V_W::m_bAddAllVehicles = false;
                    Notifications::Info("Fetching slots...");
                    SlotsHelper::CheckAndParseAllSlots(V_M::m_sPlayerID, []() {
                        Notifications::Info("Adding all vehicles...");
                        auto ownedVehicles = SlotsHelper::GetOwnedVehicles();
                        json commands = json::array();
                        for (int idx : AdderLists::GliderIndexes)
                        {
                            if (IsBannedIndex(idx)) continue;
                            if (std::find(ownedVehicles.begin(), ownedVehicles.end(), idx) == ownedVehicles.end())
                                commands.emplace_back(WSFunctions::CreateCommand(CommandsID::UpdateBRVehicle, { {OBF("i"), idx},{OBF("u"), 4} }));
                        }
                        for (int idx : AdderLists::CarIndexes)
                        {
                            if (IsBannedIndex(idx)) continue;
                            if (std::find(ownedVehicles.begin(), ownedVehicles.end(), idx) == ownedVehicles.end())
                                commands.emplace_back(WSFunctions::CreateCommand(CommandsID::UpdateBRVehicle, { {OBF("i"), idx},{OBF("u"), 4} }));
                        }
                        if (!commands.empty()) WS::SNC(OBF("Add All Vehicles"), WSFunctions::CreateSnapshot(commands));
                        Notifications::Success("All vehicles added!");
                        });
                }

                if (V_W::m_bAddAllAvatars)
                {
                    V_W::m_bAddAllAvatars = false;
                    Notifications::Info("Adding all avatars...");
                    json commands = json::array();
                    for (int idx : AdderLists::AvatarIndexes)
                    {
                        if (IsBannedIndex(idx)) continue;
                        commands.emplace_back(WSFunctions::CreateCommand(CommandsID::UpdateBattleRoyaleList, { {OBF("1"), idx} }));
                    }
                    if (!commands.empty()) WS::SNC(OBF("Add All Avatars"), WSFunctions::CreateSnapshot(commands));
                    Notifications::Success("All avatars added!");
                }

                if (V_W::m_bAddAllTrails)
                {
                    V_W::m_bAddAllTrails = false;
                    Notifications::Info("Adding all trails...");
                    json commands = json::array();
                    for (int idx : AdderLists::TrailIndexes)
                    {
                        if (IsBannedIndex(idx)) continue;
                        commands.emplace_back(WSFunctions::CreateCommand(CommandsID::UpdatePlayerEquipment, { {OBF("4"), {{OBF("lb"), idx}}} }));
                    }
                    if (!commands.empty()) WS::SNC(OBF("Add All Trails"), WSFunctions::CreateSnapshot(commands));
                    Notifications::Success("All trails added!");
                }

                if (V_W::m_bAddAllWeaponSkins)
                {
                    V_W::m_bAddAllWeaponSkins = false;
                    Notifications::Info("Adding all weapon skins...");
                    json commands = json::array();
                    for (const auto& skin : AdderLists::WearSkins)
                    {
                        if (IsBannedIndex(skin.Index)) continue;
                        commands.emplace_back(WSFunctions::CreateCommand(CommandsID::InventoryAddItemSingle, { {OBF("i"), skin.Index} }));
                    }
                    if (!commands.empty()) WS::SNC(OBF("Add All Weapon Skins"), WSFunctions::CreateSnapshot(commands));
                    Notifications::Success("All weapon skins added!");
                }

                if (V_W::m_bAddAllGraffiti)
                {
                    V_W::m_bAddAllGraffiti = false;
                    Notifications::Info("Adding all profile items...");
                    json commands = json::array();
                    for (const auto& g : AdderLists::Graffiti)
                    {
                        if (IsBannedIndex(g.Index)) continue;
                        commands.emplace_back(WSFunctions::CreateCommand(CommandsID::InventoryAddItemSingle, { {OBF("i"), g.Index} }));
                    }
                    for (const auto& i : AdderLists::Icons)
                    {
                        if (IsBannedIndex(i.Index)) continue;
                        commands.emplace_back(WSFunctions::CreateCommand(CommandsID::InventoryAddItemSingle, { {OBF("i"), i.Index} }));
                    }
                    for (const auto& f : AdderLists::Frames)
                    {
                        if (IsBannedIndex(f.Index)) continue;
                        commands.emplace_back(WSFunctions::CreateCommand(CommandsID::InventoryAddItemSingle, { {OBF("i"), f.Index} }));
                    }
                    for (const auto& b : AdderLists::Backgrounds)
                    {
                        if (IsBannedIndex(b.Index)) continue;
                        commands.emplace_back(WSFunctions::CreateCommand(CommandsID::InventoryAddItemSingle, { {OBF("i"), b.Index} }));
                    }
                    if (!commands.empty()) WS::SNC(OBF("Add All Profile Items"), WSFunctions::CreateSnapshot(commands));
                    Notifications::Success("All profile items added!");
                }

                if (V_W::m_bAddAllFreeUpgrades)
                {
                    V_W::m_bAddAllFreeUpgrades = false;
                    Notifications::Info("Adding all free upgrades...");
                    json commands = json::array();
                    for (const auto& upgrade : AdderLists::FreeUpgrades)
                    {
                        if (IsBannedIndex(upgrade.Index)) continue;
                        commands.emplace_back(WSFunctions::CreateCommand(CommandsID::InventoryAddItemConsumable, { {OBF("t"), 1380},{OBF("i"), upgrade.Index},{OBF("c"), 999} }));
                    }
                    if (!commands.empty()) WS::SNC(OBF("Add All Free Upgrades"), WSFunctions::CreateSnapshot(commands));
                    Notifications::Success("All free upgrades added!");
                }

                if (V_W::m_bAddArmor)
                {
                    WS::SNC(OBF("Add Armor"), Commands::AddArmor(V_W::m_iItemIndex, V_M::m_iPlayerLevel));
                    V_W::m_bAddArmor = false;
                }

                if (V_W::m_bAddWear)
                {
                    V_W::m_bAddWear = false;
                    SlotsHelper::CheckAndParseAllSlots(V_M::m_sPlayerID, []() {
                        if (!SlotsHelper::HasWear(V_W::m_iItemIndex))
                            WS::SNC(OBF("Add Wear"), Commands::AddWear(V_W::m_iItemIndex));
                        else
                            Notifications::Info("Wear Already Owned!");
                        });
                }

                if (V_W::m_bAddGadget)
                {
                    WS::SNC(OBF("Add Gadget"), Commands::AddGadget(V_W::m_iItemIndex, V_M::m_iPlayerLevel));
                    V_W::m_bAddGadget = false;
                }

                if (V_W::m_bAddVehicle)
                {
                    WS::SNC(OBF("Add Vehicle"), Commands::AddVehicle(V_W::m_iItemIndex, 4));
                    V_W::m_bAddVehicle = false;
                }

                if (V_W::m_bAddPet)
                {
                    V_W::m_bAddPet = false;
                    SlotsHelper::CheckAndParseAllSlots(V_M::m_sPlayerID, []() {
                        if (!SlotsHelper::HasPet(V_W::m_iItemIndex))
                            WS::SNC(OBF("Add Pet"), Commands::AddPet(V_W::m_iItemIndex));
                        else
                            Notifications::Info("Pet Already Owned!");
                        });
                }

                if (V_W::m_bAddAvatar)
                {
                    WS::SNC(OBF("Add Avatar"), Commands::AddAvatar(V_W::m_iItemIndex));
                    V_W::m_bAddAvatar = false;
                }

                if (V_W::m_bAddTrail)
                {
                    WS::SNC(OBF("Add Trail"), Commands::AddTrail(V_W::m_iItemIndex));
                    V_W::m_bAddTrail = false;
                }

                if (V_W::m_bAddItemSingle)
                {
                    WS::SNC(OBF("Add Item"), Commands::AddItemSingle(V_W::m_iItemIndex));
                    V_W::m_bAddItemSingle = false;
                }

                if (V_W::m_bSendFriendRequest)
                {
                    V_W::m_bSendFriendRequest = false;
                    WS::SNC(OBF("Send Friend Request"), Commands::SendFriendRequest(V_W::m_cFriendID), false);
                    Notifications::Success("Friend Request Sent!");
                }

                if (V_W::m_bSendClanMessage)
                {
                    V_W::m_bSendClanMessage = false;
                    WS::SNC(OBF("Send Clan Message"), Commands::SendClanMessage(V_W::m_cClanMessage), false);
                    Notifications::Success("Clan Message Sent!");
                }

                if (V_W::m_bCompleteTutorial)
                {
                    V_W::m_bCompleteTutorial = false;
                    Notifications::Info("Fetching slots...");
                    SlotsHelper::CheckAndParseAllSlots(V_M::m_sPlayerID, []() {
                        Notifications::Info("Completing tutorial...");
                        json tutorialData = SlotsHelper::GetParsedData(true, OBF("tutorial"));
                        std::vector<std::string> completedSteps;
                        if (tutorialData.is_array())
                            for (const auto& item : tutorialData)
                                if (item.is_string()) completedSteps.push_back(item.get<std::string>());

                        json commands = json::array();
                        for (int idx : TutorialIndexes)
                        {
                            if (std::find(completedSteps.begin(), completedSteps.end(), std::to_string(idx)) == completedSteps.end())
                                commands.emplace_back(WSFunctions::CreateCommand(CommandsID::TutorialComplete, { {OBF("t"), idx} }));
                        }
                        if (!commands.empty()) WS::SNC(OBF("Complete Tutorial"), WSFunctions::CreateSnapshot(commands));
                        Notifications::Success("Tutorial completed!");
                        });
                }

                if (V_W::m_bSetLevel)
                {
                    V_W::m_bSetLevel = false;
                    Notifications::Info("Fetching slots...");
                    SlotsHelper::CheckAndParseAllSlots(V_M::m_sPlayerID, []() {
                        std::set<int> alreadyClaimed;
                        json claimedJson = SlotsHelper::GetParsedData(true, OBF("claimed_rewards"));
                        if (claimedJson.is_object())
                        {
                            for (auto it = claimedJson.items().begin(); it != claimedJson.items().end(); ++it)
                            {
                                try { alreadyClaimed.insert(std::stoi(it.key())); }
                                catch (...) {}
                            }
                        }

                        Notifications::Info("Claiming " + std::to_string(alreadyClaimed.size()) + " already done, sending rest...");
                        WS::SNC(OBF("Making Level " + std::to_string(V_W::m_iSetLevel) + " safe"), Commands::ClaimSafeRewards(alreadyClaimed));

                        Notifications::Info("Setting level to " + std::to_string(V_W::m_iSetLevel) + "...", 5000);
                        json snapshot = Commands::SetPlayerLevel(V_W::m_iSetLevel, 0);
                        WebSocket::Socket::SendCommand(snapshot, [](Structs::Socket::Response packet) {
                            if (packet.ResponseStatus == OBF("OK"))
                            {
                                Notifications::Success("Level set!", 5000);
                                Extras::HandleForceSync();
                                std::thread([]() {
                                    std::this_thread::sleep_for(std::chrono::seconds(2));
                                    CallInUnityThread.emplace_back([]() {
                                        Extras::SecureLevel();
                                        Notifications::Success("Level Secured!", 5000);
                                        });
                                    }).detach();
                            }
                            else
                            {
                                Notifications::Info("Failed to set level!");
                            }
                            }, true);
                        });
                }


                if (V_M::DumpOfferItemTypes)
                {
                    V_M::DumpOfferItemTypes = false;
                    OfferItemDumper::DumpAllItemsJson();
                    ImGui::InsertNotification({ ImGuiToastType::Info, 3000, OBF("Dumped OIT's") });
                }

                if (V_M::DumpOfferItemTypesInitial)
                {
                    V_M::DumpOfferItemTypesInitial = false;

                    Notifications::Info("Loading Adder Lists...");
                    OfferItemDumper::LoadItemsToMemory();
                    OfferItemDumper::DumpMemoryToFile();
                    AdderLists::LoadListsFromCache();
                    Notifications::Success("Adder Lists Loaded!");
                }

                if (V_M::m_bCheckAllMySlots)
                {
                    V_M::m_bCheckAllMySlots = false;
                    SlotsHelper::CheckAndParseAllSlots(V_M::m_sPlayerID);
                    Notifications::Info("Grabbing our progress");
                }

                else if (V_M::m_bCheckAllOtherPlayerSlots)
                {
                    V_M::m_bCheckAllOtherPlayerSlots = false;
                    SlotsHelper::CheckAndParseAllSlots(V_M::m_sOtherPlayerID);
                    Notifications::Info("Grabbing there progress");
                }

                if (V_W::m_bCloneName)
                {
                    V_W::m_bCloneName = false;
                    json nameJson = SlotsHelper::GetParsedData(false, OBF("name"));
                    if (!nameJson.is_null() && nameJson.is_string())
                    {
                        std::string name = nameJson.get<std::string>();
                        WS::SNC(OBF("Clone Name"), Commands::SetName(name));
                    }
                    else
                    {
                        Notifications::Info("Name not found in Other Slots!");
                    }
                }

                if (V_W::m_bCloneSkins)
                {
                    V_W::m_bCloneSkins = false;

                    json skinsArray = SlotsHelper::GetParsedData(false, OBF("skins"));
                    if (skinsArray.is_array())
                    {
                        int count = 0;
                        for (const auto& skin : skinsArray)
                        {
                            if (skin.contains(OBF("data")) && skin[OBF("data")].is_string())
                            {
                                std::string data = skin[OBF("data")].get<std::string>();
                                std::string newId = RandomDigits(18);
                                WS::SNC(OBF("Add Skin"), Commands::SetSkins(newId, data));
                                count++;
                            }
                        }
                        Notifications::Success("Cloned %d skins", count);
                    }
                    else
                    {
                        Notifications::Info("Skins not found in Slots..");
                    }
                }

                if (V_W::m_bCloneCape)
                {
                    V_W::m_bCloneCape = false;
                    json capeData = SlotsHelper::GetParsedData(false, OBF("cape"));
                    if (!capeData.is_null() && capeData.is_string())
                    {
                        std::string data = capeData.get<std::string>();
                        WS::SNC(OBF("Set Cape"), Commands::SetCape(data));
                        Notifications::Success("Cape Cloned!");
                    }
                    else
                    {
                        Notifications::Info("Cape not found in Slots..");
                    }
                }

                if (V_M::m_bDumpGame)
                {
                    V_M::m_bDumpGame = false;

                    std::string filename = "NovaDump-" + Variables::Miscellaneous::m_sGameVersion + ".cs";
                    if (!Il2CppDumper::is_dumping)
                    {
                        Notifications::Info("Starting dump...");
                        Il2CppDumper::DumpAsync(filename.c_str());
                    }
                }

                if (Variables::Miscellaneous::m_bOutputToken)
                {
                    Variables::Miscellaneous::m_bOutputToken = false;
                    Login::HandleTokenOutput();
                }

                if (Variables::Miscellaneous::m_bOutputAuthInfo)
                {
                    Variables::Miscellaneous::m_bOutputAuthInfo = false;
                    Login::HandleAuthInfoOutput();
                }

                if (Variables::Miscellaneous::m_bLoginWithToken)
                {
                    Variables::Miscellaneous::m_bLoginWithToken = false;
                    Login::HandleLoginWithToken();
                }

                if (Variables::Miscellaneous::m_bLoginWithIDSecret)
                {
                    Variables::Miscellaneous::m_bLoginWithIDSecret = false;
                    Login::HandleLoginWithIDSecret();
                }

                if (Variables::Miscellaneous::m_bForceSync)
                {
                    Variables::Miscellaneous::m_bForceSync = false;
                    Extras::HandleForceSync();
                }

                if (Variables::Miscellaneous::m_bAddXP)
                {
                    Variables::Miscellaneous::m_bAddXP = false;
                    Extras::HandleAddXP();
                }

                if (Variables::Miscellaneous::m_bSkinTest)
                {
                    Variables::Miscellaneous::m_bSkinTest = false;
                    Extras::HandleSkinTest();
                }

                if (Variables::Miscellaneous::m_bDumpEventNames)
                {
                    Variables::Miscellaneous::m_bDumpEventNames = false;
                    Extras::HandleEventDump();
                }
               
            }
            Update_o(instance);
        }

        void Init()
        {
            MakeAHookHerePls("PixelTime", "Update", Update, (LPVOID*)&Update_o);

            MakeAHookHerePls("ExperienceController", 0x1c, ExperienceController::AddXP, (LPVOID*)&ExperienceController::AddXP_o);
            MakeAHookHerePls("ExperienceController", 0x6, ExperienceController::GetLevel, (LPVOID*)&ExperienceController::GetLevel_o);

            MakeAHookHereButMappedPls("ProgressSyncController", 0x5, ProgressSyncController::DoSyncSafe_Detour, (LPVOID*)&ProgressSyncController::DoSyncSafe_o);
        }
    }
}
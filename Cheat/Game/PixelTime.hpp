#pragma once
#include "Commands.hpp"
#include <Obfusheader.hpp>

namespace PixelTime
{
    // Check My Progress Slots
    void CheckMySlots(const int& slot, bool& function)
    {
        Socket::SendCommand(Commands::MySlots({slot}), [&](Structs::Socket::Response packet)
        {
            if (packet.ResponseStatus == OBF("OK"))
            {
                function = true;
            }
        });
    }

    void CheckPlayerSlots(int playerID, const std::vector<int>& slots)
    {
        Socket::SendCommand(Commands::GrabbedSlots(playerID, slots), [&](Structs::Socket::Response packet) {
            if (packet.ResponseStatus == OBF("OK")) {
                if (packet.response.is_array() && !packet.response.empty())
                {
                    const auto& responseData = packet.response[0];
                    if (responseData.contains("slots"))
                    {
                        const auto& slotsData = responseData["slots"];
                        Variables::GrabbedPlayerProgress = slotsData.dump(4);

                        //printf("\n--- PLAYER PROGRESS CAPTURED ---\n%s\n--- END DUMP ---\n\n", Variables::GrabbedPlayerProgress.c_str());
                        ImGui::InsertNotification({ ImGuiToastType::Success, 3000, OBF("Player progress captured successfully!") });
                        return;
                    }
                }
                ImGui::InsertNotification({ ImGuiToastType::Error, 3000, OBF("format invalid nigger") });

            }
            else {
                ImGui::InsertNotification({ ImGuiToastType::Error, 3000, OBF("Failed to get player progress.") });
            }
            });
    }

    // Functions
    VOID(*PixelTime_o)(IL2CPP::Object* Instance);
    VOID PixelTime(IL2CPP::Object* Instance)
    {
        if (Instance != nullptr && Pointers::SocketInstance != nullptr)
        {
            // Pointer Values
            Variables::GameVersion = Pointers::Profile::GetVersion()->ToString();
            Variables::PlayerID = Pointers::Profile::GetID()->ToString();
            Variables::PlayerUsername = Pointers::Profile::GetUsername()->ToString();
            Variables::PlayerLevel = Pointers::Profile::GetLevel();

            // Reload
            if (Variables::ReloadSocket)
            {
                Socket::SendCommand(Commands::Reload());
                ImGui::InsertNotification({ ImGuiToastType::Info, 3000, OBF("Reloading socket...") });
                Variables::ReloadSocket = false;
            }

            // Adders
            if (Variables::AddCurrency)
            {
                Socket::SendNotifCommand(OBF("Add Currency"), Commands::Currency());
                Variables::AddCurrency = false;
            }

            if (Variables::SpendCurrency)
            {
                Socket::SendNotifCommand(OBF("Spend Currency"), Commands::Currency(true));
                Variables::SpendCurrency = false;
            }

            if (Variables::LoadoutFixer)
            {
                Socket::SendNotifCommand(OBF("Resetting Loadout 1"), Commands::SetLoadout(0));
                Socket::SendNotifCommand(OBF("Resetting Loadout 2"), Commands::SetLoadout(1));
                Socket::SendNotifCommand(OBF("Resetting Loadout 3"), Commands::SetLoadout(2));
                Socket::SendNotifCommand(OBF("Resetting Default Loadout"), Commands::ResetLoadout());
                Variables::LoadoutFixer = false;
            }

            if (Variables::SetPayingUser) {
                Socket::SendNotifCommand(OBF("Set Paying User (1/2)"), Commands::SetPayingUser1());
                Socket::SendNotifCommand(OBF("Set Paying User (2/2)"), Commands::SetPayingUser2());
                Variables::SetPayingUser = false;
            }

            if (Variables::SetKeys) {
                Socket::SendNotifCommand(OBF("Set Keys"), Commands::SetKeys(Variables::CurrencyAmount, Variables::LotteryNumber));
                Variables::SetKeys = false;
            }

            if (Variables::AddModule)
            {
                Socket::SendNotifCommand(OBF("Add Module"), Commands::AddModule(346009 + (Variables::ModuleType * 1000)));
                Variables::AddModule = false;
            }

            if (Variables::AddAllModules)
            {
                Socket::SendNotifCommand(OBF("Add All Modules"), Commands::AddAllModules());
                Variables::AddAllModules = false;
            }

            if (Variables::UpgradeModule)
            {
                Socket::SendNotifCommand(OBF("Upgrade Module"), Commands::UpgradeModule(304009 + (Variables::ModuleType * 1000)));
                Variables::UpgradeModule = false;
            }

            if (Variables::UpgradeAllModules)
            {
                Socket::SendNotifCommand(OBF("Upgrade All Modules"), Commands::UpgradeAllModules());
                Variables::UpgradeAllModules = false;
            }

            if (Variables::AddConsumable)
            {
                CommandsID ID = CommandsID::InventoryAddItemConsumable;
                int I = 0, T = 0;

                if (Constants::ConsumableMapping.count(Variables::ConsumableType))
                {
                    std::tie(I, T) = Constants::ConsumableMapping.at(Variables::ConsumableType);

                    if (Variables::ConsumableType < 3)
                    {
                        ID = CommandsID::GameEvents_UpdateFreeSpin;
                    }

                    Socket::SendNotifCommand(OBF("Add Consumable"), Commands::AddConsumable(ID, I, T));
                }

                Variables::AddConsumable = false;
            }

            if (Variables::SetRank)
            {
                Socket::SendNotifCommand(OBF("Set Rank"), Commands::SetRank(Variables::ClanRank, Variables::RankPoints));
                Variables::SetRank = false;
            } 
            if (Variables::SetGoblinsPositive)
            {
                Socket::SendNotifCommand(OBF("Add Goblin Time"), Commands::SetGoblinsPositive(Variables::GoblinDays));
              
                Variables::SetGoblinsPositive = false;
            }
            if (Variables::SetGoblinsNegative)
            {
                Socket::SendNotifCommand(OBF("Remove Goblin Time"), Commands::SetGoblinsNegative(Variables::GoblinDays));
                Variables::SetGoblinsNegative = false;
            }

            if (Variables::BugClanFort)
            {
                Socket::SendNotifCommand(OBF("Bugging Clan Fort"), Commands::BugClanFort());
                Variables::BugClanFort = false;
            }if (Variables::UnBugClanFort)
            {
                Socket::SendNotifCommand(OBF("Unbugging Clan Fort"), Commands::UnBugClanFort());
                Variables::UnBugClanFort = false;
            }

            // Stats
            if (Variables::CompleteTutorial)
            {
                Socket::SendNotifCommand(OBF("Complete Tutorial"), Commands::CompleteTutorial());
                Variables::CompleteTutorial = false;
            }

            if (Variables::AddBadge)
            {
                Socket::SendNotifCommand(OBF("Add Veteran Badge"), Commands::VeteranBadge());
                Variables::AddBadge = false;
            }

            if (Variables::RemoveBadge)
            {
                Socket::SendNotifCommand(OBF("Remove Veteran Badge"), Commands::VeteranBadge(true));
                Variables::RemoveBadge = false;
            }

            if (Variables::SetLevel)
            {
                Socket::SendNotifCommand(OBF("Set Level"), Commands::SetLevel(Variables::AdderLevel));
                Variables::SetLevel = false;
            }
            if (Variables::SkipTutorial)
            {
                Socket::SendNotifCommand(OBF("Skipped BR Tutorial"), Commands::SkipBrTutorial());
                Variables::SkipTutorial = false;
            }

            if (Variables::AddWin)
            {
                const std::array<int, 10> GameModeMapping = { 0, 2, 8, 5, 31, 20, 16, 1, 2, 3 };
                const int maxGameMode = static_cast<int>(GameModeMapping.size());

                int GameModeWin = (Variables::GameMode < maxGameMode) ? GameModeMapping[Variables::GameMode] : 0;

                ImGui::InsertNotification({ ImGuiToastType::Info, 3000, OBF("Adding Wins, please wait...") });
                json Command;
                if (Variables::GameMode < 7) {
                    Command = Commands::AddWin(GameModeWin);
                }
                else if (Variables::GameMode < 10) {
                    Command = Commands::AddWinRaid(GameModeWin);
                }
                else {
                    Command = Commands::AddWinTournament();
                }

                for (int i = 0; i < Variables::WinsAmount; ++i) {
                    Socket::SendCommand(Command);
                }

                Variables::AddWin = false;
            }

            if (Variables::MonthlyStats)
            {
                Socket::SendNotifCommand(OBF("Update Monthly Stats"), Commands::UpdateMonthlyStats(Variables::Kills, Variables::Deaths, Variables::HeadShots, Variables::KillStreak, Variables::WinStreak));
                Variables::MonthlyStats = false;
            }

            // Websocket - Clan
            if (Variables::SetClanFort)
            {
                Socket::SendNotifCommand(OBF("Set Clan Fort Level"), Commands::SetClanFortLevel(Variables::ClanLevel), false);
                Variables::SetClanFort = false;
            }

            if (Variables::SetClanTank)
            {
                Socket::SendNotifCommand(OBF("Set Clan Tank Level"), Commands::SetClanTankLevel(Variables::ClanLevel), false);
                Variables::SetClanTank = false;
            }

            if (Variables::EditClanLogo)
            {
                Socket::SendNotifCommand(OBF("Edit Clan Logo"), Commands::EditClan(Lists::ClanLogos[Variables::LogoType]), false);
                Variables::EditClanLogo = false;
            }

            if (Variables::EditClanName)
            {
                Socket::SendNotifCommand(OBF("Edit Clan Name"), Commands::EditClanName(Variables::ClanNameColor), false);
                Variables::EditClanName = false;
            }

            if (Variables::AddValor)
            {
                ImGui::InsertNotification({ ImGuiToastType::Info, 3000, OBF("Trying to Add Valor...") });
                for (short i = 0; i < Variables::ValorRepeats; ++i)
                {
                    Socket::SendCommand(Commands::AddClanValor());
                }

                Variables::AddValor = false;
            }

            if (Variables::CompleteClanTasks)
            {
                Socket::SendNotifCommand(OBF("Complete Clan Tasks"), Commands::CompleteClanTasks());
                Variables::CompleteClanTasks = false;
            }

            // Slots Fetchers
            if (Variables::CheckSlots[0])
            {
                CheckMySlots(OBF(56), Variables::CompleteTutorial);
                Variables::CheckSlots[0] = false;
            }

            if (Variables::CheckSlots[1])
            {
                CheckMySlots(OBF(37), Variables::UpgradeModule);
                Variables::CheckSlots[1] = false;
            }

            if (Variables::GetPlayerProgress)
            {
                std::vector<int> allSlots;
                for (int i = 1; i <= 100; ++i) allSlots.push_back(i);
                CheckPlayerSlots(Variables::GrabbedPlayerID, allSlots);
                ImGui::InsertNotification({ ImGuiToastType::Info, 3000, OBF("Grabbing Progress") });
                Variables::GetPlayerProgress = false;
            }

            if (Variables::CheckSlots[2])
            {
                CheckMySlots(OBF(37), Variables::UpgradeAllModules);
                Variables::CheckSlots[2] = false;
            }

            // Custom
            if (Variables::SendCommand)
            {
                Socket::SendNotifCommand(OBF("Custom Command"), Commands::Custom());
                Variables::SendCommand = false;
            }

            //Pixel Pass Shit
            if (Variables::CustomBattlepassSender)
            {
                ImGui::InsertNotification({ ImGuiToastType::Info, 3000, OBF("Step 1/4: Wiping old pass slot...") });
                Socket::SendCommand(Commands::WipePassSlot());
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
                ImGui::InsertNotification({ ImGuiToastType::Info, 3000, OBF("Step 2/4: Sending custom pass data...") });
                Socket::SendCommand(Commands::NovaPixelPass(Variables::PassDayCount, Variables::MaxTierNumber));
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
                ImGui::InsertNotification({ ImGuiToastType::Info, 3000, OBF("Step 3/4: Setting pass experience...") });
                Socket::SendCommand(Commands::SetPassExperience());
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
                ImGui::InsertNotification({ ImGuiToastType::Info, 3000, OBF("Step 4/4: Finalizing pass...") });
                Socket::SendCommand(Commands::ActivatePass());
                ImGui::InsertNotification({ ImGuiToastType::Success, 5000, OBF("Custom Battlepass Added!") });
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                Socket::SendCommand(Commands::Reload());
                Variables::CustomBattlepassSender = false;
            }

            if (Variables::GiftingPassSender)
            {
                ImGui::InsertNotification({ ImGuiToastType::Info, 3000, OBF("Step 1/4: Wiping old pass slot...") });
                Socket::SendCommand(Commands::WipePassSlot());
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
                ImGui::InsertNotification({ ImGuiToastType::Info, 3000, OBF("Step 2/4: Sending gift pass data...") });
                Socket::SendCommand(Commands::CustomGiftingPass(Variables::currentGiftItems));
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
                ImGui::InsertNotification({ ImGuiToastType::Success, 5000, OBF("Gift Pass Sent!") });
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                Socket::SendCommand(Commands::Reload());
                Variables::GiftingPassSender = false;
            }

            //Sandbox
            if (Variables::SetSandboxHeart) {
                Socket::SendNotifCommand(OBF("Set Heart Count"), Commands::SetSandboxHeart(Variables::SandboxHeartCount));
                Variables::SetSandboxHeart = false;
            }
            if (Variables::SetSandboxWings) {
                Socket::SendNotifCommand(OBF("Set Wings Count"), Commands::SetSandboxWings(Variables::SandboxWingsCount));
                Variables::SetSandboxWings = false;
            }
            if (Variables::SetSandboxDisco) {
                Socket::SendNotifCommand(OBF("Set Disco Count"), Commands::SetSandboxDisco(Variables::SandboxDiscoCount));
                Variables::SetSandboxDisco = false;
            }
            if (Variables::SetSandboxHead) {
                Socket::SendNotifCommand(OBF("Set Head Count"), Commands::SetSandboxHead(Variables::SandboxHeadCount));
                Variables::SetSandboxHead = false;
            }
            if (Variables::SetSandboxBear) {
                Socket::SendNotifCommand(OBF("Set Bear Count"), Commands::SetSandboxBear(Variables::SandboxBearCount));
                Variables::SetSandboxBear = false;
            }

            
        }
        return PixelTime_o(Instance);
    }

    // Initializate
    void Init()
    {
        HookMethod("PixelTime", "Update", PixelTime, (LPVOID*)&PixelTime_o);
    }
}
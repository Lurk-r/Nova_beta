#pragma once
#include <Json.hpp>

namespace Variables
{
	// Json
	inline bool CheckSlots[3] = { false };
	inline nlohmann::ordered_json TutorialSlot;
	inline nlohmann::ordered_json ModuleSlot;

	// Clans
	inline char ClanNameColor[100] = "";
	inline int ClanRank = 11;
	inline int RankPoints = 120000;
	inline int ClanWarID = 0;
	inline int ClanLevel = 2;
	inline int ModuleType = 0;
	inline int ModuleAmount = 0;
	inline int ModuleLevel = 9;
	inline int ValorAmount = 50;
	inline int ValorRepeats = 50;
	inline int LogoType = 0;
	inline int GoblinDays = 0;

	inline bool SetRank = false;
	inline bool SetClanFort = false;
	inline bool SetClanTank = false;
	inline bool EditClanLogo = false;
	inline bool EditClanName = false;
	inline bool AddModule = false;
	inline bool AddAllModules = false;
	inline bool UpgradeModule = false;
	inline bool UpgradeAllModules = false;
	inline bool SetGoblinsPositive = false;
	inline bool SetGoblinsNegative = false;
	inline bool BugClanFort = false;
	inline bool UnBugClanFort = false;
	inline bool AddValor = false;
	inline bool CompleteClanTasks = false;

	// Websocket
	inline char EventInput[100] = "update_progress";
	inline char CommandInput[1000000] = "{}";

	inline bool AutoReload = true;
	inline bool ReloadSocket = false;
	inline bool LogWebsocket = false;
	inline bool SendCommand = false;

	// Info Pointers
	inline std::string GameVersion = "";
	inline std::string PlayerID = "";
	inline std::string PlayerUsername = "";
	inline std::string ClanID = "";

	// Account
	inline int PlayerLevel = 1;
	inline int AdderLevel = 65;
	inline int CurrencyType = 0;
	inline int CurrencyAmount = 0;
	inline int ConsumableType = 0;
	inline int ConsumableAmount = 0;

	inline bool SetLevel = false;
	inline bool CompleteTutorial = false;
	inline bool AddBadge = false;
	inline bool RemoveBadge = false;
	inline bool AddCurrency = false;
	inline bool SpendCurrency = false;
	inline bool AddConsumable = false;

	// Stats
	inline int GameMode = 0;
	inline int WinsAmount = 0;
	inline int Kills = 0;
	inline int Deaths = 0;
	inline int HeadShots = 0;
	inline int KillStreak = 0;
	inline int WinStreak = 0;
	inline int MonthlyAmount = 0;

	inline bool AddWin = false;
	inline bool MonthlyStats = false;
	inline bool MonthlyWins = false;
	inline bool MonthlyGames = false;

	// Menu 
	inline float DimAmmount = 0.8f;
	inline bool Snowflakes = true;
	inline bool Background = true;
}

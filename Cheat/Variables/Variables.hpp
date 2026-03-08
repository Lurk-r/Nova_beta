#pragma once
#include <Json.hpp>
namespace Variables
{
	namespace Websocket
	{
#pragma region MAIN
		extern bool m_bReloadSocket, m_bAddCurrency, m_bSpendCurrency, m_bAddConsumable;
		extern bool m_bDebugMode;
		extern int m_iCurrencyType, m_iCurrencyAmount, m_iConsumableType, m_iConsumableAmount;
		extern bool m_bSendCustomCommand;
		extern char m_cCustomEventName[];
		extern char m_cCustomJson[];
#pragma endregion
#pragma region ACCOUNT
		extern bool m_bCloneName;
		extern bool m_bCloneCape;
		extern bool m_bCloneSkins;
#pragma endregion
#pragma region PROFILE
		extern bool m_bAddArmor, m_bAddAllArmor;
		extern bool m_bAddWear, m_bAddAllWear;
		extern bool m_bAddAvatar, m_bAddAllAvatars;
		extern bool m_bAddGadget, m_bAddAllGadgets, m_bRemoveGadget;
		extern bool m_bAddVehicle, m_bAddAllVehicles, m_bRemoveVehicle;
		extern bool m_bAddTrail, m_bAddAllTrails;
		extern bool m_bAddPet, m_bAddAllPets;
		extern bool m_bAddAllModules;
		extern bool m_bAddItemSingle, m_bAddAllWeaponSkins, m_bAddAllGraffiti;
		extern bool m_bAddAllFreeUpgrades;
		extern bool m_bSendFriendRequest, m_bSendClanMessage;
		extern bool m_bCompleteTutorial;
		extern bool m_bCompleteTutorialPending;
		extern bool m_bSetLevel;
		extern int m_iItemIndex;
		extern int m_iSetLevel;
		extern char m_cFriendID[256];
		extern char m_cClanMessage[512];
#pragma endregion
#pragma region WEAPONS
		extern bool m_bAddWeapon, m_bRemoveWeapon;
		extern bool m_bAddAllPrimary, m_bAddAllBackup, m_bAddAllMelee, m_bAddAllSpecial, m_bAddAllSniper, m_bAddAllHeavy;
		extern bool m_bRemoveAllPrimary, m_bRemoveAllBackup, m_bRemoveAllMelee, m_bRemoveAllSpecial, m_bRemoveAllSniper, m_bRemoveAllHeavy;
		extern bool m_bAddAllWeapons, m_bRemoveAllWeapons;
		extern bool m_bAddAllCraftable, m_bAddAllWeaponParts;
		extern int m_iWeaponSearchType;
		extern char m_cWeaponSearchBuffer[256];
		extern int m_iWeaponLevel;
		extern int m_iWeaponRarity;
		extern bool m_bUseDefaultRarity;
		extern int m_iWeaponPartAmount;
#pragma endregion
#pragma region CLAN
		extern bool m_bAddItem;
		extern int m_iSearchIndex;
#pragma endregion
#pragma region MISC
#pragma endregion
	}
	namespace Gameplay
	{
#pragma region SPAWNER
#pragma endregion
#pragma region MODS
		extern bool m_bRailgun;
		extern bool m_bBazooka;
		extern bool m_bShotgun;
		extern bool m_bDash;
		extern bool m_bHarpoon;
		extern bool m_bExplodingBullets;
		extern bool m_bQuickScope;
		extern bool m_bBlindness;
		extern bool m_bInfiniteAmmo;
		extern bool m_bFastFireRate;

#pragma endregion
	}
	namespace Miscellaneous
	{
#pragma region GIFTING
#pragma endregion
#pragma region TEST
		extern bool DumpOfferItemTypes;
		extern bool DumpOfferItemTypesInitial;
		extern bool m_bLogSocket;
		extern bool m_bDumpGame;
#pragma endregion
#pragma region EXTRAS

		extern int m_iPlayerLevel;
		extern bool m_bCheckAllMySlots;
		extern bool m_bCheckAllOtherPlayerSlots;
		extern bool m_bCheckMyModuleSlot;
		extern bool m_bAddXP;
		extern bool m_bForceSync;
		extern bool m_bSkinTest;
		extern bool m_bChatFullScreen;
		extern bool m_bChatSpam;
		extern bool m_bDumpEventNames;
		extern bool m_bOutputToken;
		extern bool m_bOutputAuthInfo;
		extern bool m_bLoginWithToken;
		extern bool m_bLoginWithIDSecret;
		extern bool m_bMigrateAccount;
		extern std::string m_sLoginToken;
		extern std::string m_sLoginID;
		extern std::string m_sLoginSecret;
		extern std::string m_sPlayerUsername;
		extern std::string m_sGameVersion;
		extern std::string m_sPlayerID;
		extern std::string m_sPlayerID2;
		extern std::string m_sOtherPlayerID;
		extern std::string m_sSkinBase64;
#pragma endregion
	}
	namespace Settings
	{
#pragma region ACCOUNT_INFO
#pragma endregion
	}
}
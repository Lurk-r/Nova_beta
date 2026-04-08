#include "Variables.hpp"

namespace Variables
{
	namespace Websocket
	{
#pragma region MAIN
		bool m_bReloadSocket, m_bAddCurrency, m_bSpendCurrency, m_bAddConsumable = false;
		bool m_bDebugMode = true;
		int m_iCurrencyType, m_iCurrencyAmount, m_iConsumableType, m_iConsumableAmount = 0;

		bool m_bSendCustomCommand = false;

		char m_cCustomEventName[67] = "update_progress";
		char m_cCustomJson[999999] = "";
#pragma endregion

#pragma region ACCOUNT
		bool m_bCloneName = false;
		bool m_bCloneCape = false;
		bool m_bCloneSkins = false;
#pragma endregion

#pragma region PROFILE
		bool m_bAddArmor, m_bAddAllArmor = false;
		bool m_bAddWear, m_bAddAllWear = false;
		bool m_bAddAvatar, m_bAddAllAvatars = false;
		bool m_bAddGadget, m_bAddAllGadgets, m_bRemoveGadget = false;
		bool m_bAddVehicle, m_bAddAllVehicles, m_bRemoveVehicle = false;
		bool m_bAddTrail, m_bAddAllTrails = false;
		bool m_bAddPet, m_bAddAllPets = false;
		bool m_bAddAllModules = false;
		bool m_bAddItemSingle, m_bAddAllWeaponSkins, m_bAddAllGraffiti = false;
		bool m_bAddAllFreeUpgrades = false;
		bool m_bSendFriendRequest, m_bSendClanMessage = false;
		bool m_bCompleteTutorial = false;
		bool m_bCompleteTutorialPending = false;
		bool m_bSetLevel = false;
		int m_iItemIndex = 0;
		int m_iSetLevel = 65;
		char m_cFriendID[256] = "";
		char m_cClanMessage[512] = "";
#pragma endregion

#pragma region WEAPONS
		bool m_bAddWeapon = false;
		bool m_bRemoveWeapon = false;
		bool m_bAddAllPrimary = false;
		bool m_bAddAllBackup = false;
		bool m_bAddAllMelee = false;
		bool m_bAddAllSpecial = false;
		bool m_bAddAllSniper = false;
		bool m_bAddAllHeavy = false;
		bool m_bRemoveAllPrimary = false;
		bool m_bRemoveAllBackup = false;
		bool m_bRemoveAllMelee = false;
		bool m_bRemoveAllSpecial = false;
		bool m_bRemoveAllSniper = false;
		bool m_bRemoveAllHeavy = false;
		bool m_bAddAllWeapons = false;
		bool m_bRemoveAllWeapons = false;
		bool m_bAddAllCraftable = false;
		bool m_bAddAllWeaponParts = false;
		bool m_bUseDefaultRarity = true;

		int m_iWeaponSearchType = 0;
		int m_iWeaponLevel = 65;
		int m_iWeaponRarity = 5;
		int m_iWeaponPartAmount = 250;

		char m_cWeaponSearchBuffer[256] = "";
#pragma endregion

#pragma region CLAN
		bool m_bAddItem = false;
		int m_iSearchIndex = 0;
#pragma endregion

#pragma region MISC

#pragma endregion
	}

	namespace Gameplay
	{
#pragma region SPAWNER

#pragma endregion

#pragma region MODS
		bool m_bRailgun = false;
		bool m_bBazooka = false;
		bool m_bShotgun = false;
		bool m_bDash = false;
		bool m_bHarpoon = false;
		bool m_bExplodingBullets = false;
		bool m_bQuickScope = false;
		bool m_bBlindness = false;
		bool m_bFastFireRate = false;
		bool m_bInfiniteAmmo = false;

#pragma endregion
	}

	namespace Miscellaneous
	{
#pragma region GIFTING

#pragma endregion

#pragma region TEST
		bool DumpOfferItemTypes = false;
		bool DumpOfferItemTypesInitial = false;
		bool m_bLogSocket = true;
		bool m_bDumpGame = false;
#pragma endregion

#pragma region EXTRAS
		int m_iPlayerLevel = 69;

		bool m_bCheckAllMySlots = false;
		bool m_bCheckAllOtherPlayerSlots = false;
		bool m_bCheckMyModuleSlot = false;
		bool m_bAddXP = false;
		bool m_bForceSync = false;
		bool m_bSkinTest = false;
		bool m_bDumpEventNames = false;
		bool m_bChatFullScreen = false;
		bool m_bChatSpam = false;
		bool m_bOutputToken = false;
		bool m_bOutputAuthInfo = false;
		bool m_bLoginWithToken = false;
		bool m_bLoginWithIDSecret = false;
		bool m_bMigrateAccount = false;

		std::string m_sLoginToken = "43455d18471423d7ad17270e3680539240d8a79212e2e6982719b0d665f9b26a773dbf52c1682c73d2a0dac3c6187d2e4b6c5baa45ac9b6e51e1bae613062e64a017cb4665944275e87e3756c8bda1b7612b4f1356c71d431535dd98035652694162772b5761730d";
		std::string m_sLoginID = "344983740";
		std::string m_sLoginSecret = "2bc2b652-f25c-4163-bf73-f0716801b2ee";
		std::string m_sPlayerUsername = "Nova on Top";
		std::string m_sGameVersion = "26.67";
		std::string m_sPlayerID = "69696969";
		std::string m_sPlayerID2 = "106087532";
		std::string m_sOtherPlayerID = "69696969";
		std::string m_sSkinBase64 = "iVBORw0KGgoAAAANSUhEUgAAAgAAAADOCAIAAADHZy1AAAAHQUlEQVR4nO3csREAIQwDweu/aVEGgTchZjZ4C4y/VctKgAABAjv2Pfy/AysBAgQITA1QDwgQIEAgJwCJgAABAgTmFkgiIECAAIH0ACQCAgQIEJg+sERAgAABAnkFJBEQIECAwLwElQgIECBAIHMAEgEBAgQIzCyYRECAAAECmQSWCAgQILDz/yNQDwkQIECggyeD/zuwEiBAgMDUAPWAAAECBHICkAgIECBAYG6BJAICBAgQSA9AIiBAgACB6QNLBAQIECCQV0ASAQECBAjMS1CJgAABAgQyByARECBAgMDMgkkEBAgQIJBJYImAAAECO/8/AvWQAAECBDp4Mvi/AysBAgQITA1QDwgQIEAgJwCJgAABAgTmFkgiIECAAIH0ACQCAgQIEJg+sERAgAABAnkFJBEQIECAwLwElQgIECBAIHMAEgEBAgQIzCyYRECAAAECmQSWCAgQILDz/yNQDwkQIECggyeD/zuwEiBAgMDUAPWAAAECBHICkAgIECBAYG6BJAICBAgQSA9AIiBAgACB6QNLBAQIECCQV0ASAQECBAjMS1CJgAABAgQyByARECBAgMDMgkkEBAgQIJBJYImAAAECO/8/AvWQAAECBDp4Mvi/AysBAgQITA1QDwgQIEAgJwCJgAABAgTmFkgiIECAAIH0ACQCAgQIEJg+sERAgAABAnkFJBEQIECAwLwElQgIECBAIHMAEgEBAgQIzCyYRECAAAECmQSWCAgQILDz/yNQDwkQIECggyeD/zuwEiBAgMDUAPWAAAECBHICkAgIECBAYG6BJAICBAgQSA9AIiBAgACB6QNLBAQIECCQV0ASAQECBAjMS1CJgAABAgQyByARECBAgMDMgkkEBAgQIJBJYImAAAECO/8/AvWQAAECBDp4Mvi/AysBAgQITA1QDwgQIEAgJwCJgAABAgTmFkgiIECAAIH0ACQCAgQIEJg+sERAgAABAnkFJBEQIECAwLwElQgIECBAIHMAEgEBAgQIzCyYRECAAAECmQSWCAgQILDz/yNQDwkQIECggyeD/zuwEiBAgMDUAPWAAAECBHICkAgIECBAYG6BJAICBAgQSA9AIiBAgACB6QNLBAQIECCQV0ASAQECBAjMS1CJgAABAgQyByARECBAgMDMgkkEBAgQIJBJYImAAAECO/8/AvWQAAECBDp4Mvi/AysBAgQITA1QDwgQIEAgJwCJgAABAgTmFkgiIECAAIH0ACQCAgQIEJg+sERAgAABAnkFJBEQIECAwLwElQgIECBAIHMAEgEBAgQIzCyYRECAAAECmQSWCAgQILDz/yNQDwkQIECggyeD/zuwEiBAgMDUAPWAAAECBHICkAgIECBAYG6BJAICBAgQSA9AIiBAgACB6QNLBAQIECCQV0ASAQECBAjMS1CJgAABAgQyByARECBAgMDMgkkEBAgQIJBJYImAAAECO/8/AvWQAAECBDp4Mvi/AysBAgQITA1QDwgQIEAgJwCJgAABAgTmFkgiIECAAIH0ACQCAgQIEJg+sERAgAABAnkFJBEQIECAwLwElQgIECBAIHMAEgEBAgQIzCyYRECAAAECmQSWCAgQILDz/yNQDwkQIECggyeD/zuwEiBAgMDUAPWAAAECBHICkAgIECBAwC2QRECAAAEC6QFIBAQIECCQPrBEQIAAAQJ5BSQRECBAgEBegkoEBAgQIJA5AImAAAECBDILJhEQIECAQCaBJQICBAhkzkA9JECAAIEOTiD/34GVAAECBKYGqAcECBAgkBOARECAAAECcwskERAgQIBAegASAQECBAhMH1giIECAAIG8ApIICBAgQGBegkoEBAgQIJA5AImAAAECBGYWTCIgQIAAgUwCSwQECBDY+f8RqIcECBAg0MGTwf8dWAkQIEBgaoB6QIAAAQI5AUgEBAgQIDC3QBIBAQIECKQHIBEQIECAwPSBJQICBAgQyCsgiYAAAQIE5iWoRECAAAECmQOQCAgQIEBgZsEkAgIECBDIJLBEQIAAgZ3/H4F6SIAAAQIdPBn834GVAAECBKYGqAcECBAgkBOARECAAAECcwskERAgQIBAegASAQECBAhMH1giIECAAIG8ApIICBAgQGBegkoEBAgQIJA5AImAAAECBGYWTCIgQIAAgUwCSwQECBDY+f8RqIcECBAg0MGTwf8dWAkQIEBgaoB6QIAAAQI5AUgEBAgQIDC3QBIBAQIECKQHIBEQIECAwPSBJQICBAgQyCsgiYAAAQIE5iWoRECAAAECmQOQCAgQIEBgZsEkAgIECBDIJLBEQIAAgZ3/H4F6SIAAAQIdPBn834GVAAECBKYGqAcECBAgkBOARECAAAECcwskERAgQIBAegASAQECBAhMH1giIECAAIG8ApIICBAgQGBegkoEBAgQIJA5AImAAAECBGYWTCIgQIAAgUwCSwQECBDY+f8RqIcECBAg0MGTwf8dWAkQIEBgaoB6QIAAAQI5AUgEBAgQIDC3QBIBAQIECKQHIBEQIECAwPSBJQICBAgQyCsgiYAAAQIE5iWoRECAAAECmQOQCAgQIEBgZsEkAgIECBDIJLBEQIAAAQLqIQECBAh08GTwfwdWAgQIEJgaoB4QIECAQE4AEgEBAgQIzC2QRECAAAEC6QFIBAQIECAwfWCJgAABAgTyCkgiIECAAIF5CSoRECBAgEDmACQCAgQIEJhZMImAAAECBDIJLBEQIEBg5/9HoB4SIECAQAdPBv93YCVAgACB/fgePgR8fAdkfDgeAAAAAElFTkSuQmCC";
#pragma endregion
	}

	namespace Settings
	{
#pragma region ACCOUNT_INFO

#pragma endregion
	}
}
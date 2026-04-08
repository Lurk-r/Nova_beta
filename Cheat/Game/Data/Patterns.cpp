#include "Patterns.hpp"

namespace Patterns
{
#pragma region CLASS
	IL2CPP::FieldPattern ProfileClass = { "String", "Int32", "String", "Boolean", "Boolean", "Nullable`1", "Boolean", "String", "String", "Action" };
	IL2CPP::FieldPattern ContentRegister = { nullptr, nullptr, nullptr, "Dictionary`2", "Dictionary`2" };
	IL2CPP::FieldPattern ProgressSyncController = { "Action`1", "Action`2", "Int32" };
	IL2CPP::FieldPattern ItemRecordMapped = { "String", "Int32", "Int32", "Int32", "Int32", "String", "Boolean", "Int32", "Single[]", "List`1", "List`1", "Int32", "Single", "Int32", "List`1", "Boolean", "Boolean", "Boolean", "Boolean", "Boolean", "Boolean", "String", "Int32", "Int32", "Int32", "Int32", "Int32", "Single", "Boolean", "Boolean", "Boolean", "Boolean", "Boolean", "KeyValuePair`2", "KeyValuePair`2", "Int32", "Single", "Single", "Boolean", "String", "String", "String", "Int32", "String", "String", "Int32", "Boolean", "KeyValuePair`2" };
	IL2CPP::FieldPattern MigrationPG = { "Boolean", "String", "Int32", "Int32", "String", "List`1", "String" };
	IL2CPP::FieldPattern ProgressGrabberPG = { "String", "Action", "List`1", "Dictionary`2", "HashSet`1" };
	IL2CPP::FieldPattern serverSlots = { "Int32", "Nullable`1", "HashSet`1", nullptr };
	IL2CPP::FieldPattern FreePurchaseClass = { nullptr, nullptr, "Nullable`1", nullptr, nullptr, nullptr, "Int32" };
	IL2CPP::FieldPattern NetworkingPeerClass = { "String", nullptr, "String", "AuthModeOption", "EncryptionMode", "Boolean", "String", "String", "Dictionary`2", "String", "String", "ServerConnection", "ClientState", "Boolean", "Boolean", nullptr, "List`1", "Dictionary`2", nullptr, "String", "Boolean", nullptr, nullptr, "Int32", "Int32", "Int32", "JoinType", nullptr, "Boolean", "String[]", "Int32", "Boolean", "List`1", "CloudRegionCode", "Dictionary`2", nullptr, nullptr, "Boolean", "HashSet`1", "HashSet`1", "Dictionary`2", nullptr, nullptr, "Dictionary`2", "Dictionary`2", "Int16", "Boolean", "String", "String", "Boolean", nullptr, "Dictionary`2", "Dictionary`2", "Dictionary`2", "String", "String", "String", "ServerConnection", "AsyncOperation", "Boolean", "Dictionary`2", "Int32", nullptr, "Int32", "Int32", "Int32", "Int32" };
	IL2CPP::FieldPattern MatchKickingClass = { "Action`1", "Action", "Nullable`1", "Nullable`1" };
	IL2CPP::FieldPattern MatchKickingClass2 = { "Action`1", nullptr, "Task`1", "CancellationTokenSource", "Int32", nullptr };
	IL2CPP::FieldPattern PlayerPrefManager = { "String", "HashSet`1", "Dictionary`2", "Dictionary`2", "Dictionary`2", "Boolean", "HashSet`1", "IDictionary`2", "String[]" };
	IL2CPP::FieldPattern EventName = { "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "String", "List`1", "HashSet`1" };
	IL2CPP::FieldPattern LoadingProgress = { nullptr, "Texture2D", "GUIStyle", "Texture2D" };
	IL2CPP::FieldPattern AccountDataClassV2 = { "Int32", "Int32", "Int32", "Single", "Single", "Single", "Single", nullptr, "Coroutine", nullptr, "ValueTuple`2", "String", "String", nullptr, "Boolean", "Boolean", "String", "String", "Int32", "String", "HAuthTicket", "AuthorizationStates", "Coroutine", "Coroutine", "CancellationTokenSource", "Task`1", "String", "String", "Single", "String", "Boolean", "Boolean", "String", nullptr, nullptr, "Boolean", "Action", "Action", "Action", "Action", "Action", "Action`1", "Action", "Action" };
	IL2CPP::FieldPattern RoyaleDictionary = { "String", "String", "String", "String", "String", "String", "String", "String", "String", "RoyaleScriptableObject_GENERATED", "Action`1", "Action`1", "Action`1", "Action`1", "Action`1", "Dictionary`2" };
	IL2CPP::FieldPattern PhotonNetwork = { "String", "String", "PhotonHandler", nullptr, "Int32", "Int32", "String", "ServerSettings", "Boolean", "PhotonLogLevel", "List`1", "Single", "Single", "Single", "Boolean", "Boolean", "Dictionary`2", "HashSet`1", "HashSet`1", "Type", "Boolean", "Boolean", nullptr, "Int32", "Boolean", "Boolean", "Int32", "Int32", "Boolean", nullptr, "Boolean", "Stopwatch", "Single", "Boolean", nullptr, "Int32", "Int32", "List`1" };

	IL2CPP::FieldPattern ItemDb = { "IEqualityComparer`1", "Int32", "String[]", "String[]", "Color[]", "List`1", "Dictionary`2", "Dictionary`2", "Dictionary`2", "HashSet`1", "Dictionary`2", nullptr, nullptr, nullptr, nullptr, "Dictionary`2", "WeakReference" };
#pragma endregion

#pragma region METHOD

#pragma endregion

#pragma region IMAGES
	static void AssemblyCSharpQueue()
	{
		// No namespace
		IL2CPP::ClassMapping::AddQueue("ProfileClass", "", &ProfileClass);
		IL2CPP::ClassMapping::AddQueue("PhotonNetwork", "", &PhotonNetwork);
		IL2CPP::ClassMapping::AddQueue("NetworkingPeerClass", "", &NetworkingPeerClass);
		IL2CPP::ClassMapping::AddQueue("ItemDb", "", &ItemDb);
		IL2CPP::ClassMapping::AddQueue("ItemRecordMapped", "", &ItemRecordMapped);
		IL2CPP::ClassMapping::AddQueue("serverSlots", "", &serverSlots);
		IL2CPP::ClassMapping::AddQueue("PlayerPrefManager", "", &PlayerPrefManager);
		IL2CPP::ClassMapping::AddQueue("AccountDataClassV2", "", &AccountDataClassV2);
		IL2CPP::ClassMapping::AddQueue("RoyaleDictionary", "", &RoyaleDictionary);

		// PGCompany.obfuscatedname
		IL2CPP::ClassMapping::AddQueue("ContentRegister", "PGCompany", &ContentRegister, 92);

		// PGCompany.MatchmakingRequest.obfuscatedname
		IL2CPP::ClassMapping::AddQueue("MatchKickingClass", "PGCompany.MatchmakingRequest", &MatchKickingClass);
		IL2CPP::ClassMapping::AddQueue("MMRequestClass", "PGCompany.MatchmakingRequest", &MatchKickingClass2);

		// BestHTTP.JSON
		IL2CPP::ClassMapping::AddQueue("Json", "BestHTTP.JSON");

		// Progress.obfuscatedname
		IL2CPP::ClassMapping::AddQueue("MigrationPG", "Progress", &MigrationPG);
		IL2CPP::ClassMapping::AddQueue("ProgressGrabberPG", "Progress", &ProgressGrabberPG);

		// Rilisoft.OfferItem
		IL2CPP::ClassMapping::AddQueue("OfferItem", "Rilisoft");

		// Rilisoft.WebSocket.obfuscatedname
		// IL2CPP::ClassMapping::AddQueue("EventName", "Rilisoft.WebSocket", &EventName);

		// ProgressSyncController.Progress
		IL2CPP::ClassMapping::AddQueue("ProgressSyncController", "Progress", &ProgressSyncController);

		// PGCompany.MainStore.obfuscatedname
		IL2CPP::ClassMapping::AddQueue("FreePurchaseClass", "PGCompany", &FreePurchaseClass);

		// Rilisoft.obfuscatedname
		IL2CPP::ClassMapping::AddQueue("LoadingProgress", "Rilisoft", &LoadingProgress);
	}
	static void NewtonSoftQueue()
	{
		// Newtonsoft.Json
		IL2CPP::ClassMapping::AddQueue("JsonConvert", "Newtonsoft.Json");
	}
	static void CorlibQueue()
	{
		// System.String
		IL2CPP::ClassMapping::AddQueue("String", "System");
	}
#pragma endregion

	void Init()
	{
		// Images
		IL2CPP::ClassMapping::AddImageToScan(IL2CPP::DefaultImage::AssemblyCSharp);
		IL2CPP::ClassMapping::AddImageToScan(IL2CPP::DefaultImage::NewtonSoft);
		IL2CPP::ClassMapping::AddImageToScan(IL2CPP::DefaultImage::Corlib);

		// Mapping
		AssemblyCSharpQueue();
		NewtonSoftQueue();
		CorlibQueue();

		IL2CPP::ClassMapping::StartMapping();
	}
}
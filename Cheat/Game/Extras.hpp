#pragma once
#include <Unity/IL2CPP.hpp>

namespace Extras
{
	namespace ExperienceController
	{
		extern void(*AddXP_o)(IL2CPP::Object*, int, int, IL2CPP::Object*, IL2CPP::Object*);
		extern int(*GetLevel_o)();
		void AddXP(IL2CPP::Object*, int, int, IL2CPP::Object*, IL2CPP::Object*);
		int GetLevel();
	}

	namespace ProgressSyncController
	{
		extern void(*DoSyncSafe_o)(IL2CPP::Object*);
		void DoSyncSafe_Detour(IL2CPP::Object*);
	}

	void HandleSkinTest();
	void HandleForceSync();
	void HandleAddXP();
	void SecureLevel();
	void HandleEventDump();

	void Init();
}
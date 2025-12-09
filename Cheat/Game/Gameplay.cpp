#include "Gameplay.hpp"
#include "../Utils/Il2CppHelper.hpp"

namespace Gameplay
{
	namespace Player_move_c
	{
		void(*Update_o)(IL2CPP::Object*) = nullptr;
		void Update(IL2CPP::Object* instance)
		{
			if (instance)
			{

			}

			Update_o(instance);
		}

		void Init()
		{
			HookMethod("Player_move_c", "Update", Update, (LPVOID*)&Update_o);
		}
	}

	namespace WeaponSounds
	{
		void(*Update_o)(IL2CPP::Object*) = nullptr;
		void Update(IL2CPP::Object* instance)
		{
			if (instance)
			{

			}

			Update_o(instance);
		}

		void Init()
		{
			HookMethod("WeaponSounds", "Update", Update, (LPVOID*)&Update_o);
		}
	}
}

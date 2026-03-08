// Gameplay.hpp
#pragma once
#include <Unity/IL2CPP.hpp>
namespace Gameplay
{
	namespace Player_move_c
	{
		extern void(*Update_o)(IL2CPP::Object*);
		void Update(IL2CPP::Object*);
		void Init();
	}
	namespace WeaponSounds
	{
		extern void (*Update_o)(IL2CPP::Object*);
		void Update(IL2CPP::Object*);
		void Init();
	}
	namespace System_String
	{
		extern IL2CPP::String* (*Concat_o)(IL2CPP::String*, IL2CPP::String*, IL2CPP::String*, IL2CPP::String*);
		IL2CPP::String* Concat(IL2CPP::String*, IL2CPP::String*, IL2CPP::String*, IL2CPP::String*);
		void Init();
	}
}
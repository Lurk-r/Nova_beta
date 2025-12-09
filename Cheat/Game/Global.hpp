#pragma once
#include <Unity/IL2CPP.hpp>

namespace Global
{
	namespace PixelTime
	{
		extern void(*Update_o)(IL2CPP::Object*);
		void Update(IL2CPP::Object*);

		void Init();
	}
}
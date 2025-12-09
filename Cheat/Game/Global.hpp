#pragma once
#include <Unity/IL2CPP.hpp>

namespace Global
{
	namespace PixelTime
	{
		extern void(*PixelTime_o)(IL2CPP::Object*);
		void PixelTime(IL2CPP::Object*);

		void Init();
	}
}
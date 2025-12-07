#pragma once
#include "../Main.hpp"

namespace IL2CPP
{
	namespace DefaultTypeClass
	{
		extern Class* Array;
		extern Class* Boolean;
		extern Class* Byte;
		extern Class* Char;
		extern Class* Decimal;
		extern Class* Double;
		extern Class* Float;
		extern Class* Int16;
		extern Class* Int32;
		extern Class* Int64;
		extern Class* Object;
		extern Class* Quaternion;
		extern Class* String;
		extern Class* Type;
		extern Class* Void;
		extern Class* Vector2;
		extern Class* Vector3;
	}

	namespace DefaultImage
	{
		extern const Image* Corlib;
		extern const Image* System;
		extern const Image* UnityCoreModule;
		extern const Image* UnityPhysicsModule;
		extern const Image* AssemblyCSharp;
	}

	namespace CommonCShrap
	{
		void INIT();
	}
}

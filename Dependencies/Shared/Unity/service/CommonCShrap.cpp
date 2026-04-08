#include "CommonCShrap.hpp"

namespace IL2CPP
{
	namespace DefaultTypeClass
	{
		Class* Array = nullptr;
		Class* Boolean = nullptr;
		Class* Byte = nullptr;
		Class* Char = nullptr;
		Class* Decimal = nullptr;
		Class* Double = nullptr;
		Class* Float = nullptr;
		Class* Int16 = nullptr;
		Class* Int32 = nullptr;
		Class* Int64 = nullptr;
		Class* Object = nullptr;
		Class* Quaternion = nullptr;
		Class* String = nullptr;
		Class* Type = nullptr;
		Class* Void = nullptr;
		Class* Vector2 = nullptr;
		Class* Vector3 = nullptr;
	}

	namespace DefaultImage
	{
		const Image* Corlib;
		const Image* System;
		const Image* UnityCoreModule;
		const Image* UnityPhysicsModule;
		const Image* AssemblyCSharp;
		const Image* NewtonSoft;
	}

	namespace CommonCShrap
	{
		constexpr auto INIT_CLASS = [](Class*& field, const Image* image, const char* className)
		{
			field = image->GetClass(className);
		};

		void INIT()
		{
			Domain* domain = CurrentDomain();

			DefaultImage::Corlib = (Image*)IMPORT::il2cpp_get_corlib();
			DefaultImage::System = domain->OpenAssembly("System.dll");
			DefaultImage::UnityCoreModule = domain->OpenAssembly("UnityEngine.CoreModule.dll");
			DefaultImage::UnityPhysicsModule = domain->OpenAssembly("UnityEngine.PhysicsModule.dll");
			DefaultImage::AssemblyCSharp = domain->OpenAssembly("Assembly-CSharp.dll");
			DefaultImage::NewtonSoft = domain->OpenAssembly("Newtonsoft.Json.dll");

			constexpr auto INIT_CORLIB = [](Class*& field, const char* className)
			{
				INIT_CLASS(field, DefaultImage::Corlib, className);
			};

			constexpr auto INIT_UNITY_ENGINE = [](Class*& field, const char* className)
			{
				INIT_CLASS(field, DefaultImage::UnityCoreModule, className);
			};

			INIT_CORLIB(DefaultTypeClass::Array, "System.Array");
			INIT_CORLIB(DefaultTypeClass::Boolean, "System.Boolean");
			INIT_CORLIB(DefaultTypeClass::Byte, "System.Byte");
			INIT_CORLIB(DefaultTypeClass::Char, "System.Char");
			INIT_CORLIB(DefaultTypeClass::Decimal, "System.Decimal");
			INIT_CORLIB(DefaultTypeClass::Double, "System.Double");
			INIT_CORLIB(DefaultTypeClass::Float, "System.Single");
			INIT_CORLIB(DefaultTypeClass::Int16, "System.Int16");
			INIT_CORLIB(DefaultTypeClass::Int32, "System.Int32");
			INIT_CORLIB(DefaultTypeClass::Int64, "System.Int64");
			INIT_CORLIB(DefaultTypeClass::Object, "System.Object");
			INIT_CORLIB(DefaultTypeClass::String, "System.String");
			INIT_CORLIB(DefaultTypeClass::Type, "System.Type");
			INIT_CORLIB(DefaultTypeClass::Void, "System.Void");

			INIT_UNITY_ENGINE(DefaultTypeClass::Quaternion, "UnityEngine.Quaternion");
			INIT_UNITY_ENGINE(DefaultTypeClass::Vector2, "UnityEngine.Vector2");
			INIT_UNITY_ENGINE(DefaultTypeClass::Vector3, "UnityEngine.Vector3");
		}
	}
}
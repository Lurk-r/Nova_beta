#include "Extras.hpp"
#include "../Utils/Il2CppHelper.hpp"
#include "../Variables/Variables.hpp"
#include <vector>
#include <string>
#include <chrono>
#include <iostream>
#include "../Backend/Backend.hpp"

namespace Extras
{
	namespace ExperienceController
	{
		void(*AddXP_o)(IL2CPP::Object*, int, int, IL2CPP::Object*, IL2CPP::Object*) = nullptr;
		int(*GetLevel_o)() = nullptr;

		void AddXP(IL2CPP::Object* instance, int a, int b, IL2CPP::Object* c, IL2CPP::Object* d) {
			return AddXP_o(instance, a, b, c, d);
		}

		int GetLevel() {
			return GetLevel_o();
		}
	}
	namespace EventNameController
	{
		bool(*ProcessEvent_o)(IL2CPP::String*) = nullptr;

		bool ProcessEvent_Detour(IL2CPP::String* eventName) {
			if (eventName) {
				std::string name = eventName->ToString();
				std::cout << "Event Dumped: " << name << std::endl;
			}
			return ProcessEvent_o(eventName);
		}
	}
	namespace ProgressSyncController
	{
		void(*DoSyncSafe_o)(IL2CPP::Object*) = nullptr;

		void DoSyncSafe_Detour(IL2CPP::Object* arg) {
			DoSyncSafe_o(arg);
		}
	}

	void HandleSkinTest()
	{
		if (Variables::Miscellaneous::m_sSkinBase64.empty()) {
			return;
		}

		auto klass = Cache::GetClass("SkinsController");
		if (!klass) {
			return;
		}

		auto rawKlass = (IL2CPP::IMPORT::Il2CppClass*)klass;
		IL2CPP::IMPORT::il2cpp_runtime_class_init(rawKlass);

		auto mTextureFromString = (rawKlass->method_count > 0x1f) ? rawKlass->methods[0x1f] : nullptr;
		auto mSetCurrentSkin = (rawKlass->method_count > 0x26) ? rawKlass->methods[0x26] : nullptr;

		if (!mTextureFromString || !mSetCurrentSkin) {
			return;
		}

		auto il2cppBase64 = IL2CPP::IMPORT::il2cpp_string_new(Variables::Miscellaneous::m_sSkinBase64.c_str());
		int width = 512;
		int height = 206;
		void* argsTex[] = { il2cppBase64, &width, &height };

		auto textureObj = (IL2CPP::Object*)IL2CPP::IMPORT::il2cpp_runtime_invoke(mTextureFromString, nullptr, argsTex, nullptr);
		if (!textureObj) {
			return;
		}

		if (rawKlass->static_fields) {
			uintptr_t staticBase = (uintptr_t)rawKlass->static_fields;

			IL2CPP::String** currentNamePtr = (IL2CPP::String**)(staticBase + 0x38);
			IL2CPP::String* currentSkinName = *currentNamePtr;
			if (!currentSkinName) {
				return;
			}

			std::string skinId = currentSkinName->ToString();
			IL2CPP::Object* skinsDict = *(IL2CPP::Object**)(staticBase + 0x20);
			if (!skinsDict) {
				return;
			}

			auto dictClass = IL2CPP::IMPORT::il2cpp_object_get_class((IL2CPP::IMPORT::Il2CppObject*)skinsDict);
			auto setItem = IL2CPP::IMPORT::il2cpp_class_get_method_from_name(dictClass, "set_Item", 2);
			if (!setItem) {
				return;
			}

			void* args[] = { currentSkinName, textureObj };
			IL2CPP::IMPORT::il2cpp_runtime_invoke(setItem, (IL2CPP::IMPORT::Il2CppObject*)skinsDict, args, nullptr);
			void* argsSet[] = { currentSkinName };
			IL2CPP::IMPORT::il2cpp_runtime_invoke(mSetCurrentSkin, nullptr, argsSet, nullptr);
		}
	}

	void HandleForceSync()
	{
		auto helperClass = IL2CPP::ClassMapping::GetClass("serverSlots");
		
		auto field = IL2CPP::IMPORT::il2cpp_class_get_field_from_name((IL2CPP::IMPORT::Il2CppClass*)helperClass, "serverSlots");
		
		auto hashSetClass = IL2CPP::IMPORT::il2cpp_class_from_type(IL2CPP::IMPORT::il2cpp_field_get_type(field));

		auto hashSet = (IL2CPP::Object*)IL2CPP::IMPORT::il2cpp_object_new(hashSetClass);
		IL2CPP::IMPORT::il2cpp_runtime_object_init((IL2CPP::IMPORT::Il2CppObject*)hashSet);

		auto addMethod = IL2CPP::IMPORT::il2cpp_class_get_method_from_name(hashSetClass, "Add", 1);

		if (!ProgressSyncController::DoSyncSafe_o && !helperClass && !field && !hashSetClass && !addMethod) {
			return;
		}
		for (int i = 1; i <= 100; i++) {
			void* args[] = { IL2CPP::IMPORT::il2cpp_string_new(std::to_string(i).c_str()) };
			IL2CPP::IMPORT::il2cpp_runtime_invoke(addMethod, (IL2CPP::IMPORT::Il2CppObject*)hashSet, args, nullptr);
		}
		ProgressSyncController::DoSyncSafe_o(hashSet);
	}

	void HandleAddXP()
	{
		auto klass = Cache::GetClass("ExperienceController");
		if (!klass && !ExperienceController::AddXP_o) {
			return;
		}

		IL2CPP::Object* xpinstance = (IL2CPP::Object*)IL2CPP::IMPORT::il2cpp_object_new((IL2CPP::IMPORT::Il2CppClass*)klass);
		ExperienceController::AddXP_o(xpinstance, 1, 6549877, nullptr, nullptr);
	}

	void SecureLevel()
	{
		HandleAddXP();
	}

	void HandleEventDump()
	{
		auto domain = IL2CPP::IMPORT::il2cpp_domain_get();
		if (domain) {
			IL2CPP::IMPORT::il2cpp_thread_attach(domain);
		}

		auto klass = IL2CPP::ClassMapping::GetClass("EventName");
		if (!klass) {
			return;
		}

		auto rawKlass = (IL2CPP::IMPORT::Il2CppClass*)klass;

		IL2CPP::IMPORT::il2cpp_runtime_class_init(rawKlass);

		if (!rawKlass->static_fields) {
			return;
		}

		std::string filename = "NovaEventNames-" + Variables::Miscellaneous::m_sGameVersion + ".cs";
		std::ofstream out(filename);
		if (!out.is_open()) return;

		std::cout << "Event names dumping to " << filename << std::endl;

		out << "public static class EventNames {\n";

		void* iter = nullptr;
		int count = 0;
		uintptr_t staticBase = (uintptr_t)rawKlass->static_fields;

		while (auto field = IL2CPP::IMPORT::il2cpp_class_get_fields(rawKlass, &iter)) {
			uint32_t flags = IL2CPP::IMPORT::il2cpp_field_get_flags(field);

			if (!(flags & 0x0010)) continue;

			auto type = IL2CPP::IMPORT::il2cpp_field_get_type(field);
			if (type->type != IL2CPP::IMPORT::Il2CppTypeEnum::IL2CPP_TYPE_STRING) continue;

			int32_t offset = IL2CPP::IMPORT::il2cpp_field_get_offset(field);

			IL2CPP::String** stringPtrAddr = (IL2CPP::String**)(staticBase + offset);

			if (!stringPtrAddr) continue;

			IL2CPP::String* il2cppStr = *stringPtrAddr;

			if (il2cppStr) {
				std::string val = il2cppStr->ToString();
				const char* fName = IL2CPP::IMPORT::il2cpp_field_get_name(field);
				std::string sFieldName = fName ? fName : ("field_" + std::to_string(count));

				size_t pos = 0;
				while ((pos = val.find("\"", pos)) != std::string::npos) {
					val.replace(pos, 1, "\\\"");
					pos += 2;
				}
				val.erase(std::remove(val.begin(), val.end(), '\n'), val.end());
				val.erase(std::remove(val.begin(), val.end(), '\r'), val.end());

				out << "\tpublic static readonly string " << sFieldName << " = \"" << val << "\";\n";
				count++;
			}
		}

		out << "}\n";
		out.close();

		std::cout << "Event Names Dumped: " << count << " items." << std::endl;
	}

	void Init()
	{
		// Experience Hooks
		MakeAHookHerePls("ExperienceController", 0x1c, ExperienceController::AddXP, (LPVOID*)&ExperienceController::AddXP_o);
		MakeAHookHerePls("ExperienceController", 0x6, ExperienceController::GetLevel, (LPVOID*)&ExperienceController::GetLevel_o);

		// Sync Hook
		MakeAHookHereButMappedPls("ProgressSyncController", 0x5, ProgressSyncController::DoSyncSafe_Detour, (LPVOID*)&ProgressSyncController::DoSyncSafe_o);
	}
}
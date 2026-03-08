#include "Login.hpp"
#include "../Utils/Il2CppHelper.hpp"
#include "../Variables/Variables.hpp"

namespace Login
{
	void HandleTokenOutput()
	{
		auto klass = IL2CPP::ClassMapping::GetClass("AccountDataClassV2");
		if (!klass) return;

		auto rawKlass = (IL2CPP::IMPORT::Il2CppClass*)klass;
		IL2CPP::IMPORT::il2cpp_runtime_class_init(rawKlass);

		auto mGetInstance = (rawKlass->method_count > 0x19) ? rawKlass->methods[0x19] : nullptr;
		if (!mGetInstance) return;

		auto instance = (IL2CPP::Object*)IL2CPP::IMPORT::il2cpp_runtime_invoke(mGetInstance, nullptr, nullptr, nullptr);
		if (!instance) return;

		auto mGetToken = (rawKlass->method_count > 0x1D) ? rawKlass->methods[0x1D] : nullptr;
		if (!mGetToken) return;

		auto tokenStr = (IL2CPP::String*)IL2CPP::IMPORT::il2cpp_runtime_invoke(mGetToken, instance, nullptr, nullptr);
		if (tokenStr) {
			std::cout << "\n=== TOKEN ===\n" << tokenStr->ToString() << "\n=============\n" << std::endl;
		}
	}

	void HandleAuthInfoOutput()
	{
		auto klass = IL2CPP::ClassMapping::GetClass("AccountDataClassV2");
		if (!klass) return;

		auto rawKlass = (IL2CPP::IMPORT::Il2CppClass*)klass;
		IL2CPP::IMPORT::il2cpp_runtime_class_init(rawKlass);

		auto mGetInstance = (rawKlass->method_count > 0x19) ? rawKlass->methods[0x19] : nullptr;
		if (!mGetInstance) return;

		auto instance = (IL2CPP::Object*)IL2CPP::IMPORT::il2cpp_runtime_invoke(mGetInstance, nullptr, nullptr, nullptr);
		if (!instance) return;

		auto mGetID = (rawKlass->method_count > 0x7) ? rawKlass->methods[0x7] : nullptr;
		auto mGetSecret = (rawKlass->method_count > 0x9) ? rawKlass->methods[0x9] : nullptr;

		auto idStr = mGetID ? (IL2CPP::String*)IL2CPP::IMPORT::il2cpp_runtime_invoke(mGetID, instance, nullptr, nullptr) : nullptr;
		auto secretStr = mGetSecret ? (IL2CPP::String*)IL2CPP::IMPORT::il2cpp_runtime_invoke(mGetSecret, instance, nullptr, nullptr) : nullptr;

		std::cout << "\n=== AUTH INFO ===" << std::endl;
		std::cout << "ID: " << (idStr ? idStr->ToString() : "[NULL]") << std::endl;
		std::cout << "Secret: " << (secretStr ? secretStr->ToString() : "[NULL]") << std::endl;
		std::cout << "=================\n" << std::endl;
	}

	static int FindFirstPrivateIEnumerator(IL2CPP::IMPORT::Il2CppClass* klass)
	{
		for (int i = 0; i < klass->method_count; i++)
		{
			auto method = klass->methods[i];
			if (!method) continue;

			auto returnType = IL2CPP::IMPORT::il2cpp_method_get_return_type(method);
			if (!returnType) continue;

			auto returnClass = IL2CPP::IMPORT::il2cpp_class_from_type(returnType);
			if (!returnClass || !returnClass->name) continue;

			if (std::string(returnClass->name) != "IEnumerator") continue;

			int paramCount = IL2CPP::IMPORT::il2cpp_method_get_param_count(method);
			if (paramCount != 0) continue;

			uint32_t iflags = 0;
			auto flags = IL2CPP::IMPORT::il2cpp_method_get_flags(method, &iflags);
			auto access = flags & METHOD_ATTRIBUTE_MEMBER_ACCESS_MASK;

			if (access == METHOD_ATTRIBUTE_PRIVATE) return i;
		}
		return -1;
	}

	static void TriggerAuthSequence(void* instance, IL2CPP::IMPORT::Il2CppClass* klass)
	{
		int firstIEnumerator = FindFirstPrivateIEnumerator(klass);
		if (firstIEnumerator < 0) return;

		std::vector<int> sequence = { -2, -1, 2, 3, 4, -2, -2 };

		for (int offset : sequence)
		{
			int index = firstIEnumerator + offset;
			if (index >= 0 && index < klass->method_count)
			{
				auto method = klass->methods[index];
				if (method)
				{
					IL2CPP::IMPORT::il2cpp_runtime_invoke(method, instance, nullptr, nullptr);
				}
			}
		}
	}

	void HandleLoginWithToken()
	{
		static bool isLoginInProgress = false;

		if (isLoginInProgress) {
			std::cout << "[TOKEN LOGIN] Already in progress, ignoring duplicate call" << std::endl;
			return;
		}

		isLoginInProgress = true;
		std::cout << "[TOKEN LOGIN] Starting token login..." << std::endl;

		auto klass = IL2CPP::ClassMapping::GetClass("AccountDataClassV2");
		if (!klass) {
			std::cout << "[TOKEN LOGIN] ERROR: Failed to get class!" << std::endl;
			isLoginInProgress = false;
			return;
		}

		auto rawKlass = (IL2CPP::IMPORT::Il2CppClass*)klass;
		IL2CPP::IMPORT::il2cpp_runtime_class_init(rawKlass);

		auto mGetInstance = (rawKlass->method_count > 0x19) ? rawKlass->methods[0x19] : nullptr;
		if (!mGetInstance) {
			std::cout << "[TOKEN LOGIN] ERROR: Failed to get instance method!" << std::endl;
			isLoginInProgress = false;
			return;
		}

		auto instance = (IL2CPP::Object*)IL2CPP::IMPORT::il2cpp_runtime_invoke(mGetInstance, nullptr, nullptr, nullptr);
		if (!instance) {
			std::cout << "[TOKEN LOGIN] ERROR: Failed to get instance!" << std::endl;
			isLoginInProgress = false;
			return;
		}

		auto mSetToken = (rawKlass->method_count > 0x1E) ? rawKlass->methods[0x1E] : nullptr;
		auto mGetToken = (rawKlass->method_count > 0x1D) ? rawKlass->methods[0x1D] : nullptr;

		if (mSetToken && !Variables::Miscellaneous::m_sLoginToken.empty())
		{
			std::cout << "[TOKEN LOGIN] Setting token to: " << Variables::Miscellaneous::m_sLoginToken << std::endl;
			auto tokenString = IL2CPP::IMPORT::il2cpp_string_new(Variables::Miscellaneous::m_sLoginToken.c_str());
			void* argsToken[] = { tokenString };
			IL2CPP::IMPORT::il2cpp_runtime_invoke(mSetToken, instance, argsToken, nullptr);
			std::cout << "[TOKEN LOGIN] Token set!" << std::endl;

			if (mGetToken)
			{
				auto checkToken = (IL2CPP::String*)IL2CPP::IMPORT::il2cpp_runtime_invoke(mGetToken, instance, nullptr, nullptr);
				if (checkToken) {
					std::cout << "[TOKEN LOGIN] Verification - Token now reads as: " << checkToken->ToString() << std::endl;
				}
			}
		}

		std::cout << "[TOKEN LOGIN] Triggering auth sequence..." << std::endl;
		TriggerAuthSequence(instance, rawKlass);
		std::cout << "[TOKEN LOGIN] Auth sequence complete!" << std::endl;

		isLoginInProgress = false;
	}

	void HandleLoginWithIDSecret()
	{
		auto klass = IL2CPP::ClassMapping::GetClass("AccountDataClassV2");
		if (!klass) return;

		auto rawKlass = (IL2CPP::IMPORT::Il2CppClass*)klass;
		IL2CPP::IMPORT::il2cpp_runtime_class_init(rawKlass);

		auto mGetInstance = (rawKlass->method_count > 0x19) ? rawKlass->methods[0x19] : nullptr;
		if (!mGetInstance) return;

		auto instance = (IL2CPP::Object*)IL2CPP::IMPORT::il2cpp_runtime_invoke(mGetInstance, nullptr, nullptr, nullptr);
		if (!instance) return;

		auto mSetID = (rawKlass->method_count > 0x8) ? rawKlass->methods[0x8] : nullptr;
		auto mSetSecret = (rawKlass->method_count > 0xA) ? rawKlass->methods[0xA] : nullptr;

		if (mSetID && !Variables::Miscellaneous::m_sLoginID.empty())
		{
			auto idString = IL2CPP::IMPORT::il2cpp_string_new(Variables::Miscellaneous::m_sLoginID.c_str());
			void* argsID[] = { idString };
			IL2CPP::IMPORT::il2cpp_runtime_invoke(mSetID, instance, argsID, nullptr);
		}

		if (mSetSecret && !Variables::Miscellaneous::m_sLoginSecret.empty())
		{
			auto secretString = IL2CPP::IMPORT::il2cpp_string_new(Variables::Miscellaneous::m_sLoginSecret.c_str());
			void* argsSecret[] = { secretString };
			IL2CPP::IMPORT::il2cpp_runtime_invoke(mSetSecret, instance, argsSecret, nullptr);
		}

		TriggerAuthSequence(instance, rawKlass);
	}

	void Init()
	{
	}
}
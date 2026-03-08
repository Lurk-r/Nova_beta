#include "Gameplay.hpp"
#include "../Utils/Il2CppHelper.hpp"
#include "../Variables/Variables.hpp"
#include <vector>
#include <string>
#include <chrono>
#include <iostream>
#include <fstream>
#include "../Backend/Backend.hpp"

namespace Gameplay
{
	namespace Player_move_c
	{
		void(*Update_o)(IL2CPP::Object*) = nullptr;
		void Update(IL2CPP::Object* instance)
		{
			Update_o(instance);
		}
		void Init()
		{
			MakeAHookHerePls("Player_move_c", "Update", Update, (LPVOID*)&Update_o);
		}
	}

	namespace WeaponSounds
	{
		void(*Update_o)(IL2CPP::Object*) = nullptr;
		void Update(IL2CPP::Object* instance)

		{
			if (instance)
			{
				static uint64_t railgunOff = GET_FIELDOFFSET("WeaponSounds", "railgun");
				static uint64_t bazookaOff = GET_FIELDOFFSET("WeaponSounds", "bazooka");
				static uint64_t isDashOff = GET_FIELDOFFSET("WeaponSounds", "isDash");
				static uint64_t harpoonOff = GET_FIELDOFFSET("WeaponSounds", "harpoon");
				static uint64_t bulletExplodeOff = GET_FIELDOFFSET("WeaponSounds", "bulletExplode");
				static uint64_t isBlindEffectOff = GET_FIELDOFFSET("WeaponSounds", "isBlindEffect");
				static uint64_t blindEffectOff = GET_FIELDOFFSET("WeaponSounds", "blindEffect");
				static uint64_t isBlindEffectTimeOff = GET_FIELDOFFSET("WeaponSounds", "isBlindEffectTime");

				if (Variables::Gameplay::m_bRailgun && railgunOff)             *reinterpret_cast<bool*>((uint64_t)instance + railgunOff) = true;
				if (Variables::Gameplay::m_bBazooka && bazookaOff)             *reinterpret_cast<bool*>((uint64_t)instance + bazookaOff) = true;
				if (Variables::Gameplay::m_bDash && isDashOff)                 *reinterpret_cast<bool*>((uint64_t)instance + isDashOff) = true;
				if (Variables::Gameplay::m_bHarpoon && harpoonOff)             *reinterpret_cast<bool*>((uint64_t)instance + harpoonOff) = true;
				if (Variables::Gameplay::m_bExplodingBullets && bulletExplodeOff) *reinterpret_cast<bool*>((uint64_t)instance + bulletExplodeOff) = true;
				if (Variables::Gameplay::m_bBlindness && isBlindEffectOff && blindEffectOff && isBlindEffectTimeOff)
				{
					*reinterpret_cast<bool*>((uint64_t)instance + isBlindEffectOff) = true;
					*reinterpret_cast<int*>((uint64_t)instance + blindEffectOff) = (rand() % 60) + 1;
					*reinterpret_cast<float*>((uint64_t)instance + isBlindEffectTimeOff) = 9999.0f;
				}
			}
			Update_o(instance);
		}

		void Init()
		{
			MakeAHookHerePls("WeaponSounds", "Update", Update, (LPVOID*)&Update_o);
		}
	}

	namespace System_String
	{
		IL2CPP::String* (*Concat_o)(IL2CPP::String*, IL2CPP::String*, IL2CPP::String*, IL2CPP::String*) = nullptr;

		std::string GenerateSpam(const std::string& message, int maxCharsPerLine, int verticalRepeats)
		{
			if (message.empty()) return "";

			std::string result = "";
			int messageLength = message.length();

			for (int v = 0; v < verticalRepeats; v++)
			{
				int charsOnLine = 0;
				while (charsOnLine + messageLength <= maxCharsPerLine)
				{
					result += message;
					charsOnLine += messageLength;
				}
				result += "\n";
			}
			return result;
		}

		IL2CPP::String* Concat(IL2CPP::String* str0, IL2CPP::String* str1, IL2CPP::String* str2, IL2CPP::String* str3)
		{
			if (str1)
			{
				auto rawStr1 = (IL2CPP::IMPORT::Il2CppString*)str1;
				const auto* chars1 = IL2CPP::IMPORT::il2cpp_string_chars(rawStr1);
				if (chars1)
				{
					std::wstring wstr1(reinterpret_cast<const wchar_t*>(chars1));
					std::string username(wstr1.begin(), wstr1.end());

					if (username == Variables::Miscellaneous::m_sPlayerUsername)
					{
						std::string s0 = "";
						if (str0)
						{
							auto rawStr0 = (IL2CPP::IMPORT::Il2CppString*)str0;
							const auto* chars0 = IL2CPP::IMPORT::il2cpp_string_chars(rawStr0);
							if (chars0)
							{
								std::wstring wstr0(reinterpret_cast<const wchar_t*>(chars0));
								s0 = std::string(wstr0.begin(), wstr0.end());
							}
						}

						std::string s1 = username;

						std::string s2 = "";
						if (str2)
						{
							auto rawStr2 = (IL2CPP::IMPORT::Il2CppString*)str2;
							const auto* chars2 = IL2CPP::IMPORT::il2cpp_string_chars(rawStr2);
							if (chars2)
							{
								std::wstring wstr2(reinterpret_cast<const wchar_t*>(chars2));
								s2 = std::string(wstr2.begin(), wstr2.end());
							}
						}

						std::string s3 = "";
						if (str3)
						{
							auto rawStr3 = (IL2CPP::IMPORT::Il2CppString*)str3;
							const auto* chars3 = IL2CPP::IMPORT::il2cpp_string_chars(rawStr3);
							if (chars3)
							{
								std::wstring wstr3(reinterpret_cast<const wchar_t*>(chars3));
								s3 = std::string(wstr3.begin(), wstr3.end());
							}
						}

						std::string string0 = s0;
						std::string string1 = s1;
						std::string string2 = s2;
						std::string string3 = s3;

						if (Variables::Miscellaneous::m_bChatFullScreen)
						{
							int maxCharsPerLine = 145;
							int verticalLines = 50;
							std::string spamText = GenerateSpam(s3, maxCharsPerLine, verticalLines);
							string0 = spamText + s0;
							string3 = spamText;
						}
						else if (Variables::Miscellaneous::m_bChatSpam)
						{
							string0 = "[DEVELOPER] | < ";
							string1 = s1;
							string2 = " > | ";
							string3 = s3;
						}
						str0 = (IL2CPP::String*)IL2CPP::IMPORT::il2cpp_string_new(string0.c_str());
						str1 = (IL2CPP::String*)IL2CPP::IMPORT::il2cpp_string_new(string1.c_str());
						str2 = (IL2CPP::String*)IL2CPP::IMPORT::il2cpp_string_new(string2.c_str());
						str3 = (IL2CPP::String*)IL2CPP::IMPORT::il2cpp_string_new(string3.c_str());
					}
				}
			}

			return Concat_o(str0, str1, str2, str3);
		}

		void Init()
		{
			MakeAHookHereButMappedPls("String", 0x26, Concat, (LPVOID*)&Concat_o);
		}
	}
}
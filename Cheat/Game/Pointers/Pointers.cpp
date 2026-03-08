#include "Pointers.hpp"
#include "../../Utils/Il2CppHelper.hpp"
#include <map>

namespace Pointers
{
    template <typename F>
    void CallOnce(F&& fn) {
        static bool initialized = false;
        if (initialized) return;
        initialized = true;
        fn();
    }
    constexpr const char* CoreModule = "UnityEngine.CoreModule.dll";
    IL2CPP::Object* SocketInstance = nullptr;

    // Pointer Wrapper
    namespace Json
    {
        Pointer<IL2CPP::Object* (IL2CPP::String* json)> Decode("Json", "Decode");
        Pointer<IL2CPP::String* (IL2CPP::Object* json)> Encode("Json", "Encode");
        Pointer<IL2CPP::String* (IL2CPP::Object* json)> SerializeObject("JsonConvert", "SerializeObject");

        void Init()
        {
            CallOnce([] {
                Decode.Define();
                Encode.Define();
                SerializeObject.Define();
                });
        }
    }

    namespace ContentRegister
    {
        Pointer<IL2CPP::Object* ()> GetInstance("ContentRegister", 0x47);
        Pointer<IL2CPP::List<IL2CPP::String*>* (IL2CPP::Object* instance, int type)> GetRegisterList("ContentRegister", 0x54);
        Pointer<int(IL2CPP::Object* instance, int type, IL2CPP::String* key)> GetItemIndex("ContentRegister", 0x4f);

        void Init()
        {
            CallOnce([] {
                GetInstance.Define();
                GetRegisterList.Define();
                GetItemIndex.Define();
                });
        }
    }

    namespace OfferItem
    {
        Pointer<IL2CPP::Object* (IL2CPP::String* id)> GetItemByString("OfferItem", 0x0);
        Pointer<IL2CPP::Object* (int type, IL2CPP::String* id, int count)> GetItemTyped("OfferItem", 0x2);
        Pointer<IL2CPP::String* (IL2CPP::Object* instance)> GetName("OfferItem", 0x23);
        Pointer<IL2CPP::String* (IL2CPP::Object* instance)> GetNameDefault("OfferItem", 0x24);

        void Init()
        {
            CallOnce([] {
                GetItemByString.Define();
                GetItemTyped.Define();
                GetName.Define();
                GetNameDefault.Define();
                });
        }
    }

    namespace Slots
    {
        void (*ApplySlots)(IL2CPP::Object* slots);
    }

    namespace MainPointers
    {
        IL2CPP::Wrapper::Method<IL2CPP::String* ()> GetVersion, GetID, GetUsername;
        IL2CPP::Wrapper::Method<int()> GetLevel;
        void Init()
        {
            GetVersion = GetMethodPointer("UnityEngine.Application", "get_version", CoreModule);
            GetID = GetMappedPointer("ProfileClass", 0x2);
            GetUsername = GetMethodPointer("ProfileController", 0x34);
            GetLevel = GetMethodPointer("ExperienceController", 0x6);
        }
    }

    namespace ItemRecord
    {
        Pointer<int(IL2CPP::Object* instance)> get_Rarity("ItemRecordMapped", "get_Rarity");
        Pointer<int(IL2CPP::Object* instance)> get_Category("ItemRecordMapped", "get_Category");
        Pointer<bool(IL2CPP::Object* instance)> get_IsCraft("ItemRecordMapped", "get_IsCraft");
        Pointer<int(IL2CPP::Object* instance)> get_WeaponNumber("ItemRecordMapped", "get_WeaponNumber");
        Pointer<bool(IL2CPP::Object* instance)> get_IsRoyale("ItemRecordMapped", "get_IsRoyale");

        std::string GetRarityStr(int r) {
            switch (r) {
            case 0: return "Common";
            case 1: return "Uncommon";
            case 2: return "Rare";
            case 3: return "Epic";
            case 4: return "Legendary";
            case 5: return "Mythic";
            default: return "None";
            }
        }

        std::string GetCategoryStr(int c) {
            static const std::map<int, std::string> cats = {
                {0, "Primary"}, {1, "Backup"}, {2, "Melee"}, {3, "Special"}, {4, "Sniper"}, {5, "Heavy"},
                {6, "Hats"}, {7, "Armor"}, {8, "Skins"}, {9, "Capes"}, {10, "Boots"}, {11, "Gear"}, {12, "Mask"},
                {25000, "Pets"}, {30000, "Eggs"}, {220000, "Module"}, {260000, "ArmorSkin"}, {270000, "WearSkin"}
            };
            if (cats.count(c)) return cats.at(c);
            return std::to_string(c);
        }

        void Init()
        {
            CallOnce([] {
                get_Rarity.Define();
                get_Category.Define();
                get_IsCraft.Define();
                get_WeaponNumber.Define();
                get_IsRoyale.Define();
                });
        }
    }

    namespace ItemDb {
        Pointer<IL2CPP::Object* (int index)> GetByIndex("ItemDb", 0x7);
        Pointer<IL2CPP::Object* (IL2CPP::String* tag)> GetByTag("ItemDb", 0xe);
        Pointer<int(IL2CPP::String* tag)> GetItemCategory("ItemDb", 0x13);

        void Init() {
            CallOnce([] {
                GetByIndex.Define();
                GetByTag.Define();
                GetItemCategory.Define();
                });
        }
    }

    namespace RoyaleOITClass {
        Pointer<void* ()> RoyaleDictionary("RoyaleDictionary", 0x2);

        void Init() {
            CallOnce([] {
                RoyaleDictionary.Define();
                });
        }
    }

    void Init()
    {
        Json::Init();
        ContentRegister::Init();
        OfferItem::Init();
        ItemRecord::Init();
        ItemDb::Init();
        MainPointers::Init();
        RoyaleOITClass::Init();
    }
}
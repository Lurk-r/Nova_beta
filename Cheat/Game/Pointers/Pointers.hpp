#pragma once
#include "PointerWrapper.hpp"

namespace Pointers
{
    // Dummy Pointers
    extern IL2CPP::Object* SocketInstance;

    // Pointer Wrapper
    namespace Json
    {
        extern Pointer<IL2CPP::Object* (IL2CPP::String* json)> Decode;
        extern Pointer<IL2CPP::String* (IL2CPP::Object* json)> Encode;
        extern Pointer<IL2CPP::String* (IL2CPP::Object* json)> SerializeObject;
    }

    namespace ContentRegister
    {
        extern Pointer<IL2CPP::Object* ()> GetInstance;
        extern Pointer<IL2CPP::List<IL2CPP::String*>* (IL2CPP::Object* instance, int type)> GetRegisterList;
        extern Pointer<int(IL2CPP::Object* instance, int type, IL2CPP::String* key)> GetItemIndex;
    }

    namespace OfferItem
    {
        extern Pointer<IL2CPP::Object* (IL2CPP::String* id)> GetItemByString;
        extern Pointer<IL2CPP::Object* (int type, IL2CPP::String* id, int count)> GetItemTyped;
        extern Pointer<IL2CPP::String* (IL2CPP::Object* instance)> GetName;
        extern Pointer<IL2CPP::String* (IL2CPP::Object* instance)> GetNameDefault;
    }

    namespace MainPointers
    {
        extern IL2CPP::Wrapper::Method<IL2CPP::String* ()> GetVersion;
        extern IL2CPP::Wrapper::Method<IL2CPP::String* ()> GetID;
        extern IL2CPP::Wrapper::Method<IL2CPP::String* ()> GetUsername;
        extern IL2CPP::Wrapper::Method<int()> GetLevel;
    }

    namespace ItemRecord
    {
        extern Pointer<int(IL2CPP::Object* instance)> get_Rarity;
        extern Pointer<int(IL2CPP::Object* instance)> get_Category;
        extern Pointer<bool(IL2CPP::Object* instance)> get_IsCraft;
        extern Pointer<int(IL2CPP::Object* instance)> get_WeaponNumber;
        extern Pointer<bool(IL2CPP::Object* instance)> get_IsRoyale;

        std::string GetRarityStr(int r);
        std::string GetCategoryStr(int c);
    }

    namespace ItemDb
    {
        extern Pointer<IL2CPP::Object* (int index)> GetByIndex;
        extern Pointer<IL2CPP::Object* (IL2CPP::String* tag)> GetByTag;
        extern Pointer<int(IL2CPP::String* tag)> GetItemCategory;
    }

    namespace RoyaleOITClass
    {
        extern Pointer<void* ()> RoyaleDictionary;
    }

    void Init();
}
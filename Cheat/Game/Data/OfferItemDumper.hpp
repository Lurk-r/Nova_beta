#pragma once
#include <Unity/IL2CPP.hpp>
#include <Json.hpp>
#include <fstream>
#include <filesystem>
#include <map>
#include <vector>
#include "../Pointers/Pointers.hpp"

namespace OfferItemDumper
{
    struct ItemData {
        std::string GroupCategory;
        int TypeID;
        int Index;
        std::string ID;
        std::string Name;
        std::string InGameCategory;
        std::string Rarity;
        bool IsCraftable;
        int GalleryNumber;
        bool IsRoyale;
        bool IsShovel;
    };

    extern std::vector<ItemData> CachedItems;

    void LoadItemsToMemory();
    void DumpAllItemsJson();
    void DumpMemoryToFile();
}
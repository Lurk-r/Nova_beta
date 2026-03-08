#pragma once
#include <vector>
#include <string>
#include "OfferItemDumper.hpp"

namespace AdderLists
{
    struct WeaponInfo
    {
        int Index;
        std::string ID;
        std::string Name;
        std::string Category;
        std::string Rarity;
        bool IsCraftable;
        int GalleryNumber;
    };

    struct WearInfo
    {
        int Index;
        std::string Name;
    };

    struct ProfileInfo
    {
        int Index;
        std::string Name;
    };

    struct GadgetInfo
    {
        int Index;
        std::string Name;
    };

    struct GraffitiInfo
    {
        int Index;
        std::string Name;
    };

    struct LobbyInfo
    {
        int Index;
        std::string Name;
    };

    struct PetInfo
    {
        int Index;
        std::string Name;
    };

    struct RoyaleInfo
    {
        int Index;
        std::string Name;
        std::string SubCategory;
    };

    struct UpgradeInfo
    {
        int Index;
        std::string Name;
    };

    extern std::vector<WeaponInfo> AllWeapons;
    extern std::vector<WeaponInfo> PrimaryWeapons;
    extern std::vector<WeaponInfo> BackupWeapons;
    extern std::vector<WeaponInfo> MeleeWeapons;
    extern std::vector<WeaponInfo> SpecialWeapons;
    extern std::vector<WeaponInfo> SniperWeapons;
    extern std::vector<WeaponInfo> HeavyWeapons;
    extern std::vector<WeaponInfo> CraftableWeapons;

    extern std::vector<WearInfo> Armor;
    extern std::vector<WearInfo> Boots;
    extern std::vector<WearInfo> Capes;
    extern std::vector<WearInfo> Hats;
    extern std::vector<WearInfo> Masks;
    extern std::vector<WearInfo> WearSkins;

    extern std::vector<ProfileInfo> Icons;
    extern std::vector<ProfileInfo> Frames;
    extern std::vector<ProfileInfo> Backgrounds;

    extern std::vector<GadgetInfo> Gadgets;
    extern std::vector<GraffitiInfo> Graffiti;
    extern std::vector<LobbyInfo> LobbyItems;
    extern std::vector<PetInfo> Pets;
    extern std::vector<RoyaleInfo> Royale;
    extern std::vector<UpgradeInfo> FreeUpgrades;

    extern std::vector<int> ModulePoints;

    extern std::vector<int> GliderIndexes;
    extern std::vector<int> AvatarIndexes;
    extern std::vector<int> TrailIndexes;
    extern std::vector<int> CarIndexes;
    extern std::vector<int> ShovelIndexes;

    void LoadListsFromCache();

    const WeaponInfo* FindByName(const std::string& name);
    const WeaponInfo* FindByGallery(int galleryNumber);
    const WeaponInfo* FindByIndex(int index);

    int GetRarityValue(const std::string& rarity);
}
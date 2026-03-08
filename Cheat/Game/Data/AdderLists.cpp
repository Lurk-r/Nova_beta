#include "AdderLists.hpp"
#include <algorithm>
#include <cctype>
#include <set>

namespace AdderLists
{
    std::vector<WeaponInfo> AllWeapons;
    std::vector<WeaponInfo> PrimaryWeapons;
    std::vector<WeaponInfo> BackupWeapons;
    std::vector<WeaponInfo> MeleeWeapons;
    std::vector<WeaponInfo> SpecialWeapons;
    std::vector<WeaponInfo> SniperWeapons;
    std::vector<WeaponInfo> HeavyWeapons;
    std::vector<WeaponInfo> CraftableWeapons;

    std::vector<WearInfo> Armor;
    std::vector<WearInfo> Boots;
    std::vector<WearInfo> Capes;
    std::vector<WearInfo> Hats;
    std::vector<WearInfo> Masks;
    std::vector<WearInfo> WearSkins;

    std::vector<ProfileInfo> Icons;
    std::vector<ProfileInfo> Frames;
    std::vector<ProfileInfo> Backgrounds;

    std::vector<GadgetInfo> Gadgets;
    std::vector<GraffitiInfo> Graffiti;
    std::vector<LobbyInfo> LobbyItems;
    std::vector<PetInfo> Pets;
    std::vector<RoyaleInfo> Royale;
    std::vector<UpgradeInfo> FreeUpgrades;

    std::vector<int> ModulePoints;

    std::vector<int> GliderIndexes;
    std::vector<int> AvatarIndexes;
    std::vector<int> TrailIndexes;
    std::vector<int> CarIndexes;
    std::vector<int> ShovelIndexes;

    static const std::set<std::string> GadgetFilter = {
        "gadget_class_bullet_medkit", "gadget_class_bullet_phonixblood", "gadget_class_knight_magnet",
        "gadget_class_valk_jetpack", "gadget_class_valk_cyberbees", "gadget_class_lifeline_medstation",
        "gadget_class_lifeline_biotiksheald", "gadget_class_sara_hook", "gadget_class_sara_xray",
        "gadget_class_saber_shurikens", "gadget_class_saber_reflector", "gadget_class_mex_turret",
        "gadget_class_mex_nucleargrenade", "gadget_class_viktor_smilemine", "gadget_class_viktor_nutcracker",
        "gadget_class_saber_stealth", "gadget_br_fraggrenade", "gadget_br_molotov", "gadget_br_mine",
        "gadget_hook_monster", "gadget_mh_mech1", "gadget_mh_mech2", "gadget_mh_mech3"
    };

    static bool iequals(const std::string& a, const std::string& b)
    {
        if (a.length() != b.length()) return false;
        return std::equal(a.begin(), a.end(), b.begin(),
            [](char a, char b) {
                return std::tolower(static_cast<unsigned char>(a)) ==
                    std::tolower(static_cast<unsigned char>(b));
            });
    }

    void LoadListsFromCache()
    {
        AllWeapons.clear();
        PrimaryWeapons.clear();
        BackupWeapons.clear();
        MeleeWeapons.clear();
        SpecialWeapons.clear();
        SniperWeapons.clear();
        HeavyWeapons.clear();
        CraftableWeapons.clear();

        Armor.clear();
        Boots.clear();
        Capes.clear();
        Hats.clear();
        Masks.clear();
        WearSkins.clear();

        Icons.clear();
        Frames.clear();
        Backgrounds.clear();

        Gadgets.clear();
        Graffiti.clear();
        LobbyItems.clear();
        Pets.clear();
        Royale.clear();
        FreeUpgrades.clear();
        ModulePoints.clear();

        GliderIndexes.clear();
        AvatarIndexes.clear();
        TrailIndexes.clear();
        CarIndexes.clear();
        ShovelIndexes.clear();

        for (const auto& item : OfferItemDumper::CachedItems)
        {
            if (item.TypeID == 1155)
            {
                ModulePoints.push_back(item.Index);
                continue;
            }

            if (item.Name.empty() || item.Name == "not found") continue;

            switch (item.TypeID)
            {
            case 10:
                if (!item.InGameCategory.empty())
                {
                    WeaponInfo w = { item.Index, item.ID, item.Name, item.InGameCategory, item.Rarity, item.IsCraftable, item.GalleryNumber };
                    AllWeapons.push_back(w);
                    if (w.Category == "Primary") PrimaryWeapons.push_back(w);
                    else if (w.Category == "Backup") BackupWeapons.push_back(w);
                    else if (w.Category == "Melee") MeleeWeapons.push_back(w);
                    else if (w.Category == "Special") SpecialWeapons.push_back(w);
                    else if (w.Category == "Sniper") SniperWeapons.push_back(w);
                    else if (w.Category == "Heavy") HeavyWeapons.push_back(w);
                    if (w.IsCraftable) CraftableWeapons.push_back(w);
                    if (item.IsShovel) ShovelIndexes.push_back(item.Index);
                }
                break;

            case 20: Armor.push_back({ item.Index, item.Name }); break;
            case 50: Boots.push_back({ item.Index, item.Name }); break;
            case 60: Capes.push_back({ item.Index, item.Name }); break;
            case 40: Hats.push_back({ item.Index, item.Name }); break;
            case 30: Masks.push_back({ item.Index, item.Name }); break;
            case 1510: WearSkins.push_back({ item.Index, item.Name }); break;

            case 1630: Icons.push_back({ item.Index, item.Name }); break;
            case 1620: Frames.push_back({ item.Index, item.Name }); break;
            case 1610: Backgrounds.push_back({ item.Index, item.Name }); break;

            case 70:
                if (GadgetFilter.find(item.ID) == GadgetFilter.end())
                    Gadgets.push_back({ item.Index, item.Name });
                break;

            case 1470: Graffiti.push_back({ item.Index, item.Name }); break;
            case 85: LobbyItems.push_back({ item.Index, item.Name }); break;
            case 80: Pets.push_back({ item.Index, item.Name }); break;

            case 1080:
                Royale.push_back({ item.Index, item.Name, item.InGameCategory });

                if (item.InGameCategory == "GlidersRoyale")        GliderIndexes.push_back(item.Index);
                else if (item.InGameCategory == "RoyaleAvatars")   AvatarIndexes.push_back(item.Index);
                else if (item.InGameCategory == "TrailRoyale")     TrailIndexes.push_back(item.Index);
                else if (item.InGameCategory == "TransportRoyale") CarIndexes.push_back(item.Index);
                break;

            case 1380: FreeUpgrades.push_back({ item.Index, item.Name }); break;
            }
        }
    }

    const WeaponInfo* FindByName(const std::string& name)
    {
        auto it = std::find_if(AllWeapons.begin(), AllWeapons.end(),
            [&name](const WeaponInfo& w) { return iequals(w.Name, name); });
        return (it != AllWeapons.end()) ? &(*it) : nullptr;
    }

    const WeaponInfo* FindByGallery(int galleryNumber)
    {
        auto it = std::find_if(AllWeapons.begin(), AllWeapons.end(),
            [galleryNumber](const WeaponInfo& w) { return w.GalleryNumber == galleryNumber; });
        return (it != AllWeapons.end()) ? &(*it) : nullptr;
    }

    const WeaponInfo* FindByIndex(int index)
    {
        auto it = std::find_if(AllWeapons.begin(), AllWeapons.end(),
            [index](const WeaponInfo& w) { return w.Index == index; });
        return (it != AllWeapons.end()) ? &(*it) : nullptr;
    }

    int GetRarityValue(const std::string& rarity)
    {
        if (rarity == "Common") return 0;
        if (rarity == "Uncommon") return 1;
        if (rarity == "Rare") return 2;
        if (rarity == "Epic") return 3;
        if (rarity == "Legendary") return 4;
        if (rarity == "Mythic") return 5;
        return 0;
    }
}
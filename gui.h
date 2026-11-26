#ifndef GUI_H
#define GUI_H

#include "CGGL/cggl.h"

#include "game.h"

enum Rarity {
    Common,
    Rare,
    Epic,
    Legendary,
    Mythical,
    Heavenly
};

// https://www.color-hex.com/color-palette/38466
inline const cg::Color rarityColors[6] = {
    cg::Color(255,255,255),
    cg::Color(0,112,221),
    cg::Color(163,53,238),
    cg::Color(255,128,0),
    cg::Color(245,66,230),
    cg::Color(255,234,20)
};

struct CosmeticInfo {
    std::string name;
    std::string description;
    std::string texPath;
    int price;
    Rarity rarity;
    float rotationOffset = 0.f;
};

class StoreItem {
    public:
        StoreItem() {}
        StoreItem(cg::Texture* tex, const CosmeticInfo& info);

        void Draw(float offset, int index, float& money, cg::Texture*& targetTex, cg::Font* descriptionFont);

        int price;
        Rarity rarity;
        std::string name, description;
        cg::Texture* tex = nullptr;
        bool owned = false;
};

enum TexScalingMethod {
    TSCLinear,
    TSCNearest
};

struct StoreCatalogue {
    int nItems = 0;
    std::vector<CosmeticInfo> cosmeticsInfo;
    TexScalingMethod scalingMethod;

    std::vector<cg::Texture*> textures;

    // First item is assumed to be the default.
    std::vector<StoreItem> storeItems;
    cg::Texture** targetTexture = nullptr;
};

// Ids are currently not used, however you should still update them.
// If you don't, LastItem will be wrong which is actually in use.
enum CharacterIds {
    CIDCoolDude,
    CIDColdFace,
    CIDCryingFace,
    CIDFlabbergasted,
    CIDSkull,
    CIDVerity,
    CIDAngryFace,
    CIDPsychopath,
    CIDDinosaur,
    CIDLaughing,
    CIDAga,
    CIDLastItem
};

/*
        {
            name, 
            desc, 
            path, 
            price, 
            rarity
        },
*/
inline StoreCatalogue characterStore {
    CIDLastItem,

    {
        {
            "Cool Dude",
            "A cool dude. He likes his sunglasses.\n\nNo perks.",
            "tex/characters/cooldude.png",
            0,
            Common
        },
        {
            "Cold Face",
            "Someone turn the AC off!\n\n- Freezing: Time moves slower.",
            "tex/characters/coldface.png",
            99,
            Common
        },
        {
            "Crying Face",
            "Become #1 on the leaderboard to shush them.\n\n- Propelling Tears:\nLaunches tears that decrease your gravity.",
            "tex/characters/cryingface.png",
            69,
            Common
        },
        {
            "Flabbergasted",
            "What are they shocked at?!\n\n- Shocked: Coins are worth +1.",
            "tex/characters/flabbergasted.png",
            199,
            Rare
        },
        {
            "Skull",
            ":skull:\n\n- Undead: Immune to death but coins are nerfed.",
            "tex/characters/skull.png",
            499,
            Epic
        },
        {
            "Verity",
            "Ask him anything! He knows everything.\n\n- Verity: Predicts what's coming next.",
            "tex/characters/verity.png",
            999,
            Legendary
        },
        {
            "Angry Face",
            "Why'd you make them mad??\n\n- Rage: Speeds up time.",
            "tex/characters/angryface.png",
            599,
            Epic
        },
        {
            "Psychopath",
            "Who knows what they're up to...\n\n- Crazy: On RMB, teleport to the nearest coin.",
            "tex/characters/psychopath.png",
            1299,
            Legendary
        },
        {
            "Dinosaur",
            "They look stupid.\n\n- Apex Predator: Coins run away from you,\nbut they're worth more.",
            "tex/characters/dinosaur.png",
            549,
            Epic
        },
        {
            "Laughing Face",
            "They're just here to have fun :)\n\n- Curious: You attract coins, but they're worth less.",
            "tex/characters/laughing.png",
            549,
            Epic
        },
        {
            "Aga",
            "Aga.\n\n- Aga: Aga.",
            "tex/characters/aga.png",
            1999,
            Mythical
        }
    },

    TSCLinear
};

enum GunIds {
    GIDDefault,
    GIDRustyBarrel,
    GIDAK47,
    GIDSawedOffShotgun,
    GIDRevolver,
    GIDFlamethrower,
    GIDGrenadeLauncher,
    GIDSniperRifle,
    GIDSniperRifleGreen,
    GIDBazooka,
    GIDSubmachineGun,
    GIDLaserGun,
    GIDGasterBlaster,
    GIDLastItem
};

inline StoreCatalogue gunStore {
    GIDLastItem,

    {
        {
            "Pistol",
            "Slow reload time and power. It just sucks.",
            "tex/guns/1.png",
            0,
            Common,
        },
        {
            "Rusty Barrel",
            "An old, rusty and boring shotgun.",
            "tex/guns/shotgun.png",
            99,
            Common
        },
        {
            "AK-47",
            "Powerful assault rifle. Shoots very fast.",
            "tex/guns/AK47.png",
            299,
            Rare
        },
        {
            "Sawed-Off\nShotgun",
            "A shotgun with it's edge sawed off.\n\n- Spread: You get launched higher..",
            "tex/guns/SawedOffShotgun.png",
            199,
            Rare
        },
        {
            "Revolver",
            "A revolver recovered from the 19th century. It's old.\n\n- Old Age: 1 in 10 chance for the gun not to work.",
            "tex/guns/Revolver.png",
            29,
            Common
        },
        {
            "Flamethrower",
            "Propels you with fire. Might be a bit unstable.\n\n- Extreme Heat: Coins melt into pure gold because of\nthe flamethrower's flames, making them worth more.",
            "tex/guns/38.png",
            1499,
            Mythical
        },
        {
            "Grenade\nLauncher",
            "Shoots grenades that launches you when exploding.\n\n- Grenades: Looks cooler, meaning more style points.",
            "tex/guns/44.png",
            799,
            Epic
        },
        {
            "Sniper\nRifle",
            "Insane power but insane reloading time.\n\n- Higher Power: More power, more style points.",
            "tex/guns/49.png",
            699,
            Epic
        },
        {
            "Green Sniper\nRifle",
            "Same as sniper rifle but green.\n\n- Higher Power: More power, more style points.",
            "tex/guns/50.png",
            749,
            Epic
        },
        {
            "Bazooka",
            "Shoots missiles.\n\n- Big Boy: Launches you twice when shooting.\nLong reload time.",
            "tex/guns/45.png",
            1099,
            Legendary
        },
        {
            "Submachine Gun",
            "Shoots really fast but has low power.",
            "tex/guns/15.png",
            399,
            Rare
        },
        {
            "Laser Gun",
            "Gravity is a thing of the past.",
            "tex/guns/Icon29_19.png",
            2499,
            Heavenly
        },
        {
            "Gaster Blaster",
            "Very, very interesting.",
            "tex/guns/gasterblaster.png",
            6666,
            Heavenly
        }
    },

    // Textures are pixel art so scale them appropiatly.
    TSCNearest
};

inline std::vector<StoreCatalogue*> storeCatalogues = {&characterStore, &gunStore};

void LoadStoreCatalogues(Player& player, float size);
void FreeStoreCatalogues();

void DrawStore(float& money, StoreCatalogue* store, cg::Font* descriptionFont);

void DrawLeaderboard(Highscore playerHighscore, cg::Texture* playerTex);

#endif
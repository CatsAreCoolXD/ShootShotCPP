#ifndef GUI_H
#define GUI_H

#include "CGGL/cggl.h"

class Player;
class Highscore;

enum Rarity {
    Common,
    Rare,
    Epic,
    Legendary,
    Mythical,
    Heavenly
};

enum StoreId {
    CharacterStoreId,
    GunStoreId
};
struct CosmeticInfo {
    int id;
    std::string name;
    std::string description;
    std::string texPath;
    int price;
    Rarity rarity;

    float gunPower = 1.f;
    float gunReloadTime = 0.25f; // Seconds
    float scale = 0.05f; // Scale for the guns
    float rotationOffset = 0.f;

    cg::Texture* tex = nullptr;
    bool owned = false;
    StoreId storeId;
};

enum TexScalingMethod {
    TSCLinear,
    TSCNearest
};

struct StoreCatalogue {
    int nItems = 0;
    StoreId storeId;
    std::vector<CosmeticInfo> cosmeticsInfo;
    TexScalingMethod scalingMethod;

    std::vector<cg::Texture*> textures;
};

// Ids are currently not used, however you should still update them.
// If you don't, LastItem will be wrong which is actually in use.
enum CharacterId {
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

enum GunId {
    GIDPistol,
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

#include "game.h"

// https://www.color-hex.com/color-palette/38466
inline const cg::Color rarityColors[6] = {
    cg::Color(255,255,255),
    cg::Color(0,112,221),
    cg::Color(163,53,238),
    cg::Color(255,128,0),
    cg::Color(245,66,230),
    cg::Color(255,234,20)
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
    CharacterStoreId,

    {
        {
            CIDCoolDude,
            "Cool Dude",
            "A cool dude. He likes his sunglasses.\n\nNo perks.",
            "tex/characters/cooldude.png",
            0,
            Common
        },
        {
            CIDColdFace,
            "Cold Face",
            "Someone turn the AC off!\n\n- Freezing: Time moves slower.",
            "tex/characters/coldface.png",
            99,
            Common
        },
        {
            CIDCryingFace,
            "Crying Face",
            "Become #1 on the leaderboard to shush them.\n\n- Propelling Tears:\nLaunches tears that decrease your gravity.",
            "tex/characters/cryingface.png",
            69,
            Common
        },
        {
            CIDFlabbergasted,
            "Flabbergasted",
            "What are they shocked at?!\n\n- Shocked: Coins are worth +1.",
            "tex/characters/flabbergasted.png",
            199,
            Rare
        },
        {
            CIDSkull,
            "Skull",
            ":skull:\n\n- Undead: Immune to death but coins are nerfed.",
            "tex/characters/skull.png",
            499,
            Epic
        },
        {
            CIDVerity,
            "Verity",
            "Ask him anything! He knows everything.\n\n- Verity: Predicts what's coming next.",
            "tex/characters/verity.png",
            999,
            Legendary
        },
        {
            CIDAngryFace,
            "Angry Face",
            "Why'd you make them mad??\n\n- Rage: Speeds up time.",
            "tex/characters/angryface.png",
            599,
            Epic
        },
        {
            CIDPsychopath,
            "Psychopath",
            "Who knows what they're up to...\n\n- Crazy: On RMB, teleport to the nearest coin.",
            "tex/characters/psychopath.png",
            1299,
            Legendary
        },
        {
            CIDDinosaur,
            "Dinosaur",
            "They look stupid.\n\n- Apex Predator: Coins run away from you,\nbut they're worth more.",
            "tex/characters/dinosaur.png",
            549,
            Epic
        },
        {
            CIDLaughing,
            "Laughing Face",
            "They're just here to have fun :)\n\n- Curious: You attract coins, but they're worth less.",
            "tex/characters/laughing.png",
            549,
            Epic
        },
        {
            CIDAga,
            "Aga",
            "Aga.\n\n- Aga: Aga.",
            "tex/characters/aga.png",
            1999,
            Mythical
        }
    },

    TSCLinear
};

#define GUN_VAR_INFINITY FLT_MAX

inline StoreCatalogue gunStore {
    GIDLastItem,
    GunStoreId,

    {
        {
            GIDPistol,
            "Pistol",
            "Slow reload time and power. It just sucks.",
            "tex/guns/1.png",
            0,
            Common,
            1.f,
            0.35f
        },
        {
            GIDRustyBarrel,
            "Rusty Barrel",
            "An old, rusty and boring shotgun.",
            "tex/guns/shotgun.png",
            99,
            Common,
            1.5f,
            0.6f
        },
        {
            GIDAK47,
            "AK-47",
            "Powerful assault rifle. Shoots very fast.",
            "tex/guns/AK47.png",
            299,
            Rare,
            0.75f,
            0.05f,
            0.1f
        },
        {
            GIDSawedOffShotgun,
            "Sawed-Off\nShotgun",
            "A shotgun with it's edge sawed off.\n\n- Spread: You get launched higher..",
            "tex/guns/SawedOffShotgun.png",
            199,
            Rare,
            2.f,
            0.6f
        },
        {
            GIDRevolver,
            "Revolver",
            "A revolver recovered from the 19th century. It's old.\n\n- Old Age: 1 in 10 chance for the gun not to work.",
            "tex/guns/Revolver.png",
            29,
            Common,
            1.2f,
            0.35f
        },
        {
            GIDFlamethrower,
            "Flamethrower",
            "Propels you with fire. Might be a bit unstable.\n\n- Extreme Heat: Coins melt into pure gold because of\nthe flamethrower's flames, making them worth more.",
            "tex/guns/38.png",
            1499,
            Mythical,
            0.2f,
            0.f,
            0.1f
        },
        {
            GIDGrenadeLauncher,
            "Grenade\nLauncher",
            "Shoots grenades that launches you when exploding.\n\n- Grenades: Looks cooler, meaning more style points.",
            "tex/guns/44.png",
            799,
            Epic,
            2.f,
            2.f,
            0.2f
        },
        {
            GIDSniperRifle,
            "Sniper\nRifle",
            "Insane power but insane reloading time.\n\n- Higher Power: More power, more style points.",
            "tex/guns/49.png",
            699,
            Epic,
            3.f,
            3.f,
            0.2f
        },
        {
            GIDSniperRifleGreen,
            "Green Sniper\nRifle",
            "Same as sniper rifle but green.\n\n- Higher Power: More power, more style points.",
            "tex/guns/50.png",
            749,
            Epic,
            3.f,
            3.f,
            0.2f
        },
        {
            GIDBazooka,
            "Bazooka",
            "Shoots missiles.\n\n- Big Boy: Launches you twice when shooting.\nLong reload time.",
            "tex/guns/45.png",
            1099,
            Legendary,
            4.f,
            4.f,
            0.25f
        },
        {
            GIDSubmachineGun,
            "Submachine Gun",
            "Shoots really fast but has low power.",
            "tex/guns/15.png",
            399,
            Rare,
            0.3f,
            0.085f
        },
        {
            GIDLaserGun,
            "Laser Gun",
            "Gravity is a thing of the past.",
            "tex/guns/Icon29_19.png",
            2499,
            Heavenly,
            GUN_VAR_INFINITY,
            0.f,
            0.1f,
            45.f
        },
        {
            GIDGasterBlaster,
            "Gaster Blaster",
            "Very, very interesting.",
            "tex/guns/gasterblaster.png",
            6666,
            Heavenly,
            GUN_VAR_INFINITY,
            0.f
        }
    },

    // Textures are pixel art so scale them appropriatly.
    TSCNearest
};

inline std::vector<StoreCatalogue*> storeCatalogues = {&characterStore, &gunStore};

void LoadStoreCatalogues(Player& player, float size);
void FreeStoreCatalogues();

void DrawStore(float& money, StoreCatalogue* store, Player& player, cg::Font* descriptionFont);

void DrawLeaderboard(Highscore playerHighscore, cg::Texture* playerTex);

#endif
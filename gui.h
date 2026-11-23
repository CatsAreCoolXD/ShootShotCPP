#ifndef GUI_H
#define GUI_H

#include "CGGL/cggl.h"

#include "game.h"

class StoreItem {
    public:
        StoreItem() {}
        StoreItem(cg::Texture* tex, int price, std::string name) : tex(tex), price(price), name(name) {}

        void Draw(float offset, int index, float& money, cg::Texture*& targetTex);

        int price;
        std::string name;
        cg::Texture* tex = nullptr;
        bool owned = false;
};

struct StoreCatalogue {
    int nItems = 0;
    std::vector<std::string> texturePaths;
    std::vector<std::string> names;
    std::vector<std::string> descriptions;
    std::vector<cg::Texture*> textures;

    // First item is assumed to be the default.
    std::vector<StoreItem> storeItems;
    cg::Texture** targetTexture = nullptr;
};

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

inline StoreCatalogue characterStore {
    CIDLastItem,

    // Texture paths
    {
        "tex/characters/cooldude.png",
        "tex/characters/coldface.png",
        "tex/characters/cryingface.png",
        "tex/characters/flabbergasted.png",
        "tex/characters/skull.png",
        "tex/characters/verity.png",
        "tex/characters/angryface.png",
        "tex/characters/psychopath.png",
        "tex/characters/dinosaur.png",
        "tex/characters/laughing.png",
        "tex/characters/aga.png"
    },

    // Names
    {
        "Cool Dude",
        "Cold Face",
        "Crying Face",
        "Flabbergasted",
        "Skull",
        "Verity",
        "Angry Face",
        "Psychopath",
        "Dinosaur",
        "Laughing Face",
        "Aga"
    },

    // Descriptions
    {
        "A cool dude. He likes his sunglasses.",
        "Someone turn the AC off!",
        "They're crying because you aren't #1 on the leaderboard.",
        "What is he shocked at?!",
        "Only in ohio",
        "Ask him anything! He knows everything.",
        "What'd you make them mad for??",
        "Who knows what they're up to...",
        "They look stupid.",
        "They're just here to have fun :)",
        "Maga is better."
    }
};

enum ShotgunIds {
    SIDDefault,
    SIDLastItem
};

inline StoreCatalogue shotgunStore {
    SIDLastItem,

    // Texture paths
    {
        "tex/shotguns/shotgun.png"
    },

    // Names
    {
        "Rusty Barrel",
    },

    // Descriptions
    {
        "An old, rusty shotgun you found in an abandened building."
    }
};

inline std::vector<StoreCatalogue*> storeCatalogues = {&characterStore, &shotgunStore};

void LoadStoreCatalogues(Player& player, float size);
void FreeStoreCatalogues();

void DrawStore(float& money, Player& player, StoreCatalogue* store);

void DrawLeaderboard(Highscore playerHighscore, cg::Texture* playerTex);

#endif
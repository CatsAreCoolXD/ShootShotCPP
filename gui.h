#ifndef GUI_H
#define GUI_H

#include "CGGL/cggl.h"

#include "game.h"

enum Characters {
    CoolDude,
    ColdFace,
    CryingFace,
    Flabbergasted,
    Skull,
    Verity,
    LastItem
};

inline std::vector<std::string> characterTexturePaths = {
    "tex/cooldude.png",
    "tex/coldface.png",
    "tex/cryingface.png",
    "tex/flabbergasted.png",
    "tex/skull.png",
    "tex/verity.png"
};

inline std::vector<std::string> characterNames = {
    "Cool Dude",
    "Cold Face",
    "Crying Face",
    "Flabbergasted",
    "Skull",
    "Verity"
};

inline std::vector<cg::Texture*> characterTextures(Characters::LastItem);

void LoadCharacterTextures(Player& player, float size);
void FreeCharacterTextures();
void DrawCharacterStore(float& money, Player& player);

class StoreItem {
    public:
        StoreItem() {}
        StoreItem(cg::Texture* tex, int price, std::string name) : tex(tex), price(price), name(name) {}

        void Draw(float offset, int index, float& money, Player& player);

        int price;
        std::string name;
        cg::Texture* tex = nullptr;
        bool owned = false;
};

inline std::vector<StoreItem> characterShopItems(Characters::LastItem);

#endif
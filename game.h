#ifndef GAME_H
#define GAME_H

#include "CGGL/cggl.h"
#include <vector>

class Player {
    public:
        Player(cg::Vec2f pos, float size);
        void Update();
        void Draw();

        void DeleteTextures();
    private:
        cg::Vec2f pos, velocity = cg::Vec2f(0,0);
        float size;
        cg::Texture sprite, shotgunTex;
};

class Coin {
    public:
        Coin(cg::Vec2f pos) : pos(pos) {}
        cg::Vec2f pos;
};

inline std::vector<Coin> coins;

inline void AddCoin(cg::Vec2f pos) {
    coins.emplace_back(pos);
}

void UpdateCoins(const Player& player);
void DrawCoins();

#endif
#ifndef GAME_H
#define GAME_H

#include "CGGL/cggl.h"
#include <vector>

#define COIN_SIZE cg::Vec2f(50,50)

enum GameScene {
    MainMenu,
    Game,
    DeathScreen,
    Transition
};

inline GameScene scene = GameScene::MainMenu;

class Player {
    public:
        Player(cg::Vec2f pos, float size);
        void Update();
        void Draw();

        void DeleteTextures();

        cg::Vec2f pos, velocity = cg::Vec2f(0,0);
        float size;
    private:
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

inline void GameOver() {
    std::cout << "Game Over!\n";
    scene = GameScene::DeathScreen;
}

void GenerateCoin();

void UpdateCoins(const Player& player);
void DrawCoins();

#endif
#ifndef GAME_H
#define GAME_H

#include "CGGL/cggl.h"
#include <vector>

#define COIN_SIZE cg::Vec2f(50,50)

enum GameScene {
    SceneDogCheck,
    SceneMainMenu,
    SceneGame,
    SceneDeathScreen,
    SceneStore
};

inline GameScene scene = GameScene::SceneMainMenu;

class Player {
    public:
        Player(cg::Vec2f pos, float size);
        void Update();
        void Draw();

        cg::Texture*& GetSpriteTexture() { return sprite; }
        cg::Texture*& GetGunTexture() { return gunTex; }
        void SetSprite(cg::Texture* tex) { sprite = tex; }
        void SetShotgunTexture(cg::Texture* tex) { gunTex = tex; }

        cg::Vec2f pos, velocity = cg::Vec2f(0,0), accel = cg::Vec2f(0,0);
        float size;
    private:
        cg::Texture* gunTex;
        cg::Texture* sprite = nullptr;
};

class Coin {
    public:
        Coin(cg::Vec2f pos) : pos(pos) {}
        cg::Vec2f pos;
};

inline int score = 0;
inline int coinsCollected = 0;
inline std::vector<Coin> coins;

inline void AddCoin(cg::Vec2f pos) {
    coins.emplace_back(pos);
}

void GameOver();

void ScaleTex(cg::Texture* tex, cg::Vec2f targetSize);

inline cg::Color backgroundTint;
inline const std::vector<cg::Color> backgroundColors = {
    cg::Color(255, 0, 0),
    cg::Color(0, 0, 255),
    cg::Color(255, 255, 0),
    cg::Color(255, 0, 255),
    cg::Color(0, 255, 255),
    cg::Color(255, 125, 0),
    cg::Color(125, 255, 0),
    cg::Color(0, 255, 125),
    cg::Color(0, 125, 255),
    cg::Color(125, 0, 255),
    cg::Color(255, 0, 125)
};

inline void NewBackgroundColor(){
    backgroundTint = backgroundColors[rand() % backgroundColors.size()];
}

void GenerateCoin();

void UpdateCoins(const Player& player);
void DrawCoins();

inline void UpdateSceneChange(GameScene targetScene, bool change){
    static GameScene queue;
    if (!change) queue = targetScene;
    else scene = queue;
}

struct Highscore {
    std::string user;
    int score;
    bool isPlayer = false;
    std::string pfpPath = "";
    cg::Texture* pfp = nullptr;
};

inline std::vector<Highscore> highscores = {
    Highscore{"Gabe Newell", 1231},
    Highscore{"Markiplier", 1229},
    Highscore{"Lord Verity", 1142},
    Highscore{"Jensen Huang", 1102},
    Highscore{"Sam Altman", 999},
    Highscore{"Jack Black", 866},
    Highscore{"Donald Duck", 666},
    Highscore{"MrBeast", 549},
    Highscore{"Horror Skunx", 476},
    Highscore{"Captain Sparklez", 322},
    Highscore{"Judelow", 301},
    Highscore{"Elon Musk", 265},
};

void LoadHighscoreProfiles();
void DeleteHighscoreProfiles();

#endif
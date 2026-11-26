#include "game.h"

#include "audio.h"
#include <format>

Player::Player(cg::Vec2f pos, float size) : pos(pos), size(size) {
    return;
    // shotgunTex.LoadImage("tex/shotgun.png");
    // shotgunTex.FlipVertically();
    // cg::Vec2f ogSize = shotgunTex.GetSize();
    // float scale = std::max(ogSize.x, ogSize.y) / (size * 0.75f);
    // shotgunTex.Scale(1.f /  scale);
}

void Player::Draw(){
    cg::Texture* sprite = character->tex;
    cg::Texture* gunTex = gun->tex;
    sprite->SetOrigin(pos, FLAG_CENTERED);

    // Draw 3 sprites for seamless teleportations from one side of the screen to another
    cg::Vec2f ogSize = sprite->GetSize();
    ScaleTex(sprite, cg::Vec2f(size,size));
    sprite->SetOrigin(pos - sprite->GetSize() / 2.f);
    cg::Draw(*sprite, FLAG_NO_BORDER);
    cg::Draw(*sprite, sprite->GetOrigin() + cg::Vec2f(cg::GetWindowWidth(), 0.f), sprite->GetSize(), FLAG_NO_BORDER);
    cg::Draw(*sprite, sprite->GetOrigin() - cg::Vec2f(cg::GetWindowWidth(), 0.f), sprite->GetSize(), FLAG_NO_BORDER);
    sprite->SetSize(ogSize);

    // Draw shotgun rotated towards the mouse. Todo: integrate this into the CGGL library
    ogSize = gunTex->GetSize();
    gunTex->Scale(cg::GetWindowWidth() * 0.05f / std::max(ogSize.x, ogSize.y));
    cg::Vec2f gunSize = gunTex->GetSize();
    cg::Vec2f origin = pos + cg::Vec2f(size,-size) / 3.f;
    
    cg::Vec2f mousePos = cg::GetMousePos();
    cg::Vec2f delta = mousePos - origin;

    // This code is copied from cg::Line()
    cg::Vec2f dir = delta.Normalized();
    cg::Vec2f right = dir.Right() * gunSize.y / 2.f;

    cg::Color color(255,255,255);
    cg::Vec2f a = origin, b = origin + dir * gunSize.x;
    cg::Vertex tri1[3] = {
        cg::Vertex(a + right, color, cg::Vec2f(0,0)),
        cg::Vertex(a - right, color, cg::Vec2f(0,1)),
        cg::Vertex(b + right, color, cg::Vec2f(1,0))
    };

    cg::Vertex tri2[3] = {
        cg::Vertex(b + right, color, cg::Vec2f(1,0)),
        cg::Vertex(b - right, color, cg::Vec2f(1,1)),
        cg::Vertex(a - right, color, cg::Vec2f(0,1))
    };

    cg::UseTexture(*gunTex);
    cg::Triangle(tri1, false);
    cg::Triangle(tri2, false);
    cg::StopUsingTexture();
    gunTex->SetSize(ogSize);
}

void Player::EquipCharacter(CharacterId id)
{
    CosmeticInfo* info = FindCosmetic(id, characterStore);
    character = info;
}

void Player::EquipGun(GunId id)
{
    CosmeticInfo* info = FindCosmetic(id, gunStore);
    gun = info;
}

void Player::Update(){
    constexpr float mass = 1.f;
    const cg::Vec2f force = cg::Vec2f(0.f, -1500.f);
    accel = force / mass; // F = m * a

    pos = pos + velocity * cg::GetDeltaTime() + accel * 0.5f * cg::GetDeltaTime() * cg::GetDeltaTime();
    velocity = velocity + accel * cg::GetDeltaTime();

    constexpr float maxBoost = 500.f;
    if (scene == SceneGame && cg::GetMouseButtonDown(MOUSE_BUTTON_LEFT) && pos.y < cg::GetWindowHeight()){
        cg::Vec2f delta = pos - cg::Vec2f(cg::GetMousePos());
        float intensity = std::min(delta.Length(), maxBoost) * gun->gunPower;

        velocity = delta.Normalized() * intensity * 2.5f;

        cg::Play(explosionSfx);
    }

    // Clamp speed
    // constexpr float maxSpeed = 500.f;
    // if (velocity.Length() > maxSpeed)
    //     velocity = velocity.Normalized() * maxSpeed;

    if (pos.x < 0)
        pos.x += cg::GetWindowWidth();

    if (pos.x > cg::GetWindowWidth())
        pos.x -= cg::GetWindowWidth();

    if (pos.y > cg::GetWindowHeight() && std::abs(velocity.y) > 500.f && velocity.y > 0)
        pos.y = 0;

    if (pos.y < 0) {
        GameOver();
        cg::Play(failSfx);
        velocity = cg::Vec2f(0,0);
    }

    if (scene != SceneDeathScreen) scene = SceneGame;
}

bool loadedCoinTextures = false;

cg::Texture coin1, coin2, coin3, coin4, coin5, coin6;
std::vector<cg::Texture*> coinTextures;

int Randint(int min, int max){
    return min + rand() % (max - min);
}

CosmeticInfo* FindCosmetic(int id, StoreCatalogue& store)
{
    for (int i = 0; i < store.cosmeticsInfo.size(); i++){
        CosmeticInfo* info = &store.cosmeticsInfo[i];
        if (info->id == id) return info;
    }
    return nullptr;
}

void GameOver()
{
    cg::Play(failSfx);
    UpdateSceneChange(SceneDeathScreen, false);
    std::cout << "Game Over!\n";
}

void ScaleTex(cg::Texture *tex, cg::Vec2f targetSize)
{
    cg::Vec2f texSize = tex->GetSize();
    float scale = std::min(targetSize.x, targetSize.y) / std::max(texSize.x, texSize.y);
    tex->Scale(scale);
}

void GenerateCoin()
{
    const static cg::Vec2f margin(cg::GetWindowWidth() * 0.02f, cg::GetWindowHeight() * 0.05f);
    AddCoin(cg::Vec2f(Randint(margin.x, cg::GetWindowWidth() - margin.x), Randint(margin.y, cg::GetWindowHeight() - margin.y)));
}

void UpdateCoins(const Player &player)
{
    std::vector<int> collectedCoins;
    for (int i = 0; i < coins.size(); i++){
        const Coin& coin = coins[i];
        float dst = abs((coin.pos - player.pos).Length());

        if (dst < player.size + COIN_SIZE.x){
            collectedCoins.push_back(i);
        }
    }

    if (collectedCoins.empty()) return;

    cg::Play(coinSfx);

    std::sort(collectedCoins.begin(), collectedCoins.end(), [](int a, int b){
        return a > b;
    });

    for (int i : collectedCoins){
        std::cout << "Collected coin at index " << i << std::endl;
        coins.erase(coins.begin() + i);
        GenerateCoin();

        // The more coins collected, the less score coins give
        constexpr float coinValue = 10.f, coinDegration = 100.f;
        score += coinValue * exp(-coinsCollected / coinDegration);
        coinsCollected++;

        NewBackgroundColor();
    }
}

void DrawCoins(){
    if (!loadedCoinTextures){
        coin2.LoadImage("tex/coins/coin2.png");
        coin1.LoadImage("tex/coins/coin1.png");
        coin3.LoadImage("tex/coins/coin3.png");
        coin4.LoadImage("tex/coins/coin4.png");
        coin5.LoadImage("tex/coins/coin5.png");
        coin6.LoadImage("tex/coins/coin6.png");

        coinTextures = { &coin1, &coin2, &coin3, &coin4, &coin5, &coin6 };
        loadedCoinTextures = true;
    }

    static cg::Clock clock;
    if (!clock.IsTicking()) clock.Start();

    double elapsed = clock.TimeElapsed<cg::Seconds>();

    // A full coin rotation takes a second with this setup
    int phase = int(elapsed * 6) % 6;
    cg::Texture* coinTex = coinTextures[phase];
    for (Coin& coin : coins){
        coinTex->SetOrigin(coin.pos);
        coinTex->SetSize(COIN_SIZE);
        cg::Draw(*coinTex);
    }
}

void LoadHighscoreProfiles()
{
    for (Highscore& score : highscores){
        try
        {
            score.pfp = new cg::Texture(std::string("tex/pfp/" + score.user + ".jpeg").c_str());
            score.pfp->FlipVertically();
            std::cout << "Loaded profile picture for " << score.user << ".\n";
        }
        catch(const std::exception& e)
        {
            std::cout << "Failed to load profile picture for " << score.user << ": " << e.what() << std::endl;
            delete score.pfp;
        }
    }

    // Break line
    std::cout << std::endl;
}

void DeleteHighscoreProfiles(){
    for (Highscore& score : highscores){
        if (score.pfp) {
            score.pfp->Free();
            delete score.pfp;
            std::cout << "Deleting pfp of user " << score.user << "." << std::endl;
        }
    }
    // Break line
    std::cout << std::endl;
}

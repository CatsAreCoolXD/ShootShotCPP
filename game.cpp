#include "game.h"

Player::Player(cg::Vec2f pos, float size) : pos(pos), size(size) {
    sprite.LoadImage("tex/player.png");
    sprite.SetSize(cg::Vec2f(size,size));
    sprite.SetOrigin(pos, true);
    sprite.FlipVertically();

    shotgunTex.LoadImage("tex/shotgun.png");
    shotgunTex.FlipVertically();
    cg::Vec2f ogSize = shotgunTex.GetSize();
    float scale = std::max(ogSize.x, ogSize.y) / (size * 0.75f);
    shotgunTex.Scale(1.f /  scale);
}

void Player::Draw(){
    sprite.SetOrigin(pos, FLAG_CENTERED);

    // Draw 3 sprites for seamless teleportations from one side of the screen to another
    cg::Draw(sprite, FLAG_NO_BORDER);
    cg::Draw(sprite, sprite.GetOrigin() + cg::Vec2f(cg::GetWindowWidth(), 0.f), sprite.GetSize(), FLAG_NO_BORDER);
    cg::Draw(sprite, sprite.GetOrigin() - cg::Vec2f(cg::GetWindowWidth(), 0.f), sprite.GetSize(), FLAG_NO_BORDER);

    // Draw shotgun rotated towards the mouse
    cg::Vec2f shotgunSize = shotgunTex.GetSize();
    cg::Vec2f origin = pos + cg::Vec2f(size,-size) / 3.f;
    shotgunTex.SetOrigin(origin);
    
    cg::Vec2f mousePos = cg::GetMousePos();
    cg::Vec2f delta = mousePos - origin;

    // This code is copied from cg::Line()
    cg::Vec2f dir = delta.Normalized();
    cg::Vec2f right = dir.Right() * shotgunSize.y;

    cg::Color color(255,255,255);
    cg::Vec2f a = origin, b = origin + dir * shotgunSize.y;
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

    cg::UseTexture(shotgunTex);
    cg::Triangle(tri1, false);
    cg::Triangle(tri2, false);
    cg::StopUsingTexture();
}

void Player::DeleteTextures()
{
    sprite.Free();
    shotgunTex.Free();
}

void Player::Update(){
    velocity = velocity + cg::Vec2f(0.f, -30.f) * cg::GetDeltaTime();

    constexpr float maxBoost = 500.f;
    if (scene == Game && cg::GetMouseButtonDown(MOUSE_BUTTON_LEFT)){
        cg::Vec2f delta = pos - cg::Vec2f(cg::GetMousePos());
        float intensity = std::min(delta.Length(), maxBoost);

        velocity = delta.Normalized() * intensity * 2.5f * cg::GetDeltaTime();
    }

    // Clamp speed
    constexpr float maxSpeed = 500.f;
    if (velocity.Length() > maxSpeed)
        velocity = velocity.Normalized() * maxSpeed;

    pos = pos + velocity;

    if (pos.x < 0)
        pos.x += cg::GetWindowWidth();

    if (pos.x > cg::GetWindowWidth())
        pos.x = 0;

    if (pos.y > cg::GetWindowHeight() && std::abs(velocity.y) > 5.f)
        pos.y = 0;

    if (pos.y < 0) {
        GameOver();
        velocity = cg::Vec2f(0,0);
    }

    if (scene != DeathScreen) scene = Game;
}

bool loadedCoinTextures = false;

cg::Texture coin1, coin2, coin3, coin4, coin5, coin6;
std::vector<cg::Texture*> coinTextures;

int Randint(int min, int max){
    return min + rand() % (max - min);
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

    std::sort(collectedCoins.begin(), collectedCoins.end(), [](int a, int b){
        return a > b;
    });

    for (int i : collectedCoins){
        std::cout << "Collected coin at index " << i << std::endl;
        coins.erase(coins.begin() + i);
        GenerateCoin();

        // The more coins collected, the less score coins give
        score += 10 * exp(-coinsCollected / 100.f);
        coinsCollected++;

        NewBackgroundColor();
    }
}

void DrawCoins(){
    if (!loadedCoinTextures){
        coin1.LoadImage("tex/coin1.png");
        coin2.LoadImage("tex/coin2.png");
        coin3.LoadImage("tex/coin3.png");
        coin4.LoadImage("tex/coin4.png");
        coin5.LoadImage("tex/coin5.png");
        coin6.LoadImage("tex/coin6.png");

        coinTextures = { &coin1, &coin2, &coin3, &coin4, &coin5, &coin6 };
        loadedCoinTextures = true;
    }

    // A full coin rotation takes a second with this setup
    int phase = int(cg::GetFrameCount() / (cg::GetAverageFPS() / 6)) % 6;
    cg::Texture* coinTex = coinTextures[phase];
    for (Coin& coin : coins){
        coinTex->SetOrigin(coin.pos);
        coinTex->SetSize(COIN_SIZE);
        cg::Draw(*coinTex);
    }
}
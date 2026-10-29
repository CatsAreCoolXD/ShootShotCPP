#include "game.h"

Player::Player(cg::Vec2f pos, float size) : pos(pos), size(size) {
    sprite.LoadImage("tex/player.png");
    sprite.SetSize(cg::Vec2f(size,size));
    sprite.SetOrigin(pos, true);
    sprite.FlipVertically();

    shotgunTex.LoadImage("tex/shotgun.png");
    cg::Vec2f ogSize = shotgunTex.GetSize();
    float scale = std::max(ogSize.x, ogSize.y) / (size * 0.25f);
    shotgunTex.Scale(scale);
}

void Player::Draw(){
    sprite.SetOrigin(pos);

    // Draw 3 sprites for seamless teleportations from one side of the screen to another
    cg::Draw(sprite, FLAG_NO_BORDER);
    cg::Draw(sprite, sprite.GetOrigin() + cg::Vec2f(cg::GetWindowWidth(), 0.f), sprite.GetSize(), FLAG_NO_BORDER);
    cg::Draw(sprite, sprite.GetOrigin() - cg::Vec2f(cg::GetWindowWidth(), 0.f), sprite.GetSize(), FLAG_NO_BORDER);

    // Todo: Draw rotated shotgun
}

void Player::DeleteTextures()
{
    sprite.Free();
    shotgunTex.Free();
}

void Player::Update(){
    velocity = velocity + cg::Vec2f(0.f, -30.f) * cg::GetDeltaTime();

    if (cg::GetMouseButtonDown(MOUSE_BUTTON_LEFT)){
        cg::Vec2f delta = pos - cg::Vec2f(cg::GetMousePos());
        float intensity = delta.Length();

        velocity = delta * 5.f * cg::GetDeltaTime();
    }

    // Clamp speed
    constexpr float maxSpeed = 20.f;
    if (velocity.Length() > maxSpeed){
        velocity = velocity.Normalized() * maxSpeed;
    }

    pos = pos + velocity;

    if (pos.x < 0)
        pos.x += cg::GetWindowWidth();

    if (pos.x > cg::GetWindowWidth())
        pos.x = 0;

    if (pos.y > cg::GetWindowHeight() && std::abs(velocity.y) > 5.f)
        pos.y = 0;
}

bool loadedCoinTextures = false;

cg::Texture coin1, coin2, coin3, coin4, coin5, coin6;

std::vector<cg::Texture*> coinTextures;

void UpdateCoins(const Player& player){
    for (Coin& coin : coins){

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
    }

    // A full coin rotation takes a second with this setup
    int phase = int(cg::GetFrameCount() / (cg::GetAverageFPS() / 6)) % 6;
    cg::Texture* coinTex = coinTextures[phase];
    for (Coin& coin : coins){
        coinTex->SetOrigin(coin.pos);
        coinTex->SetSize(cg::Vec2f(50,50));
        cg::Draw(*coinTex);
    }
}
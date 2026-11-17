#include "gui.h"
#include "audio.h"

void StoreItem::Draw(float offset, int index, float& money, Player& player)
{
    cg::Vec2f center = cg::Vec2f(cg::GetWindowSize() / 2.f) - cg::Vec2f(offset, 0.f);
    cg::Vec2f containerSize = cg::Vec2f(cg::GetWindowWidth() / 5.f, cg::GetWindowHeight() / 2.f);

    const float spacing = cg::GetWindowWidth() * 0.05f;
    center.x += (containerSize.x + spacing / 2.f) * index;

    cg::RoundedRectangle(center, containerSize, 15, FLAG_CENTERED);

    const float padding = spacing / 4.f;
    const float texSize = containerSize.x - padding * 2.f;

    const float containerTop = center.y + containerSize.y / 2.f;
    const float texCenterY   = containerTop - padding - texSize / 2.f;

    cg::RoundedRectangle(
        cg::Vec2f(center.x, texCenterY),
        cg::Vec2f(texSize, texSize),
        10, FLAG_CENTERED
    );

    cg::Draw(*tex, cg::Vec2f(center.x, texCenterY) - cg::Vec2f(texSize, texSize) / 2.f, cg::Vec2f(texSize, texSize), FLAG_CENTERED);

    cg::Vec2f titleTextPos = cg::Vec2f(center.x, cg::GetWindowHeight() - texCenterY - padding * 2);
    cg::Text(name, titleTextPos, FLAG_CENTERED | FLAG_USE_SECONDARY_COLOR);

    cg::Vec2f buyButtonPos = titleTextPos - cg::Vec2f(0, spacing * 0.85f);
    std::string buttonText = owned ? "Use" : ("Buy for $" + std::to_string(price));
    if (cg::GUIButton(buttonText, buyButtonPos, cg::Vec2f(texSize, spacing), FLAG_CENTERED)){
        if (!owned && money >= price){
            std::cout << "Bought " << name << ".\n";
            owned = true;
            cg::Sound* purchaseSound = (rand() % 2) == 0 ? purchase1 : purchase2;
            cg::Play(purchaseSound, 2.f);

            player.SetSprite(tex);
        } else if (owned)
            player.SetSprite(tex);
    }
}

void LoadCharacterTextures(Player& player, float size)
{
    for (int i = 0; i < Characters::LastItem; i++){
        cg::Texture* tex = new cg::Texture(characterTexturePaths[i].c_str());
        tex->SetSize(cg::Vec2f(size,size));
        tex->FlipVertically();
        characterTextures[i] = tex;

        characterShopItems[i] = StoreItem(tex, 25, characterNames[i]);
    }

    player.SetSprite(characterTextures[0]);
}

void FreeCharacterTextures(){
    for (int i = 0; i < Characters::LastItem; i++){
        delete characterTextures[i];
    }
}

void DrawCharacterStore(float &money, Player &player)
{
    static float offset = 0;
    offset += cg::GetMouseScroll().y * -50;
    for (int i = 0; i < Characters::LastItem; i++){
        characterShopItems[i].Draw(offset, i, money, player);
    }

    if (cg::GUIButton("Return to menu", 
        cg::Vec2f(cg::GetWindowWidth() / 2.f, cg::GetWindowHeight() * 0.9f), 
        cg::Vec2f(cg::GetWindowWidth() / 5.f, cg::GetWindowHeight() * 0.1f), FLAG_CENTERED
    )){
        UpdateSceneChange(MainMenu, false);
    }
}

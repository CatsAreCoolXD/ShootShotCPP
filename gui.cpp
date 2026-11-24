#include "gui.h"
#include "audio.h"

StoreItem::StoreItem(cg::Texture *tex, const CosmeticInfo &info) : 
    tex(tex), price(info.price), name(info.name), description(info.description), rarity(info.rarity) {}

void StoreItem::Draw(float offset, int index, float &money, cg::Texture *&targetTex, cg::Font *descriptionFont)
{
    const float spacing = cg::GetWindowWidth() * 0.025f;
    const float padding = spacing / 4.f;
    const cg::Vec2f descriptionSize = cg::MeasureText(description, descriptionFont);
    const float nameHeight = cg::MeasureText(name).y;
    const float containerWidth = cg::GetWindowWidth() / 5.f;
    const float texSize = containerWidth - spacing; // On both sides there will be spacing / 2.f distance to the edge
    const cg::Vec2f buttonSize(texSize, spacing * 2);
    cg::Vec2f containerSize = cg::Vec2f(cg::GetWindowWidth() / 5.f, 
        spacing + texSize
        + nameHeight + padding 
        + descriptionSize.y + padding 
        + buttonSize.y + spacing / 2.f
    );

    cg::Vec2f center = cg::Vec2f(cg::GetWindowSize() / 2.f) - cg::Vec2f(offset, 0.f);
    center.x += (containerSize.x + spacing / 2.f) * index;

    cg::RoundedRectangle(center, containerSize, 15, FLAG_CENTERED);

    const float containerTop = center.y + containerSize.y / 2.f;
    const float texCenterY = containerTop - spacing / 2.f - texSize / 2.f;

    cg::PushPrimaryColor(rarityColors[rarity]);
    cg::RoundedRectangle(
        cg::Vec2f(center.x, texCenterY),
        cg::Vec2f(texSize, texSize),
        10, FLAG_CENTERED
    );
    cg::PopStyle();

    cg::Draw(*tex, cg::Vec2f(center.x, texCenterY) - cg::Vec2f(texSize, texSize) / 2.f, cg::Vec2f(texSize, texSize));

    cg::Vec2f titleTextPos = cg::Vec2f(center.x, texCenterY - texSize / 2.f - spacing / 2.f - nameHeight / 2.f);
    cg::Vec2f descriptionTextPos = titleTextPos - cg::Vec2f(0, nameHeight / 2.f + padding + descriptionSize.y / 2.f);

    cg::Text(name, titleTextPos, FLAG_CENTERED | FLAG_USE_SECONDARY_COLOR);

    cg::PushFont(*descriptionFont);
    cg::Text(description, descriptionTextPos, FLAG_CENTERED | FLAG_USE_SECONDARY_COLOR);
    cg::PopFont();

    cg::Vec2f buyButtonPos = descriptionTextPos - cg::Vec2f(0, descriptionSize.y / 2.f + padding + buttonSize.y / 2.f);
    std::string buttonText = owned ? (targetTex == tex ? "Equipped" : "Equip") : ("Buy for $" + std::to_string(price));
    cg::PushPrimaryColor(rarityColors[rarity]);
    if (cg::GUIButton(buttonText, buyButtonPos, buttonSize, FLAG_CENTERED)){
        if (!owned && money >= price){
            std::cout << "Bought " << name << ".\n";
            owned = true;
            cg::Sound* purchaseSound = (rand() % 2) == 0 ? purchase1 : purchase2;
            cg::Play(purchaseSound, 2.f);

            targetTex = tex;
        } else if (owned)
            targetTex = tex;
    }
    cg::PopStyle();
}

void LoadStoreCatalogues(Player& player, float size)
{
    characterStore.targetTexture = &player.GetSpriteTexture();
    shotgunStore.targetTexture = &player.GetShotgunTexture();

    for (StoreCatalogue* store : storeCatalogues) {
        // Sort items by price here so I don't have to
        std::sort(store->cosmeticsInfo.begin(), store->cosmeticsInfo.end(), [](const CosmeticInfo& a, const CosmeticInfo& b){
            if (a.rarity == b.rarity) return a.price < b.price;
            return a.rarity < b.rarity;
        });

        for (int i = 0; i < store->nItems; i++){
            const CosmeticInfo& info = store->cosmeticsInfo[i];
            cg::Texture* tex = new cg::Texture(info.texPath.c_str());
            tex->SetSize(cg::Vec2f(size,size));
            tex->FlipVertically();
            store->textures.push_back(tex);

            std::cout << "Loaded texture for store: \"" << info.texPath << "\"\n";

            StoreItem& item = store->storeItems.emplace_back(tex, info);
            if (i > 0) continue;

            // Equip default item
            item.owned = true;
            *store->targetTexture = item.tex;
        }
        
        // Break line
        std::cout << std::endl;
    }
}

void FreeStoreCatalogues(){
    for (StoreCatalogue* store : storeCatalogues) {
        for (int i = 0; i < store->nItems; i++){
            delete store->textures[i];
            std::cout << "Deleted store texture: \"" << store->cosmeticsInfo[i].texPath << "\"\n";
        }

        // Break line
        std::cout << std::endl;
    }
}

void DrawStore(float &money, Player &player, StoreCatalogue* store, cg::Font* descriptionFont)
{
    static float offset = 0;
    offset += cg::GetMouseScroll().y * -50;

    for (int i = 0; i < store->nItems; i++)
        store->storeItems[i].Draw(offset, i, money, *store->targetTexture, descriptionFont);

    const cg::Vec2f backButtonCenter = cg::Vec2f(cg::GetWindowWidth() / 2.f, cg::GetWindowHeight() * 0.9f);
    const cg::Vec2f backButtonSize = cg::Vec2f(cg::GetWindowWidth() / 5.f, cg::GetWindowHeight() * 0.1f);
    if (cg::GUIButton("Return to menu", backButtonCenter, backButtonSize, FLAG_CENTERED))
        UpdateSceneChange(SceneMainMenu, false);
}

void DrawLeaderboard(Highscore playerHighscore, cg::Texture* playerTex)
{
    cg::Vec2f center(cg::GetWindowWidth() * 0.8f, cg::GetWindowHeight() * 0.5f);
    cg::Vec2f size(cg::GetWindowWidth() * 0.2f, cg::GetWindowHeight() * 0.4f);
    cg::Rectangle(center, size, FLAG_CENTERED);

    int playerPosition = 10;
    
    std::vector<Highscore> tempHighscores = highscores;
    tempHighscores.push_back(playerHighscore);
    std::sort(tempHighscores.begin(), tempHighscores.end(), [](Highscore& a, Highscore& b){
        return a.score > b.score;
    });

    for (int i = 0; i < tempHighscores.size(); i++){
        if (tempHighscores[i].isPlayer) {
            playerPosition = i;
            break;
        }
    }

    // Amount of entrys into the leaderboard.
    // If the player is not in the top 10 show the top 10 anyways and the player under it.
    // If the player is in the top 10 just show the top 10.
    bool playerIsOutsideTopTen = playerPosition >= 10;
    const int entrys = playerIsOutsideTopTen ? 10 : 9;

    const float entryHeight = size.y / entrys;
    const float spacing = cg::GetWindowWidth() * 0.01f;

    static cg::Font leaderBoardFont("Fonts/Oswald.ttf", 26);
    cg::PushFont(leaderBoardFont);

    const float pfpSize = entryHeight - spacing / 2.f;

    for (int i = 0; i < entrys + 1; i++){
        std::string user = tempHighscores.size() > i ? tempHighscores[i].user : "";
        std::string score = tempHighscores.size() > i ? std::to_string(tempHighscores[i].score) : "";

        bool isPlayer = tempHighscores[i].isPlayer || (i >= entrys && playerIsOutsideTopTen);
        if (isPlayer) {
            user = "You";
            score = std::to_string(playerHighscore.score);
        }

        user = std::to_string(i + 1) + ". " + user;

        cg::Vec2f pos(center.x - size.x / 2.f, center.y + size.y / 2.f - i * entryHeight);
        cg::Rectangle(pos, cg::Vec2f(size.x, entryHeight)); // Line

        if (tempHighscores.size() > i) {
            cg::Texture* pfp = isPlayer ? playerTex: tempHighscores[i].pfp;
            if (pfp){
                cg::Vec2f ogSize = pfp->GetSize();
                pfp->SetSize(cg::Vec2f(pfpSize, pfpSize));
                pfp->SetOrigin(pos + cg::Vec2f(entryHeight, entryHeight) / 2.f - cg::Vec2f(pfpSize, pfpSize) / 2.f);
                cg::UseTexture(*pfp);
                cg::RoundedRectangle(pfp->GetOrigin(), pfp->GetSize(), 5);
                cg::StopUsingTexture();
                pfp->SetSize(ogSize);
            }
        }

        cg::Text(user, cg::Vec2f(pos.x + pfpSize + spacing / 2.f, pos.y + cg::MeasureText(user).y / 2.f), FLAG_USE_SECONDARY_COLOR);
        cg::Text(score, cg::Vec2f(pos.x + size.x - cg::MeasureText(score).x - spacing / 2.f, pos.y + cg::MeasureText(score).y / 2.f), FLAG_USE_SECONDARY_COLOR);
    }
    cg::PopFont();
}

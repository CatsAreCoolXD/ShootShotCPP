#include <iostream>

#include "game.h"
#include "audio.h"
#include "gui.h"

int main(){
    {
        cg::WindowSettings settings;
        settings.resizable = false;
        settings.antiAliasing = true;
        settings.windowMode = WINDOW_MODE_WINDOWED_BORDERLESS;
        cg::Initialize("Clip Manager", cg::Vec2i(1920, 1080), settings);
        cg::SetFPSLimit(1000);
        cg::SetConfig(CGGL_CONFIG_VSYNC, true);
        cg::SetConfig(CGGL_CONFIG_QUIT_ON_KEY_ESCAPE, true);

        cg::SetBackgroundColor(cg::Color(255,255,255));

        cg::PushPrimaryColor(cg::Color(255, 243, 176));
        cg::PushSecondaryColor(cg::Color(51, 92, 103));

        cg::Font font("Fonts/Roboto.ttf", 50);
        cg::Font smallFont("Fonts/Roboto.ttf", 16);
        cg::Font bigFont("Fonts/Roboto.ttf", 100);
        cg::Font scoreFont("Fonts/ArchivoBlack-Regular.ttf", 200);
        cg::PushFont(font);

        Player player(cg::GetWindowSize() / 2.f, 100);
        LoadStoreCatalogues(player, 100);

        cg::Texture background("tex/background.png");
        background.SetOrigin(cg::Vec2f(0,0));
        background.SetSize(cg::Vec2f(cg::GetWindowSize()));

        srand(time(NULL));
        GenerateCoin();
        GenerateCoin();

        NewBackgroundColor();

        Highscore playerHighscore{"You", 0, true};

        std::string menuText = "ShootShot!";

        int musicIndex = 0;
        cg::Sound* music[2] = {
            music2Sfx,
            musicSfx
        };

        cg::Play(music2Sfx);

        cg::ToggleMute();

        UpdateSceneChange(SceneMainMenu, false);

        LoadHighscoreProfiles();

        StoreCatalogue* targetStoreCatalogue = nullptr;;

        while (cg::WindowIsOpen()){
            cg::NewFrame();

            // Funny bunny
            if (scene == SceneDogCheck){
                cg::PushPrimaryColor(cg::Color(0,0,0));
                cg::Rectangle(cg::Vec2f(0,0), cg::Vec2f(cg::GetWindowSize()), FLAG_NO_BORDER);
                cg::Vec2f size = cg::Vec2f(cg::GetWindowHeight(), cg::GetWindowHeight()) / 4.f;
                cg::Draw(*characterStore.textures[CIDAga], cg::Vec2f(cg::GetWindowSize()) / 2.f - size / 2.f, size);
                cg::Draw();
                cg::PopStyle();
                continue;
            }

            if (!cg::IsPlaying(music[musicIndex])){
                std::cout << "Music number " << musicIndex << " is finished. Playing next song.\n";
                musicIndex = (++musicIndex) % 2;
                cg::Play(music[musicIndex]);
            }

            background.SetTint(backgroundTint);
            cg::Draw(background);

            if (scene != SceneGame && cg::GetMouseButtonDown(MOUSE_BUTTON_LEFT)){
                cg::Play(clickSfx, 10.f);
            }

            if (scene == SceneGame){
                cg::PushFont(scoreFont);
                std::string scoreText = std::to_string(score);
                cg::PushPrimaryColor(cg::Color(128,128,128,128));
                cg::Text(scoreText, cg::Vec2f(cg::GetWindowSize() / 2.f) - cg::MeasureText(scoreText) / 2.f);
                cg::PopStyle();
                cg::PopFont();

                player.Update();
                UpdateCoins(player);
            }

            DrawCoins();
            player.Draw();

            if (scene == SceneStore){
                static float money = 10000; // Todo
                DrawStore(money, targetStoreCatalogue, player, &smallFont);
            }

            if (scene == SceneMainMenu){
                cg::PushFont(bigFont);
                cg::Text(menuText, 
                    cg::Vec2f(cg::GetWindowWidth() / 2.f, cg::GetWindowHeight() * 0.65) - cg::MeasureText(menuText) / 2.f,
                    FLAG_USE_SECONDARY_COLOR
                );

                cg::PopFont();
                cg::Vec2f size(cg::GetWindowWidth() / 5.f, cg::GetWindowHeight() / 10.f);
                cg::Vec2f buttonPos = cg::Vec2f(cg::GetWindowSize() / 2.f) - size / 2.f;
                const float spacing = cg::GetWindowHeight() * 0.05f;
                if (cg::GUIButton("Start", buttonPos, size)) {
                    UpdateSceneChange(SceneGame, false);
                    player.pos = cg::Vec2f(cg::GetWindowSize() / 2.f); 
                }

                buttonPos.y -= size.y + spacing;
                if (cg::GUIButton("Character Store", buttonPos, size)) {
                    targetStoreCatalogue = &characterStore;
                    UpdateSceneChange(SceneStore, false);
                }

                buttonPos.y -= size.y + spacing;
                if (cg::GUIButton("Shotgun Store", buttonPos, size)) {
                    targetStoreCatalogue = &gunStore;
                    UpdateSceneChange(SceneStore, false);
                }

                DrawLeaderboard(playerHighscore, player.GetCurrentCharacter()->tex);
            }

            // Todo: Make a death screen with stuff like revive, unlock a new character etc
            if (scene == SceneDeathScreen){
                playerHighscore.score = std::max(playerHighscore.score, score);
                score = 0;
                UpdateSceneChange(SceneMainMenu, false);
            }

            std::string fpsText = (std::string)"FPS: " + std::to_string(int(cg::GetAverageFPS()));
            cg::Text(fpsText, cg::Vec2f(10, cg::GetWindowHeight() - cg::MeasureText(fpsText).y - 10));

            cg::Draw();
            UpdateSceneChange(SceneDogCheck, true);
        }
        // Break line
        std::cout << std::endl;

        font.Free();
        smallFont.Free();
        bigFont.Free();
        scoreFont.Free();

        FreeStoreCatalogues();

        DeleteHighscoreProfiles();
    }

    // Exit
    cg::Terminate();

    std::cout << "Done, returning exit code 0." << std::endl;

    return 0;
}
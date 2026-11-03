#include <iostream>

#include "game.h"
#include "audio.h"

int main(){
    {
        cg::WindowSettings settings;
        settings.resizable = false;
        settings.antiAliasing = true;
        settings.windowMode = WINDOW_MODE_FULLSCREEN;
        cg::Initialize("Clip Manager", cg::Vec2i(1920, 1080), settings);
        cg::SetFPSLimit(1000);
        cg::SetConfig(CGGL_CONFIG_VSYNC, true);
        cg::SetConfig(CGGL_CONFIG_QUIT_ON_KEY_ESCAPE, true);

        cg::SetBackgroundColor(cg::Color(255,255,255));

        cg::PushPrimaryColor(cg::Color(255, 243, 176));
        cg::PushSecondaryColor(cg::Color(51, 92, 103));

        cg::Font font("Fonts/Roboto.ttf", 50);
        cg::Font bigFont("Fonts/Roboto.ttf",100);
        cg::Font scoreFont("Fonts/ArchivoBlack-Regular.ttf", 200);
        cg::PushFont(font);

        Player player(cg::GetWindowSize() / 2.f, 100);

        cg::Texture background("tex/background.png");
        background.SetOrigin(cg::Vec2f(0,0));
        background.SetSize(cg::Vec2f(cg::GetWindowSize()));

        srand(time(NULL));
        GenerateCoin();
        GenerateCoin();

        NewBackgroundColor();

        std::string menuText = "ShootShot!";

        Audio::Init();

        int musicIndex = 0;
        Audio::Sound* music[2] = {
            music2Sfx,
            musicSfx
        };

        Audio::Play(music2Sfx);

        while (cg::WindowIsOpen()){
            cg::NewFrame();

            if (!Audio::IsPlaying(music[musicIndex])){
                std::cout << "Music number " << musicIndex << " is finished. Playing next song.\n";
                musicIndex = (++musicIndex) % 2;
                Audio::Play(music[musicIndex]);
            }

            background.SetTint(backgroundTint);
            cg::Draw(background);

            if (scene != Game && cg::GetMouseButtonDown(MOUSE_BUTTON_LEFT)){
                Audio::Play(clickSfx, 10.f);
            }

            if (scene == Game || scene == Transition){
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

            if (scene != Game && scene != Transition){
                cg::PushFont(bigFont);
                cg::Text(menuText, 
                    cg::Vec2f(cg::GetWindowWidth() / 2.f, cg::GetWindowHeight() * 0.75) - cg::MeasureText(menuText) / 2.f,
                    FLAG_USE_SECONDARY_COLOR
                );

                cg::PopFont();
                cg::Vec2f size(cg::GetWindowWidth() / 5.f, cg::GetWindowHeight() / 10.f);
                if (cg::GUIButton("Start", cg::Vec2f(cg::GetWindowSize() / 2.f) - size / 2.f, size)) {
                    scene = Transition;
                    player.pos = cg::Vec2f(cg::GetWindowSize() / 2.f); 
                }
            }

            if (scene == DeathScreen){
                
            }

            std::string fpsText = (std::string)"FPS: " + std::to_string(int(cg::GetAverageFPS()));
            cg::Text(fpsText, cg::Vec2f(10, cg::GetWindowHeight() - cg::MeasureText(fpsText).y - 10));

            cg::Draw();
        }

        font.Free();

        player.DeleteTextures();
    }

    // Exit
    Audio::Shutdown();
    cg::Terminate();

    return 0;
}
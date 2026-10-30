#include <iostream>

#include "game.h"

int main(){
    {
        cg::WindowSettings settings;
        settings.resizable = false;
        settings.antiAliasing = true;
        settings.windowMode = WINDOW_MODE_FULLSCREEN;
        cg::Initialize("Clip Manager", cg::Vec2i(1920, 1080), settings);
        cg::SetConfig(CGGL_CONFIG_VSYNC, true);
        cg::SetConfig(CGGL_CONFIG_QUIT_ON_KEY_ESCAPE, true);

        cg::SetBackgroundColor(cg::Color(255,255,255));

        cg::PushPrimaryColor(cg::Color(255, 243, 176));
        cg::PushSecondaryColor(cg::Color(51, 92, 103));

        cg::Font font("Fonts/Roboto.ttf", 50);
        //cg::Font bigFont("/home/dkooistra/Documents/C++/Ettools/Fonts/ArchivoBlack-Regular.ttf",32);
        cg::PushFont(font);

        Player player(cg::GetWindowSize() / 2.f, 100);

        cg::Texture background("tex/background.png");
        background.SetOrigin(cg::Vec2f(0,0));
        background.SetSize(cg::Vec2f(cg::GetWindowSize()));

        GenerateCoin();
        srand(time(NULL));

        while (cg::WindowIsOpen()){
            cg::NewFrame();

            cg::Draw(background);

            if (scene != Game && scene != Transition){
                const cg::Vec2f guiSize(cg::GetWindowWidth() * 0.75, cg::GetWindowHeight() * 0.8);
                cg::GUIStartSection("ShootShot", cg::Vec2f(cg::GetWindowSize() / 2.f) - guiSize / 2.f + cg::Vec2f(0.f, guiSize.y));

                if (cg::GUIButton("Start")) {
                    scene = Transition;
                    player.pos = cg::Vec2f(cg::GetWindowSize() / 2.f); 
                }

                cg::GUIEndSection();
            }

            if (scene == DeathScreen){
                
            } else if (scene == Game || scene == Transition){
                player.Update();
                UpdateCoins(player);
            }

            player.Draw();

            DrawCoins();

            cg::Draw();
        }

        font.Free();

        player.DeleteTextures();
    }

    cg::Terminate();

    return 0;
}
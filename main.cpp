#include <iostream>

#include "game.h"

int main(){
    cg::WindowSettings settings;
    settings.resizable = false;
    settings.antiAliasing = true;
    settings.windowMode = WINDOW_MODE_WINDOWED_BORDERLESS;
    cg::Initialize("Clip Manager", cg::Vec2i(1920, 1080), settings);
    cg::SetConfig(CGGL_CONFIG_VSYNC, true);
    cg::SetConfig(CGGL_CONFIG_QUIT_ON_KEY_ESCAPE, true);

    float windowSize = std::min(cg::GetWindowWidth(), cg::GetWindowHeight());

    cg::SetBackgroundColor(cg::Color(255,255,255));

    cg::PushPrimaryColor(cg::Color(255, 243, 176));
    cg::PushSecondaryColor(cg::Color(51, 92, 103));

    cg::Font font("/home/dkooistra/Documents/C++/Ettools/Fonts/Roboto.ttf", windowSize * 0.05f);
    //cg::Font bigFont("/home/dkooistra/Documents/C++/Ettools/Fonts/ArchivoBlack-Regular.ttf",32);
    cg::PushFont(font);

    Player player(cg::GetWindowSize() / 2.f, 100);

    cg::Texture background("tex/background.png");
    background.SetOrigin(cg::Vec2f(0,0));
    background.SetSize(cg::Vec2f(cg::GetWindowSize()));

    AddCoin(cg::Vec2f(cg::GetWindowSize() / 4.f));

    while (cg::WindowIsOpen()){
        cg::NewFrame();

        cg::Draw(background);

        player.Update();
        player.Draw();

        UpdateCoins(player);
        DrawCoins();

        cg::Draw();
    }

    font.Free();

    player.DeleteTextures();

    cg::Terminate();

    return 0;
}
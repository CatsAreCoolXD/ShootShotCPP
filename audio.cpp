#include "audio.h"

#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

#include <iostream>
#include <algorithm>
#include <vector>

namespace Audio {
    namespace {
        ma_result result;
        ma_engine engine;
        std::vector<Sound*> sounds;

        bool ready = false;
    }

    void Init(){
        result = ma_engine_init(NULL, &engine);
        if (result != MA_SUCCESS) {
            std::cout << "Failed to initiliaze audio!\n";
        }

        ready = true;

        coinSfx = new Sound("sfx/coin.wav");
        explosionSfx = new Sound("sfx/explosion.wav");
        failSfx = new Sound("sfx/fail.wav");
        musicSfx = new Sound("sfx/music.wav");
        music2Sfx = new Sound("sfx/candyland.mp3");
        clickSfx = new Sound("sfx/click.mp3");
    }

    void Play(Sound *sound, float volume){
        ma_sound_set_volume(&sound->sound, volume);
        ma_sound_start(&sound->sound);

        if (std::find(sounds.begin(), sounds.end(), sound) == sounds.end()){
            sounds.push_back(sound);
        }
    }

    void Stop(Sound *sound)
    {
        ma_sound_set_volume(&sound->sound, 0.f);
    }

    bool IsPlaying(Sound *sound)
    {
        if (ma_sound_is_playing(&sound->sound)) return true;
        return !ma_sound_at_end(&sound->sound);
    }

    void Shutdown(){
        for (Sound* sound : sounds){
            ma_sound_uninit(&sound->sound);
            delete sound;
        }

        ma_engine_uninit(&engine);
    }

    Sound::Sound(std::string path)
    {
        if (!ready) return;
        this->path = path;
        result = ma_sound_init_from_file(
            &engine,
            path.c_str(),
            0,
            NULL,
            NULL,
            &sound
        );

        if (result != MA_SUCCESS){
            std::cout << "Couldn't create sound object!\n";
        }
    }
}
#ifndef AUDIO_H
#define AUDIO_H

#include "miniaudio.h"

#include <string>

namespace Audio {
    class Sound {
        public:
            Sound(std::string path);
            Sound() {}
            std::string path;
            ma_sound sound;
            int index;
    };

    void Init();

    // Volume:
    // 0.f = 0%
    // 1.f = 100%
    void Play(Sound* sound, float volume = 1.f);
    void Stop(Sound* sound);

    bool IsPlaying(Sound* sound);

    void Shutdown();
}

inline Audio::Sound* coinSfx = nullptr;
inline Audio::Sound* explosionSfx = nullptr;
inline Audio::Sound* failSfx = nullptr;
inline Audio::Sound* musicSfx = nullptr;
inline Audio::Sound* music2Sfx = nullptr;
inline Audio::Sound* clickSfx = nullptr;

#endif
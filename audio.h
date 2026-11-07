#ifndef AUDIO_H
#define AUDIO_H

#include "CGGL/cggl.h"

inline cg::Sound* coinSfx = new cg::Sound("sfx/coin.wav");
inline cg::Sound* explosionSfx = new cg::Sound("sfx/explosion.wav");
inline cg::Sound* failSfx = new cg::Sound("sfx/fail.wav");
inline cg::Sound* musicSfx = new cg::Sound("sfx/music.wav");
inline cg::Sound* music2Sfx = new cg::Sound("sfx/candyland.mp3");
inline cg::Sound* clickSfx = new cg::Sound("sfx/click.mp3");

#endif
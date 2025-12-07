#pragma once

#include <raylib.h>
#include <string>

#include "config.h"

#define URI_SOUND_SPLAT "splat1.wav"

class World {
    int count;
    Sound splat;
    
    public:
    World() {};
    ~World() = default;
    
    void load();
    void render(int screenWidth, int screenHeight) const;
    void update();
    void unload();
};

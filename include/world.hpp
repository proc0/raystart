#pragma once

#include <raylib.h>
#include <string>

#include "config.h"

#define URI_SOUND_SPLAT "splat1.wav"

class World {
    int count_ = 0;
    Sound splat;
    
    public:
    int screenWidth;
    int screenHeight;
    
    World() {};
    ~World() = default;
    
    int count();
    void load();
    void render() const;
    void update();
    void unload();
};

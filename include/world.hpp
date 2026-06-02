#pragma once

#include <raylib.h>

class World {
    int count_ = 0;
    Sound splat;
    
public:
    World() {};
    ~World() = default;
    
    int count();
    void load();
    void render() const;
    void update();
    void unload();
};

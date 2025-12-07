#include "world.hpp"

int World::count() {
    return count_;
}

void World::load(){
    std::string pathAssets = DIR_ASSETS;
    const char* pathSoundSplat = pathAssets.append("/").append(URI_SOUND_SPLAT).c_str();

    splat = LoadSound(pathSoundSplat);
    count_ = 0;
}

void World::render() const {
    DrawRectangleGradientH(0, 0, screenWidth, screenHeight, BLUE, ORANGE);
}

void World::update(){

    if(IsKeyPressed(KEY_SPACE)){
        count_++;
        PlaySound(splat);
    }
    
    if (IsKeyPressed(KEY_H)){
        if (IsCursorHidden()){
            ShowCursor();
        } else {
            HideCursor();
        }
    }
}

void World::unload(){
    UnloadSound(splat);
}
#include "world.hpp"

void World::load(){
    std::string pathAssets = DIR_ASSETS;
    const char* pathSoundSplat = pathAssets.append("/").append(URI_SOUND_SPLAT).c_str();

    splat = LoadSound(pathSoundSplat);
    count = 0;
}

void World::render(int screenWidth, int screenHeight) const {
    const char* countText = TextFormat("Count: %i", count);
    DrawText(countText, 50, 50, 20, WHITE);
    if (IsCursorHidden()) {
        DrawText("CURSOR HIDDEN", screenWidth/2-100, 60, 20, RED);
    } else {
        DrawText("CURSOR VISIBLE", screenWidth/2-100, 60, 20, GREEN);
    }
}

void World::update(){

    if(IsKeyPressed(KEY_SPACE)){
        count++;
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
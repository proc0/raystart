#include "game.hpp"

#if __EMSCRIPTEN__
EM_JS(int, getWindowWidth, (), {
    return window.document.querySelector('canvas').clientWidth;
});

EM_JS(int, getWindowHeight, (), {
    return window.document.querySelector('canvas').clientHeight;
});
#endif

void Game::resize() {
    #if __EMSCRIPTEN__
        // add padding to fit other web elements 
        int width = getWindowWidth() - WINDOW_PAD;
        int height = getWindowHeight() - WINDOW_PAD;
    #else
        int width = GetScreenWidth();
        int height = GetScreenHeight();
    #endif

    if(screenHeight != height || screenWidth != width){
    #if __EMSCRIPTEN__
        screenWidth = width; 
        screenHeight = height;
        SetWindowSize(screenWidth, screenHeight);
    #else
        screenWidth = width;
        screenHeight = height;
    #endif
    }

    lastResize = std::chrono::steady_clock::now();
    TraceLog(LOG_INFO, "Window resized %dx%d", screenWidth, screenHeight);
}

void Game::update(){
    if(!IsWindowFullscreen() && IsWindowResized()){
        auto now = std::chrono::steady_clock::now();
        if (now - lastResize > RESIZE_COOLDOWN) {
            resize();
        }
    }

    if(IsKeyPressed(KEY_SPACE)){
        count++;
        PlaySound(splat);
    }
    
    if (IsKeyPressed(KEY_H)){
        if (IsCursorHidden())
        {
            ShowCursor();
        }
        else
        {
            HideCursor();
        }
    }
    
}

void Game::render() const {
    BeginDrawing();
        ClearBackground(BLACK);

        DrawRectangleGradientH(0, 0, screenWidth, screenHeight, BLUE, ORANGE);
        
        DrawCircleV(GetMousePosition(), 40, YELLOW);

        const char* countText = TextFormat("Count: %i", count);
        DrawText(countText, 50, 50, 20, WHITE);
        if (IsCursorHidden()) {
            DrawText("CURSOR HIDDEN", screenWidth/2-100, 60, 20, RED);
        } else {
            DrawText("CURSOR VISIBLE", screenWidth/2-100, 60, 20, GREEN);
        }
    EndDrawing();
}

void Game::load(){
    std::string pathAssets = DIR_ASSETS;
    const char* pathSoundSplat = pathAssets.append("/").append(URI_SOUND_SPLAT).c_str();

    splat = LoadSound(pathSoundSplat);
    count = 0;
}

void Game::unload(){
    UnloadSound(splat);
}

void Game::loop(void* self) {
    Game* game = static_cast<Game*>(self);

    game->update();
    game->render();
}

void Game::run() {
    #ifdef __EMSCRIPTEN__
        resize();
        // no target FPS (3rd param) for performance
        emscripten_set_main_loop_arg(loop, this, 0, 1);
    #else
        SetTargetFPS(TARGET_FPS);
        while (!WindowShouldClose()) {
            loop(this);
        }
    #endif
}

#pragma once

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif
#include <string>
#include <chrono>
#include <raylib.h>

#include "config.h"

// settings
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define TARGET_FPS 120

static inline constexpr auto RESIZE_COOLDOWN = std::chrono::milliseconds(300);

#ifdef __EMSCRIPTEN__
#define WINDOW_PAD 20
#endif
// assets
#define URI_SOUND_SPLAT "splat1.wav"

class Game {
    std::chrono::steady_clock::time_point lastResize = std::chrono::steady_clock::now();
    int count;
    Sound splat;
    
    public:
    int screenWidth = SCREEN_WIDTH;
    int screenHeight = SCREEN_HEIGHT;
    
    ~Game() = default;
    
    void load();
    static void loop(void* self);
    void render() const;
    void resize();
    void run();
    void unload();
    void update();
};

#pragma once

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif
#include <chrono>
#include <raylib.h>

#include "config.h"
#include "world.hpp"

// initial settings
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define TARGET_FPS 120

static inline constexpr auto RESIZE_COOLDOWN = std::chrono::milliseconds(300);

#ifdef __EMSCRIPTEN__
#define WINDOW_PAD 20
#endif

class Game {
    World world;

    std::chrono::steady_clock::time_point lastResize = std::chrono::steady_clock::now();
    enum State {
        BEGIN,
        START,
        PLAY,
        READY,
        PAUSE,
        OVER,
        WIN,
        END
    };
    State state = BEGIN;
    
    public:
    int screenWidth = SCREEN_WIDTH;
    int screenHeight = SCREEN_HEIGHT;
    
    Game() {};
    ~Game() = default;

    const bool isRunning() const;
    void load();
    static void loop(void* self);
    void render() const;
    void resize();
    void run();
    void unload();
    void update();
};

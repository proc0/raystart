#include "app.hpp"

#include "config.h"
#include "types.hpp"

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#include <emscripten/html5.h>
#endif
#include <raylib.h>

void App::load() {
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE);

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, PROJECT_NAME);
    InitAudioDevice();
    
    SetExitKey(KEY_NULL);

	screen.load();
	game.load();
	world.load();

    screen.addListener(&world);
}

void App::render() const {
	BeginDrawing();
		world.render();
		game.render();
	EndDrawing();
}

void App::run(void* self) {
    App* app = static_cast<App*>(self);

#ifdef __EMSCRIPTEN__
    if (app->state == State::App::END) return;
#endif

    app->update();
    app->render();

// #if DEBUG
//     app->debugRender();
// #endif
}

void App::start() {
// logo update and render
// InputEvent inputEvent = input.update();
// auto endIntroTime = std::chrono::steady_clock::now();
// while (!WindowShouldClose() && inputEvent.type == Event::Input::IDLE && endIntroTime - introStartTime < introCooldown) {
//     inputEvent = input.update();
//     endIntroTime = std::chrono::steady_clock::now();
//     introRender();
// }
	state = State::App::RUN;
	
#ifdef __EMSCRIPTEN__
    // no target FPS (3rd param) for web performance
    emscripten_set_main_loop_arg(run, this, 0, 1);
    emscripten_set_beforeunload_callback(this, unload);
#else
    SetTargetFPS(TARGET_FPS);
    while (!WindowShouldClose() && state == State::App::RUN) {
        run(this);
    }
#endif
}

void App::update() {
    InputEvent inputEvent = input.update();

    // window resizing
    screen.update(inputEvent);
    // if (resized) {
    //     // world.resize();
    //     // menu->resize();
    //     // display.resize();
    //     // game->resize();

    //     // if(state == State::App::PAUSE || state == State::App::START) {
    //     //     world.update(game->status(), control.status(), display.status());
    //     // }
    // }

	game.update();
	world.update();

	// if (WindowShouldClose()) {
	// 	state = State::App::END;
	// }
}

const char* App::unload(int eventType, const void *reserved, void *self) {
    App* app = static_cast<App*>(self);

	app->world.unload();
	app->game.unload();

    CloseAudioDevice();
    CloseWindow();

    delete app;

    return nullptr;
}

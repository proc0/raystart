#include "app.hpp"

#include "config.h"
#include "types.hpp"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif
#include <raylib.h>

void App::load() {
	screen.load();
	game.load();
	world.load();
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
#else
    SetTargetFPS(TARGET_FPS);
    while (state == State::App::RUN) {
        run(this);
    }
#endif
}

void App::update() {
    InputEvent inputEvent = input.update();

    // window resizing
    bool resized = screen.update(inputEvent);
    if (resized) {
        // world.resize();
        // menu->resize();
        // display.resize();
        // game->resize();

        // if(state == State::App::PAUSE || state == State::App::START) {
        //     world.update(game->status(), control.status(), display.status());
        // }
    }

	game.update();
	world.update();

	if (WindowShouldClose()) {
		state = State::App::END;
	}
}

void App::unload() {
	world.unload();
	game.unload();
}

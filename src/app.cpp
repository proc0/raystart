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

    screen.listen(&world);
}

void App::render() const {
	BeginDrawing();
		world.render();
		game.render();
	EndDrawing();
}

void App::run(void* self) {
    App* app = static_cast<App*>(self);

    app->update();
    app->render();
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
    // no target FPS (3rd param) to allow browser to optimize frame rate
    // set simulate infinite loop (4th param) to 0 to let the rest of the function execute (on Web)
    emscripten_set_main_loop_arg(run, this, 0, 0);
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

    screen.update(inputEvent);

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

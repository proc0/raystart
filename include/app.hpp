#pragma once

#include "types.hpp"
#include "game.hpp"

class App {
	Game game;
	
	State::App state = State::App::NIL;

public:
	App() {};
    ~App() = default;

    void load();
    void render() const;
    void run(void* self);
    void start();
    void update();
    void unload();
};

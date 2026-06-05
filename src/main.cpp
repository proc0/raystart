#include "app.hpp"

// #include "config.h"
#include <raylib.h>

int main(void){
    App* app = new App();
    
    app->load();
    app->start();
#ifndef __EMSCRIPTEN__
    app->unload(0, (const void*)nullptr, app);
#endif

    // CloseAudioDevice();
    // CloseWindow();

    return 0;
}
#include "app.hpp"

#include "config.h"
#include <raylib.h>

int main(void){
    App app;

    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE);

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, PROJECT_NAME);
    InitAudioDevice();
    
    SetExitKey(KEY_NULL);
    
    app.load();
    app.start();
    app.unload();

    CloseAudioDevice();
    CloseWindow();

    return 0;
}
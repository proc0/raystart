#include "screen.hpp"

#include "defaults.hpp"
#include "types.hpp"

#include <raylib.h>
#include <raymath.h>

void Screen::load() {
    // must initialize the screen size
    resize();
}

// float Screen::adapt(float value) const {
//     return value * unit;
// }

// Vector2 Screen::adapt(b2Vec2 area) const {
//     return {
//         area.x * unit,
//         area.y * unit
//     };
// }

Rectangle Screen::center(Rectangle area) const {
    return {
        x/2.0f - area.width/2.0f,
        y/2.0f - area.height/2.0f,
        area.width,
        area.height
    };
}

Rectangle Screen::center(Rectangle area, Vector2 offset) const {
    return {
        x/2.0f - area.width/2.0f + offset.x,
        y/2.0f - area.height/2.0f + offset.y,
        area.width,
        area.height
    };
}

// float Screen::convert(float value) const {
//     return value * unit/SCREEN_UNIT;
// }

// Vector2 Screen::convert(b2Vec2 area) const {
//     return {
//         area.x * unit/SCREEN_UNIT,
//         area.y * unit/SCREEN_UNIT
//     };
// }

// Rectangle Screen::project(Rectangle area) const {
//     return {
//         halfX + area.x * unit,
//         halfY - area.y * unit,
//         adapt(area.width),
//         adapt(area.height)
//     };
// }

// Vector2 Screen::project(b2Vec2 point) const {
//     return {
//         halfX + point.x * unit,
//         halfY - point.y * unit
//     };
// }

// b2Vec2 Screen::inject(Vector2 point) const {
//     return {
//         ((point.x  - camera.offset.x - halfX) / unit),
//         ((halfY - point.y + camera.offset.y) / unit)
//     };
// }

bool Screen::update(InputEvent input) {
    fps = GetFPS();

    if(IsWindowResized()){
        auto timeResize = std::chrono::steady_clock::now();
        if (timeResize - timeLastResize > timeResizeRate) {
            resize();
        }
    }

    // zooming and panning
    Vector2 mouseWheel = GetMouseWheelMoveV();
    
    if (mouseWheel.y > 0) {
        zoom(true);
    } else if (mouseWheel.y < 0) {
        zoom(false);
    } else if (IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE)) { 
        isTracking = true;
    }

    if (isToggleTracking && input.type == Event::Input::PRIMARY) {
        isTracking = true;
    }

    if (isTracking) {
        track(input);
    }

    // unified flag processing for both internal 
    // method calling and external method calling
    // resize functions
    if (hasResized) {
        // reset flag
        hasResized = false;
        return true;
    }

    return false;
}

#if __EMSCRIPTEN__
EM_JS(int, getWindowWidth, (), {
    return window.document.querySelector('canvas').clientWidth;
});

EM_JS(int, getWindowHeight, (), {
    return window.document.querySelector('canvas').clientHeight;
});
#endif

void Screen::resize() {
    #if __EMSCRIPTEN__
        int newWidth = getWindowWidth();
        int newHeight = getWindowHeight();
    #else
        int newWidth = GetScreenWidth();
        int newHeight = GetScreenHeight();
    #endif

    // calculate ratio based on screen diagonal
    ratio = ROUND4(sqrtf(powf(newWidth, 2.0f) + powf(newHeight, 2.0f))/unitRatio);
    x = newWidth;
    y = newHeight;
    halfX = newWidth/2;
    halfY = newHeight/2;
    unit = SCREEN_UNIT*ratio + zoomUnit*ratio;
    timeLastResize = std::chrono::steady_clock::now();

    hasResized = true;
    TraceLog(LOG_INFO, "SCREEN resized %ix%i - UNIT: %f", newWidth, newHeight, unit);
}

float Screen::scale(float value) const {
    return value*ratio;
}

Rectangle Screen::scale(Rectangle area) const {
    // float ratio = x/static_cast<float>(SCREEN_WIDTH);
    return {
        area.x * ratio,
        area.y * ratio,
        area.width * ratio,
        area.height * ratio,
    };
}

Vector2 Screen::scale(Vector2 point) const {
    // float ratio = x/static_cast<float>(SCREEN_WIDTH);
    return {
        point.x * ratio,
        point.y * ratio
    };
}

void Screen::toggleTrack() {
    isToggleTracking = true;
}

void Screen::track(InputEvent input) {
    // TODO: move middle mouse button to Input, refactor
    bool beginTrack = isToggleTracking ? input.type == Event::Input::PRIMARY : IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE);
    bool continueTrack = isToggleTracking ? input.type == Event::Input::PRIMARY_DOWN : IsMouseButtonDown(MOUSE_BUTTON_MIDDLE);
    bool endTrack = isToggleTracking ? input.type == Event::Input::PRIMARY_UP : IsMouseButtonReleased(MOUSE_BUTTON_MIDDLE);

    if (beginTrack) {
        Vector2 origin = { static_cast<float>(halfX) + camera.offset.x, static_cast<float>(halfY) + camera.offset.y };
        originDelta = Vector2Subtract(origin, input.position);
    } else if (continueTrack) {
        camera.offset.x = input.position.x - static_cast<float>(halfX) + originDelta.x;
        camera.offset.y = input.position.y - static_cast<float>(halfY) + originDelta.y;
        offset.x = camera.offset.x;
        offset.y = camera.offset.y;
    } else if (endTrack) {
        originDelta = { 0.0f, 0.0f };
        // stop tracking
        isTracking = false;
        isToggleTracking = false;
        isDoneTracking = true;
    }
}

bool Screen::tracking() const {
    return isToggleTracking || isTracking;
}

bool Screen::doneTracking() {
    if (isDoneTracking) {
        isDoneTracking = false;
        return true;
    }
    return false;
}

void Screen::zoom(bool increase) {
    float amount = 0.1f;
    if (increase) {
        unit += amount*ratio;
        zoomUnit += amount;
    } else {
        unit -= amount*ratio;
        zoomUnit -= amount;
    }
    hasResized = true;
}

int Screen::width() const {
    return x;
}

int Screen::height() const {
    return y;
}

int Screen::halfWidth() const {
    return halfX;
}

int Screen::halfHeight() const {
    return halfY;
}
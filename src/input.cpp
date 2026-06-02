#include "input.hpp"
#include "types.hpp"

#include <raylib.h>

InputEvent Input::update() {

	Vector2 mousePosition = GetMousePosition();

    lastGesture = currentGesture;
    currentGesture = GetGestureDetected();
    Vector2 touchPosition = GetTouchPosition(0);

    Vector2 position = currentGesture != GESTURE_NONE ? touchPosition : mousePosition;
    // Vector2 position = GetMousePosition();

	if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || currentGesture == GESTURE_TAP) {
		return {
			.type = Event::Input::PRIMARY,
			.position = position,
		};
	} else if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) || currentGesture == GESTURE_HOLD || currentGesture == GESTURE_DRAG) {
		return {
			.type = Event::Input::PRIMARY_DOWN,
			.position = position,
		};
	} else if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) || (currentGesture == GESTURE_NONE && lastGesture != GESTURE_NONE)) {
		return {
			.type = Event::Input::PRIMARY_UP,
			.position = position,
		};
	} else if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) || currentGesture == GESTURE_DOUBLETAP) {
		return {
			.type = Event::Input::SECONDARY,
			.position = position,
		};
	} else if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
		return {
			.type = Event::Input::SECONDARY_DOWN,
			.position = position,
		};
	} else if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT)) {
		return {
			.type = Event::Input::SECONDARY_UP,
			.position = position,
		};
	} 

	// if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
	// 	return {
	// 		.type = Event::Input::PRIMARY,
	// 		.position = position,
	// 	};
	// } else if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
	// 	return {
	// 		.type = Event::Input::PRIMARY_DOWN,
	// 		.position = position,
	// 	};
	// } else if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
	// 	return {
	// 		.type = Event::Input::PRIMARY_UP,
	// 		.position = position,
	// 	};
	// } else if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
	// 	return {
	// 		.type = Event::Input::SECONDARY,
	// 		.position = position,
	// 	};
	// } else if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
	// 	return {
	// 		.type = Event::Input::SECONDARY_DOWN,
	// 		.position = position,
	// 	};
	// } else if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT)) {
	// 	return {
	// 		.type = Event::Input::SECONDARY_UP,
	// 		.position = position,
	// 	};
	// } 


	if (IsKeyPressed(KEY_ESCAPE)) { 
		return {
			.type = Event::Input::KEY_ESCAPE,
			.position = position,
		};
	} else if (GetKeyPressed() != 0) {
		return {
			.type = Event::Input::KEY_OTHER,
			.position = position,
		};
	}

	return {
		.type = Event::Input::IDLE,
		.position = position
	};
}
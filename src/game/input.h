#pragma once
#include "engine/inputManager.h"

enum EGameInput
{
	GAME_INPUT_UP,
	GAME_INPUT_DOWN,
	GAME_INPUT_LEFT,
	GAME_INPUT_RIGHT,
	GAME_INPUT_LSHIFT,
	GAME_INPUT_Z,
	GAME_INPUT_X,
	GAME_INPUT_ENTER,
	GAME_INPUT_ESCAPE
};

inline bool isPressed(EGameInput vInput)
{
	switch (vInput)
	{
	case GAME_INPUT_UP:			return DxEngine::IS_KEY_PRESSED(KEY_INPUT_UP) || DxEngine::IS_JOYPAD_BUTTON_PRESSED(XINPUT_BUTTON_DPAD_UP);
	case GAME_INPUT_DOWN:		return DxEngine::IS_KEY_PRESSED(KEY_INPUT_DOWN) || DxEngine::IS_JOYPAD_BUTTON_PRESSED(XINPUT_BUTTON_DPAD_DOWN);
	case GAME_INPUT_LEFT:		return DxEngine::IS_KEY_PRESSED(KEY_INPUT_LEFT) || DxEngine::IS_JOYPAD_BUTTON_PRESSED(XINPUT_BUTTON_DPAD_LEFT);
	case GAME_INPUT_RIGHT:		return DxEngine::IS_KEY_PRESSED(KEY_INPUT_RIGHT) || DxEngine::IS_JOYPAD_BUTTON_PRESSED(XINPUT_BUTTON_DPAD_RIGHT);
	case GAME_INPUT_LSHIFT:		return DxEngine::IS_KEY_PRESSED(KEY_INPUT_LSHIFT) || DxEngine::IS_JOYPAD_BUTTON_PRESSED(XINPUT_BUTTON_LEFT_SHOULDER);
	case GAME_INPUT_Z:			return DxEngine::IS_KEY_PRESSED(KEY_INPUT_Z) || DxEngine::IS_JOYPAD_BUTTON_PRESSED(XINPUT_BUTTON_A);
	case GAME_INPUT_X:			return DxEngine::IS_KEY_PRESSED(KEY_INPUT_X) || DxEngine::IS_JOYPAD_BUTTON_PRESSED(XINPUT_BUTTON_X);
	case GAME_INPUT_ENTER:		return DxEngine::IS_KEY_PRESSED(KEY_INPUT_RETURN) || DxEngine::IS_JOYPAD_BUTTON_PRESSED(XINPUT_BUTTON_START);
	case GAME_INPUT_ESCAPE:		return DxEngine::IS_KEY_PRESSED(KEY_INPUT_ESCAPE) || DxEngine::IS_JOYPAD_BUTTON_PRESSED(XINPUT_BUTTON_BACK);
	default:					_ASSERTE(false);
	}
}

inline bool checkHit(EGameInput vInput)
{
	switch (vInput)
	{
	case GAME_INPUT_UP:			return DxEngine::CHECK_HIT_KEY(KEY_INPUT_UP) || DxEngine::CHECK_HIT_JOYPAD_BUTTON(XINPUT_BUTTON_DPAD_UP);
	case GAME_INPUT_DOWN:		return DxEngine::CHECK_HIT_KEY(KEY_INPUT_DOWN) || DxEngine::CHECK_HIT_JOYPAD_BUTTON(XINPUT_BUTTON_DPAD_DOWN);
	case GAME_INPUT_LEFT:		return DxEngine::CHECK_HIT_KEY(KEY_INPUT_LEFT) || DxEngine::CHECK_HIT_JOYPAD_BUTTON(XINPUT_BUTTON_DPAD_LEFT);
	case GAME_INPUT_RIGHT:		return DxEngine::CHECK_HIT_KEY(KEY_INPUT_RIGHT) || DxEngine::CHECK_HIT_JOYPAD_BUTTON(XINPUT_BUTTON_DPAD_RIGHT);
	case GAME_INPUT_LSHIFT:		return DxEngine::CHECK_HIT_KEY(KEY_INPUT_LSHIFT) || DxEngine::CHECK_HIT_JOYPAD_BUTTON(XINPUT_BUTTON_LEFT_SHOULDER);
	case GAME_INPUT_Z:			return DxEngine::CHECK_HIT_KEY(KEY_INPUT_Z) || DxEngine::CHECK_HIT_JOYPAD_BUTTON(XINPUT_BUTTON_A);
	case GAME_INPUT_X:			return DxEngine::CHECK_HIT_KEY(KEY_INPUT_X) || DxEngine::CHECK_HIT_JOYPAD_BUTTON(XINPUT_BUTTON_X);
	case GAME_INPUT_ENTER:		return DxEngine::CHECK_HIT_KEY(KEY_INPUT_RETURN) || DxEngine::CHECK_HIT_JOYPAD_BUTTON(XINPUT_BUTTON_START);
	case GAME_INPUT_ESCAPE:		return DxEngine::CHECK_HIT_KEY(KEY_INPUT_ESCAPE) || DxEngine::CHECK_HIT_JOYPAD_BUTTON(XINPUT_BUTTON_BACK);
	default:					_ASSERTE(false);
	}
}
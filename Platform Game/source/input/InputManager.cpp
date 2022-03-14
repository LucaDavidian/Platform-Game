#include "InputManager.h"
#include "../GraphicsManager.h"
#include "../event system/Event.hpp"
#include <SDL2/SDL.h>

void InputManager::AddKeyBinding(const std::string &actionName, const Vector<KeyInfo> &keyChord)
{
	mKeyBindings.insert(std::make_pair(actionName, keyChord));
}

bool InputManager::IsActionMapped(const std::string &actionName) const
{
	return mKeyBindings.find(actionName) != mKeyBindings.end();
}

bool InputManager::CheckAction(const std::string &actionName) const
{
	if (auto &&keyBindingPair = mKeyBindings.find(actionName); keyBindingPair != mKeyBindings.end())
		return keyBindingPair->second.Check();

	return false;
}

void InputManager::PollKeyboardState()
{
	// cache pointer to internal SDL keyboard state array
	static const uint8_t *keyboardState = SDL_GetKeyboardState(NULL);  

	// update current keyboard state
	for (unsigned int key = 0; key < NUM_KEYS; key++)
	{
		// save previous key state
		mPreviousKeys[key] = mCurrentKeys[key];

		// update current key state
		mCurrentKeys[key] = keyboardState[key];
	}
}

InputManager::KeyState InputManager::GetKeyState(unsigned int scanCode) const
{
	if (mPreviousKeys[scanCode] == 1)   // key was pressed during previous frame
	{
		if (mCurrentKeys[scanCode] == 1)
			return KeyState::PRESSED;
		else
			return KeyState::JUST_RELEASED;
	}
	else   // key wasn't pressed during the previous frame
	{
		if (mCurrentKeys[scanCode] == 1)
			return KeyState::JUST_PRESSED;
		else
			return KeyState::RELEASED;
	}
}

void InputManager::PollMouseState()
{
	// save previous mouse position relative to window
	mMousePreviousXPosition = mMouseCurrentXPosition;                        
	mMousePreviousYPosition = mMouseCurrentYPosition;
	
	// get current mouse position in screen coordinates (SDL origin is top left)
	uint32_t mouseState = SDL_GetMouseState(&mMouseCurrentXPosition, &mMouseCurrentYPosition);            
	mMouseCurrentYPosition = GraphicsManager::GetInstance().GetViewportHeight() - mMouseCurrentYPosition;   // reverse mouse input y-axis (SDL origin is top left, OpenGL and engine use bottom left)

	// save previous mouse button state
	mMousePreviousLeftButton = mMouseCurrentLeftButton;                      
	mMousePreviousRightButton = mMouseCurrentRightButton;
	mMousePreviousMiddleButton = mMouseCurrentMiddleButton;
	
	// get current mouse button state
	mMouseCurrentLeftButton = mouseState & SDL_BUTTON(SDL_BUTTON_LEFT);      
	mMouseCurrentRightButton = mouseState & SDL_BUTTON(SDL_BUTTON_RIGHT);
	mMouseCurrentMiddleButton = mouseState & SDL_BUTTON(SDL_BUTTON_MIDDLE);
}

InputManager::KeyState InputManager::GetMouseButtonState(MouseButton button)
{
	switch (button)
	{
	case MouseButton::LEFT:
		if (mMousePreviousLeftButton)
			if (mMouseCurrentLeftButton)
				return KeyState::PRESSED;
			else
				return KeyState::JUST_RELEASED;
		else
			if (mMouseCurrentLeftButton)
				return KeyState::JUST_PRESSED;
			else
				return KeyState::RELEASED;
	case MouseButton::MIDDLE:
		if (mMousePreviousMiddleButton)
			if (mMouseCurrentLeftButton)
				return KeyState::PRESSED;
			else
				return KeyState::JUST_RELEASED;
		else
			if (mMouseCurrentMiddleButton)
				return KeyState::JUST_PRESSED;
			else
				return KeyState::RELEASED;
	case MouseButton::RIGHT:
		if (mMousePreviousRightButton)
			if (mMouseCurrentLeftButton)
				return KeyState::PRESSED;
			else
				return KeyState::JUST_RELEASED;
		else
			if (mMouseCurrentRightButton)
				return KeyState::JUST_PRESSED;
			else
				return KeyState::RELEASED;
	}
}

int InputManager::GetMouseDeltaXPosition() const
{
	return mMouseCurrentXPosition - mMousePreviousXPosition;
}

int InputManager::GetMouseDeltaYPosition() const
{
	return mMouseCurrentYPosition - mMousePreviousYPosition;
}

int InputManager::GetMouseAbsoluteXPosition() const
{
	return mMouseCurrentXPosition;
}

int InputManager::GetMouseAbsoluteYPosition() const
{
	return mMouseCurrentYPosition;
}

void InputManager::PollInput()
{
	// poll keyboard state
	PollKeyboardState();

	// poll mouse state
	PollMouseState();
}

void InputManager::DispatchInput()
{
	// get state of input devices
	PollInput();

	// dispatch keyboard events
	for (unsigned int scanCode = 0; scanCode < NUM_KEYS; scanCode++)
		if (GetKeyState(scanCode) == KeyState::JUST_PRESSED)
		{
			mKeyPressEvent(SDL_GetKeyFromScancode((SDL_Scancode)scanCode));

			InputEvent inputEvent;
			inputEvent.mType = InputEvent::Type::KEY_PRESS;
			inputEvent.keyPressEvent.mKey = SDL_GetKeyFromScancode((SDL_Scancode)scanCode);
			mInputEvent(inputEvent);
		}
		else if (GetKeyState(scanCode) == KeyState::JUST_RELEASED)
		{
			mKeyReleaseEvent(SDL_GetKeyFromScancode((SDL_Scancode)scanCode));

			InputEvent inputEvent;
			inputEvent.mType = InputEvent::Type::KEY_RELEASE;
			inputEvent.keyReleaseEvent.mKey = SDL_GetKeyFromScancode((SDL_Scancode)scanCode);
			mInputEvent(inputEvent);
		}
		else if (GetKeyState(scanCode) == KeyState::PRESSED)
		{
			mKeyDownEvent(SDL_GetKeyFromScancode((SDL_Scancode)scanCode));

			InputEvent inputEvent;
			inputEvent.mType = InputEvent::Type::KEY_DOWN;
			inputEvent.keyDownEvent.mKey = SDL_GetKeyFromScancode((SDL_Scancode)scanCode);
			mInputEvent(inputEvent);
		}

	// dispatch mouse events
	if (GetMouseDeltaXPosition() || GetMouseDeltaYPosition())
	{
		mMouseMoveEvent(mMouseCurrentXPosition, mMouseCurrentYPosition);

		InputEvent inputEvent;
		inputEvent.mType = InputEvent::Type::MOUSE_MOVE;
		inputEvent.mouseMoveEvent.x = mMouseCurrentXPosition;
		inputEvent.mouseMoveEvent.y = mMouseCurrentYPosition;	
		mInputEvent(inputEvent);
	}

	if (GetMouseButtonState(MouseButton::LEFT) == KeyState::JUST_PRESSED)
	{
		mMousePressEvent([](bool b) { return b; }, mMouseCurrentXPosition, mMouseCurrentYPosition);  // mouse click is consumed by first event handler returning true

		InputEvent inputEvent;
		inputEvent.mType = InputEvent::Type::MOUSE_PRESS;
		inputEvent.mousePressEvent.x = mMouseCurrentXPosition;
		inputEvent.mousePressEvent.y = mMouseCurrentYPosition;
		mInputEvent(inputEvent);
	}

	if (GetMouseButtonState(MouseButton::LEFT) == KeyState::JUST_RELEASED)
	{
		mMouseReleaseEvent([](bool b) { return b; }, mMouseCurrentXPosition, mMouseCurrentYPosition);

		InputEvent inputEvent;
		inputEvent.mType = InputEvent::Type::MOUSE_RELEASE;
		inputEvent.mouseReleaseEvent.x = mMouseCurrentXPosition;
		inputEvent.mouseReleaseEvent.y = mMouseCurrentYPosition;
		mInputEvent(inputEvent);
	}

	if (GetMouseButtonState(MouseButton::LEFT) == KeyState::PRESSED)
	{
		mMouseDownEvent(mMouseCurrentXPosition, mMouseCurrentYPosition);

		InputEvent inputEvent;
		inputEvent.mType = InputEvent::Type::MOUSE_DOWN;
		inputEvent.mouseDownEvent.x = mMouseCurrentXPosition;
		inputEvent.mouseDownEvent.y = mMouseCurrentYPosition;
		mInputEvent(inputEvent);
	}
}


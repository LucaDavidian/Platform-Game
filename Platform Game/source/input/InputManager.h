#ifndef INPUT_SYSTEM_H
#define INPUT_SYSTEM_H

#include "../delegates/signal.hpp"
#include "../data structures/vector.hpp"
#include <string>
#include <map>

struct InputEvent;

class InputManager
{
private:
	SIGNAL_ONE_PARAM(KeyboardSignal, unsigned int);  // emitted when a keyboard input event occurs
	KeyboardSignal mKeyPressEvent;
	KeyboardSignal mKeyReleaseEvent;
	KeyboardSignal mKeyDownEvent;
	KeyboardSignal mKeyUpEvent;

	SIGNAL_ONE_PARAM(AxisSignal, float);  // emitted every frame
	AxisSignal mAxisSignal;

	SIGNAL_TWO_PARAM(MouseSignal, int, int); // emitted when a mouse input event occurs
	MouseSignal mMouseMoveEvent;
	MouseSignal mMouseDownEvent;
	MouseSignal mMouseUpEvent;

	SIGNAL_RET_TWO_PARAM(MouseSignalR, bool, int, int);
	MouseSignalR mMousePressEvent;
	MouseSignalR mMouseReleaseEvent;

	SIGNAL_ONE_PARAM(InputSignal, InputEvent &);  // emitted when any input event occurs
	InputSignal mInputEvent;

public:
	enum class KeyState { JUST_PRESSED, PRESSED, JUST_RELEASED, RELEASED, MAX };
	enum class MouseButton { LEFT, MIDDLE, RIGHT };

	struct KeyInfo
	{
		unsigned int mKey;
		KeyState mKeyState;
	};

private:
	class KeyChord
	{
	public:
		KeyChord(Vector<KeyInfo> const &keyChord) : mKeyChord(keyChord) {}

		bool Check() const
		{
			for (auto &&keyInfo : mKeyChord)
				if (InputManager::GetInstance().GetKeyState(keyInfo.mKey) != keyInfo.mKeyState)
					return false;

			return true;
		}

	private:
		Vector<KeyInfo> mKeyChord;
	};

public:
	void AddKeyBinding(const std::string &actionName, const Vector<KeyInfo> &keyChord);
	bool IsActionMapped(const std::string &actionName) const;
	bool CheckAction(const std::string &actionName) const;   // used to poll for actions

	template <typename T, typename PtrToMemFun>
	Connection RegisterKeyEvent(int key, KeyState keyState, T &instance, PtrToMemFun ptrToMemFun, unsigned priority = -1)
	{
		switch (keyState)
		{
			case KeyState::JUST_PRESSED:
				return RegisterKeyPressEvent(key, instance, ptrToMemFun, priority);
				break;
			case KeyState::PRESSED:
				return RegisterKeyDownEvent(key, instance, ptrToMemFun, priority);
				break;
			case KeyState::JUST_RELEASED:
				return RegisterKeyReleasedEvent(key, instance, ptrToMemFun, priority);
				break;
			case KeyState::RELEASED:
				return RegisterKeyUpEvent(key, instance, ptrToMemFun, priority);
				break;
			default:
				;
		}
	}

public:
	// functions to register to a specific key event
	template <typename T, typename PtrToMemFun>
	Connection RegisterKeyPressEvent(int key, T &instance, PtrToMemFun ptrToMemFun, unsigned priority = -1)
	{
		return mKeyEvents[key][KeyState::JUST_PRESSED].Bind(instance, ptrToMemFun, priority);
	}

	template <typename T, typename PtrToMemFun>
	Connection RegisterKeyReleaseEvent(int key, T &instance, PtrToMemFun ptrToMemFun, unsigned priority = -1)
	{
		return mKeyEvents[key][KeyState::JUST_RELEASED].Bind(instance, ptrToMemFun, priority);
	}

	template <typename T, typename PtrToMemFun>
	Connection RegisterKeyDownEvent(int key, T &instance, PtrToMemFun ptrToMemFun, unsigned priority = -1)
	{
		return mKeyEvents[key][KeyState::PRESSED].Bind(instance, ptrToMemFun, priority);
	}

	template <typename T, typename PtrToMemFun>
	Connection RegisterKeyUpEvent(int key, T &instance, PtrToMemFun ptrToMemFun, unsigned priority = -1)
	{
		return mKeyEvents[key][KeyState::RELEASED].Bind(instance, ptrToMemFun, priority);
	}

	// functions to register to any key event
	template <typename T, typename PtrToMemFun>
	Connection RegisterKeyPressEvent(T &instance, PtrToMemFun ptrToMemFun, unsigned priority = -1) 
	{ 
		return mKeyPressEvent.Bind(instance, ptrToMemFun, priority); 
	}

	template <typename T, typename PtrToMemFun>
	Connection RegisterKeyReleaseEvent(T &instance, PtrToMemFun ptrToMemFun, unsigned priority = -1) 
	{ 
		return mKeyReleaseEvent.Bind(instance, ptrToMemFun, priority); 
	}

	template <typename T, typename PtrToMemFun>
	Connection RegisterKeyDownEvent(T &instance, PtrToMemFun ptrToMemFun, unsigned priority = -1) 
	{ 
		return mKeyDownEvent.Bind(instance, ptrToMemFun, priority); 
	}

	template <typename T, typename PtrToMemFun>
	Connection RegisterKeyUpEvent(T &instance, PtrToMemFun ptrToMemFun, unsigned priority = -1)
	{
		return mKeyUpEvent.Bind(instance, ptrToMemFun, priority);
	}

	// functions to register to mouse events
	template <typename T, typename PtrToMemFun>
	Connection RegisterMouseMoveEvent(T &instance, PtrToMemFun ptrToMemFun, unsigned priority = -1) 
	{ 
		return mMouseMoveEvent.Bind(instance, ptrToMemFun, priority); 
	}

	template <typename T, typename PtrToMemFun>
	Connection RegisterMousePressEvent(T &instance, PtrToMemFun ptrToMemFun, unsigned priority = -1) 
	{ 
		return mMousePressEvent.Bind(instance, ptrToMemFun, priority); 
	}

	template <typename T, typename PtrToMemFun>
	Connection RegisterMouseReleaseEvent(T &instance, PtrToMemFun ptrToMemFun, unsigned priority = -1) 
	{ 
		return mMouseReleaseEvent.Bind(instance, ptrToMemFun, priority); 
	}

	template <typename T, typename PtrToMemFun>
	Connection RegisterMouseDownEvent(T &instance, PtrToMemFun ptrToMemFun, unsigned priority = -1) 
	{ 
		return mMouseDownEvent.Bind(instance, ptrToMemFun, priority); 
	}

	template <typename T, typename PtrToMemFun>
	Connection RegisterMouseUpEvent(T &instance, PtrToMemFun ptrToMemFun, unsigned priority = -1)
	{
		return mMouseUpEvent.Bind(instance, ptrToMemFun, priority);
	}

	// function to register to generic input event
	template <typename T, typename PtrToMemFun>
	Connection RegisterInputEvent(T &instance, PtrToMemFun ptrToMemFun, unsigned priority = -1) 
	{ 
		return mInputEvent.Bind(instance, ptrToMemFun, priority); 
	}

public:
	static InputManager &GetInstance() { static InputManager instance; return instance; }

	void DispatchInput();

	KeyState GetKeyState(unsigned int scanCode) const;

	KeyState GetMouseButtonState(MouseButton button);
	int GetMouseAbsoluteXPosition() const;
	int GetMouseAbsoluteYPosition() const;
	int GetMouseDeltaXPosition() const;
	int GetMouseDeltaYPosition() const;	     
private:
	InputManager() = default;

	void PollInput();

	void PollKeyboardState();
	static const unsigned int NUM_KEYS = 256U;   // indexes into array of keys are SDL_Scancode values
	unsigned int mCurrentKeys[NUM_KEYS]{};
	unsigned int mPreviousKeys[NUM_KEYS]{};

	SIGNAL(KeySignal);
	KeySignal mKeyEvents[NUM_KEYS][(unsigned int)KeyState::MAX];

	std::map<std::string, KeyChord> mKeyBindings;

	void PollMouseState();
	int mMousePreviousXPosition = 0;    // mouse coordinates are relative to top left corner of window
	int mMousePreviousYPosition = 0;
	int mMouseCurrentXPosition = 0;
	int mMouseCurrentYPosition = 0;
	bool mMousePreviousLeftButton = false;
	bool mMousePreviousRightButton = false;
	bool mMousePreviousMiddleButton = false;
	bool mMouseCurrentLeftButton = false;
	bool mMouseCurrentRightButton = false;
	bool mMouseCurrentMiddleButton = false;
};


#endif  // INPUT_SYSTEM_H
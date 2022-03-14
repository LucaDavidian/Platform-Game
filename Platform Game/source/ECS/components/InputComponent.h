#ifndef INPUT_COMPONENT_H
#define INPUT_COMPONENT_H

#include "../Component.h"
#include "../../data structures/vector.hpp"
#include "../../event system/Event.hpp"
#include "../../Input/InputManager.h"
#include "../../delegates/connection.hpp"

class InputComponent : public Component
{
public:
	template <typename T, typename PtrToMemFun>
	Connection BindAction(const std::string &actionName, T &instance, PtrToMemFun ptrToMemFun)
	{
		if (!InputManager::GetInstance().IsActionMapped(actionName))
			return Connection();

		if (mActionEvents.find(actionName) == mActionEvents.end())
			mActionEvents.insert(std::make_pair(actionName, ActionSignal()));

		return mActionEvents[actionName].Bind(instance, ptrToMemFun);
	}

	virtual void Update();

private:
	SIGNAL(ActionSignal);
	std::map<std::string, ActionSignal> mActionEvents;
};

#endif  // INPUT_COMPONENT_H


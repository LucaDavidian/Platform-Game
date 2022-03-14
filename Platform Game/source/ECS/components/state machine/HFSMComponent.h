#ifndef HFSMCOMPONENT_H
#define HFSMCOMPONENT_H

#include "../../Component.h"
#include "../../../data structures/stack.hpp"
#include "../../../logger/Logger.h"
#include <map>  // #TODO_use_my_implementation
#include <string>
#include <cassert>

class HFSMComponent : public Component
{
public:

	class HFSMState
	{
	public:
		HFSMState(HFSMComponent &HFSM) : mHFSM(HFSM) {}

		virtual void OnEnter() 
		{
			// set start time 
		}

		virtual void OnExit() {}

		virtual void OnUpdate(float deltaTime)   
		{
			// chain of responsibility pattern (successor is parent)
			if (mParent)
				mParent->OnUpdate(deltaTime);
		}

		void SetParent(HFSMState *parent)
		{
			mParent = parent;
		}

		HFSMState *GetParent() const
		{
			return mParent;
		}

		void SetInitialState(HFSMState *initialState)
		{
			mInitialState = initialState;
		}

		HFSMState *GetInitialState() const
		{
			return mInitialState;
		}

	protected:
		HFSMComponent &mHFSM;
		float mStartTime = 0.0f;

	private:
		HFSMState *mParent = nullptr;
		HFSMState *mInitialState = nullptr;
	}; 


	~HFSMComponent()
	{
		for (auto &&state : mStates)
			delete(state.second);
	}

	// create state-transition table/map (#TODO expose to/read from script)
	void AddState(const std::string &stateName, HFSMState *state)      
	{
		if (!mStates.insert(std::make_pair(stateName, state)).second)
			Logger::Instance().Log("FSM error: can't add state " + stateName + ", state already exist", Logger::Level::LOG_ERROR);
	}

	void Initialize()
	{
		HFSMState* initialState = nullptr;

		for (auto statePair : mStates)
			if (!statePair.second->GetParent())
			{
				initialState = statePair.second;
				break;
			}

		assert(initialState);

		static HFSMState rootState(*this);  // this is the root pseudostate

		initialState->SetParent(&rootState);
		rootState.SetInitialState(initialState);

		mCurrentState = &rootState;
		mCurrentState->OnEnter();

		while (mCurrentState->GetInitialState())
		{
			mCurrentState = mCurrentState->GetInitialState();
			mCurrentState->OnEnter();
		}
	}
	
	void ChangeState(HFSMState* mainSourceState, const std::string& mainTargetStateName)
	{
		if (auto it = mStates.find(mainTargetStateName); it != mStates.end())
		{
			HFSMState* mainTargetState = it->second;      // get main target state 
			Transition(mainSourceState, mainTargetState);
		}
	}

	void Update(float deltaTime)
	{
		mCurrentState->OnUpdate(deltaTime);
	}

private:
	std::map<std::string, HFSMState*> mStates;
	HFSMState* mCurrentState = nullptr;

	void Transition(HFSMState* mainSourceState, HFSMState* mainTargetState)
	{
		// 1. find the least common ancestor of the main source state and the main target state

		HFSMState* LCA = nullptr;

		HFSMState* mainSourceAncestor = mainSourceState->GetParent();
		//HFSMState *mainSourceAncestor = mainSourceState;

		while (mainSourceAncestor)
		{
			//HFSMState *mainTargetAncestor = mainTargetState->GetParent();
			HFSMState* mainTargetAncestor = mainTargetState;

			while (mainTargetAncestor && mainSourceAncestor != mainTargetAncestor)
				mainTargetAncestor = mainTargetAncestor->GetParent();

			if (mainSourceAncestor == mainTargetAncestor)
			{
				LCA = mainSourceAncestor;
				break;
			}

			mainSourceAncestor = mainSourceAncestor->GetParent();
		}

		assert(LCA);

		// 2. exit the source state configuration up to the LCA excluded

		while (mCurrentState && mCurrentState != LCA)
		{
			mCurrentState->OnExit();
			mCurrentState = mCurrentState->GetParent();
		}

		assert(mCurrentState);

		// 3. enter the target state configuration starting from below the LCA

		Stack<HFSMState*> targetStateConfiguration;

		HFSMState* state = mainTargetState;

		while (state->GetInitialState())   // find target leaf state
			state = state->GetInitialState();

		while (state != LCA)   // save states from target leaf to LCA excluded
		{
			targetStateConfiguration.Push(state);
			state = state->GetParent();
		}

		while (!targetStateConfiguration.Empty())
		{
			mCurrentState = targetStateConfiguration.Top();
			targetStateConfiguration.Pop();
			mCurrentState->OnEnter();
		}
	}
};

#endif // HFSMCOMPONENT_H




#include "Timer.h"

Timer::Timer()
{
	Reset();
}

void Timer::Reset()
{
	mDeltaTime = std::chrono::steady_clock::duration::zero();
	mPauseDuration = std::chrono::steady_clock::duration::zero();
	mCurrentTime = std::chrono::steady_clock::now();
	mPreviousTime = std::chrono::steady_clock::now();
	mStartTime = std::chrono::steady_clock::now();
	mStopTime = std::chrono::steady_clock::now(),
	mState = State::RESET;
}

void Timer::Start()
{
	switch (mState)
	{
		case State::RESET:
			mStartTime = mPreviousTime = std::chrono::steady_clock::now();
			mState = State::RUNNING;
			break;
		case State::RUNNING:
			break;
		case State::HALTED:
			mStartTime = std::chrono::steady_clock::now();
			mPauseDuration += mStartTime - mStopTime;
			mState = State::RUNNING;
			break;
	}
}

void Timer::Stop()
{
	switch (mState)
	{
		case State::RESET:
			break;
		case State::RUNNING:
			mStopTime = std::chrono::steady_clock::now();
			mState = State::HALTED;
			break;
		case State::HALTED:
			break;
	}
}

void Timer::Tick()
{
	switch (mState)
	{
		case State::RESET:
			break;
		case State::RUNNING:
			mCurrentTime = std::chrono::steady_clock::now();
			std::chrono::steady_clock::duration elapsedTime = mCurrentTime - mPreviousTime - mPauseDuration;
			mPreviousTime = mCurrentTime;
			mDeltaTime = elapsedTime;
			break;
		case State::HALTED:
			break;
	}
}

float Timer::GetDeltaTimeMilliseconds() const
{
	std::chrono::duration<float,std::milli> millis = mDeltaTime;
	return millis.count();
}

float Timer::GetTime() const
{
	std::chrono::steady_clock::time_point currentTime = std::chrono::steady_clock::now();
	return std::chrono::duration<float, std::milli>(currentTime - mStartTime).count();
}
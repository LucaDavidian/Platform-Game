#ifndef TIMER_H
#define TIMER_H

#include <chrono>

class Timer
{
public:
	Timer();
	void Reset();
	void Start();
	void Stop();
	void Tick();
	float GetDeltaTimeMilliseconds() const;
	float GetTime() const;

	static Timer &GlobalTimer() { static Timer globalTimer; return globalTimer; }
private:
	std::chrono::steady_clock::time_point mStartTime;
	std::chrono::steady_clock::time_point mCurrentTime;
	std::chrono::steady_clock::time_point mPreviousTime;
	std::chrono::steady_clock::time_point mStopTime;
	std::chrono::steady_clock::duration mDeltaTime;
	std::chrono::steady_clock::duration mPauseDuration;
	
	enum class State { RESET, RUNNING, HALTED } mState;
};

#endif  // TIMER_H
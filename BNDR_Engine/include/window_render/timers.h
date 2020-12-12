#pragma once
#include <pch.h>

using timePoint = std::chrono::steady_clock::time_point;
using duration = std::chrono::duration<float>;

namespace bndr {

	class DLL_EXPORT Clock {

	protected:
		// start of clock
		timePoint start;
		// end of clock
		timePoint end;
		static timePoint Now() { return std::chrono::high_resolution_clock::now(); }

	public:

		Clock() : start(Clock::Now()), end(Clock::Now()) {}
		// returns the time elapsed since last DeltaTime call
		float DeltaTime();

	};

	class DLL_EXPORT Timer : protected Clock {
		
		// the time interval of the timer
		float timeInterval;
		// -1.0f denotes that the timer is stopped
		float elapsedTime;

	public:

		Timer(float interval) : Clock(), timeInterval(interval), elapsedTime(interval) {}
		// continues the timer count (resets it if timer has previously stopped or continues counting)
		void CountDown(float deltaTime);
		// returns true if the elapsed time is >= the time interval (i.e if time interval is 5.0f then after 5 seconds this will return true)
		// if TimesUp returns true it automatically stops the timer (then the timer can reset once Tick is called)
		bool TimesUp();
		// resets the time interval
		void ResetTimeInterval(float newInterval);
		// get the elapsed time
		float GetElapsedTime() { return elapsedTime; }
	};
}


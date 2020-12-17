#pragma once
#include <pch.h>

using timePoint = std::chrono::steady_clock::time_point;
using duration = std::chrono::duration<float>;

namespace bndr {

	class BNDR_API Clock {

	protected:
		// start of clock
		timePoint start;
		// end of clock
		timePoint end;
		static timePoint now() { return std::chrono::high_resolution_clock::now(); }

	public:

		Clock() : start(Clock::now()), end(Clock::now()) {}
		// returns the time elapsed since last DeltaTime call
		float deltaTime();

	};

	class BNDR_API Timer : protected Clock {
		
		// the time interval of the timer
		float timeInterval;
		// -1.0f denotes that the timer is stopped
		float elapsedTime;

	public:

		Timer(float interval) : Clock(), timeInterval(interval), elapsedTime(interval) {}
		// continues the timer count (resets it if timer has previously stopped or continues counting)
		void countDown(float deltaTime);
		// returns true if the elapsed time is >= the time interval (i.e if time interval is 5.0f then after 5 seconds this will return true)
		// if TimesUp returns true it automatically stops the timer (then the timer can reset once Tick is called)
		bool timesUp();
		// resets the time interval
		void resetTimeInterval(float newInterval);
		// get the elapsed time
		float getElapsedTime() { return elapsedTime; }
	};
}


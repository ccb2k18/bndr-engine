#include <pch.h>
#include "timers.h"

namespace bndr {

	float Clock::DeltaTime() {

		end = Clock::Now();
		// time is in nanoseconds so we must divide by 10^9 to get seconds
		float deltaTime = ((end - start).count())/1000000000.0f;
		start = end;
		return deltaTime;
	}

	void Timer::CountDown(float deltaTime) {

		// advance the timer by the passed time
		elapsedTime -= deltaTime;
	}

	bool Timer::TimesUp() {

		// check if the timer has reached 0 and if so reset the elapsed time to the time interval
		if (elapsedTime <= 0.0f) {

			elapsedTime = timeInterval;
			return true;
		}
		return false;
	}

	void Timer::ResetTimeInterval(float newInterval) {

		// reset the interval if the time is greater than or equal to zero else leave it unchanged
		if (newInterval >= 0.0f) {

			timeInterval = newInterval;
			// start over the timer
			elapsedTime = timeInterval;
		}
	}
}

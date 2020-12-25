/*MIT License

Copyright (c) 2020 Caleb Christopher Bender

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.*/

#include <pch.h>
#include "timers.h"

namespace bndr {

	float Clock::deltaTime() {

		end = Clock::now();
		// time is in nanoseconds so we must multiply by 10^-9 to get seconds
		float deltaTime = ((end - start).count())*0.000000001f;
		start = end;
		return deltaTime;
	}

	void Timer::countDown(float deltaTime) {

		// advance the timer by the passed time
		elapsedTime -= deltaTime;
	}

	bool Timer::timesUp() {

		// check if the timer has reached 0 and if so reset the elapsed time to the time interval
		if (elapsedTime <= 0.0f) {

			elapsedTime = timeInterval;
			return true;
		}
		return false;
	}

	void Timer::resetTimeInterval(float newInterval) {

		// reset the interval if the time is greater than or equal to zero else leave it unchanged
		if (newInterval >= 0.0f) {

			timeInterval = newInterval;
			// start over the timer
			elapsedTime = timeInterval;
		}
	}
}

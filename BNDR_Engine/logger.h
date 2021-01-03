/*MIT License

Copyright (c) 2021 Caleb Christopher Bender

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

#pragma once
#include <ctime>
#include <cstdio>
#include <stdexcept>
#include <string>

namespace bndr {

	// Static Logger class used for logging exceptions or various messages
	class Logger {
	
	public:

		// get the current time stamp
		static std::string getTimeStamp() {

			time_t rawTime;
			struct tm* info;

			time(&rawTime);
			info = localtime(&rawTime);
			char* stamp = asctime(info);
			std::string timeStamp;
			if (stamp != NULL) {

				timeStamp = stamp;
				timeStamp[timeStamp.size() - 1] = '\0';
			}
			return timeStamp;
		}

		// throws an exception
		static void throwException(const char* errorMessage) {

			printf("\nBNDR Exception [%s]: %s\n", Logger::getTimeStamp().c_str(), errorMessage);
			throw std::runtime_error("");
		}

		// displays a message
		static void displayMessage(const char* message) {

			printf("\nBNDR Message [%s]: %s\n", Logger::getTimeStamp().c_str(), message);
		}
	};
}


#define BNDR_EXCEPTION(e) ::bndr::Logger::throwException(e)
#define BNDR_MESSAGE(m) ::bndr::Logger::displayMessage(m)


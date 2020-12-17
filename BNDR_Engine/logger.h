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
			std::string timeStamp = asctime(info);
			timeStamp[timeStamp.size() - 1] = '\0';
			return timeStamp;
		}

		// throws an exception
		static void throwException(const char* errorMessage) {

			printf("BNDR Exception [%s]: %s\n", Logger::getTimeStamp().c_str(), errorMessage);
			throw std::runtime_error("error");
		}

		// displays a message
		static void displayMessage(const char* message) {

			printf("BNDR Message [%s]: %s\n", Logger::getTimeStamp().c_str(), message);
		}
	};
}


#define BNDR_EXCEPTION(e) ::bndr::Logger::throwException(e)
#define BNDR_MESSAGE(m) ::bndr::Logger::displayMessage(m)


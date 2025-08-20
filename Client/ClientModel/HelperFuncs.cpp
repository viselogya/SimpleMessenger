#include "pch.h"
#include "HelperFuncs.h"

std::chrono::system_clock::time_point HelperFuncs::ChronoParse(const std::string& strTime) {
	std::stringstream stream(strTime);
	std::chrono::system_clock::time_point time;

	stream >> std::chrono::parse("%F %R", time);

	return time;
}

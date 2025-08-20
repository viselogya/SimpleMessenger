#include "pch.h"
#include "HelperFuncs.h"

std::optional<std::chrono::system_clock::time_point> HelperFuncs::ChronoParse(const std::string& strTime) {
	std::stringstream stream(strTime);
	std::chrono::system_clock::time_point time;

	stream >> std::chrono::parse("%F %R", time);

	if (stream.fail()) {
		return std::nullopt;
	}

	return time;
}

bool HelperFuncs::isNumber(const std::string& s)
{
	auto it = s.cbegin();

	while (it != s.end() && std::isdigit(*it)) ++it;

	return !s.empty() && it == s.end();
}

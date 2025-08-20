#pragma once

namespace HelperFuncs
{
	// строка должна быть представленна в формате "%F %R"
	std::optional<std::chrono::system_clock::time_point> ChronoParse(const std::string& strTime);

	bool isNumber(const std::string& s);

}

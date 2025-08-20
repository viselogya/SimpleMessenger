#pragma once

namespace HelperFuncs
{
	std::filesystem::path GetAppFolderPath();

	sockaddr_in CreateSockaddr_in(const std::string& IP, const int portNumber);

	// строка должна быть представленна в формате "%F %R"
	std::chrono::system_clock::time_point ChronoParse(const std::string& strTime);

	bool isNumber(const std::string& s);
};
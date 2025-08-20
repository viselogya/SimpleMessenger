#include "pch.h"
#include "HelperFuncs.h"
#include "ClientExceptions.h"

std::filesystem::path HelperFuncs::GetAppFolderPath()
{
	WCHAR buff[MAX_PATH];
	DWORD sz = ::GetModuleFileNameW(NULL, buff, std::size(buff));

	if (sz == 0 || ::GetLastError() == ERROR_INSUFFICIENT_BUFFER)
	{
		throw ClientExceptionSystem("ERROR WITH GET_MODUEL_FILENAMEW");
	}

	const std::filesystem::path path{ buff };

	return path.parent_path();
}

sockaddr_in HelperFuncs::CreateSockaddr_in(const std::string& IP, const int portNumber) {
	sockaddr_in info;
	in_addr ip_to_bin;

	if (::inet_pton(AF_INET, IP.data(), &ip_to_bin) != 1) {
		std::cout << "Error in IP translation to special numeric format:\n" << GetLastError() << '\n';
		system("pause");

		exit(1);
	}

	::ZeroMemory(&info, sizeof(info));
	info.sin_family = AF_INET;
	info.sin_port = ::htons(static_cast<u_short>(portNumber));
	info.sin_addr = ip_to_bin;
	
	return info;
}

std::chrono::system_clock::time_point HelperFuncs::ChronoParse(const std::string& strTime) {
	std::stringstream stream(strTime);
	std::chrono::system_clock::time_point time;

	stream >> std::chrono::parse("%F %R", time);

	return time;
}

bool HelperFuncs::isNumber(const std::string& s)
{
	std::string::const_iterator it = s.begin();

	while (it != s.end() && std::isdigit(*it)) ++it;

	return !s.empty() && it == s.end();
}

#include "pch.h"
#include "HelperFuncs.h"
#include "SocketLayerExceptions.h"

sockaddr_in HelperFuncs::CreateSockaddr_in(const std::string& IP, const uint16_t portNumber) {
	sockaddr_in info;
	in_addr ip_to_bin;

	if (inet_pton(AF_INET, IP.data(), &ip_to_bin) != 1) {
		throw SocketLayerExceptionSystem(std::format("Error in IP translation to special numeric format! GLE = {}", WSAGetLastError()));
	}

	ZeroMemory(&info, sizeof(info));
	info.sin_family = AF_INET;
	info.sin_port = htons(portNumber);
	info.sin_addr = ip_to_bin;

	return info;
}

bool HelperFuncs::is_number(const std::string& s)
{
	auto it = s.begin();

	while (it != s.end() && std::isdigit(*it)) ++it;
	
	return !s.empty() && it == s.end();
}

std::string HelperFuncs::GetOpenSslErrorDescription()
{
	std::string errors;
	unsigned long e;
	char buf[256];
	while ((e = ERR_get_error()) != 0) {
		ERR_error_string_n(e, buf, sizeof(buf));
		errors += buf;
		errors += '\n';
	}

	return errors;
}

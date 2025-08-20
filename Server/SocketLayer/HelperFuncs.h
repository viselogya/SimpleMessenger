#pragma once

namespace HelperFuncs
{
	sockaddr_in CreateSockaddr_in(const std::string& IP, const uint16_t portNumber);

	bool is_number(const std::string& name);

	std::string GetOpenSslErrorDescription();
};


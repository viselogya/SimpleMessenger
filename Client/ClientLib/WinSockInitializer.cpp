#include "pch.h"
#include "WinSockInitializer.h"
#include "ClientExceptions.h"

WinSockInitializer::WinSockInitializer()
{
	WSADATA wsData;

	if (::WSAStartup(MAKEWORD(2, 2), &wsData) != 0)
	{
		throw std::format("WSAStartup failed! GLE = {}", WSAGetLastError());
	}
}


WinSockInitializer::~WinSockInitializer()
{
	::WSACleanup();
}

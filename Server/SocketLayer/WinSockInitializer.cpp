#include "pch.h"
#include "WinSockInitializer.h"
#include "SocketLayerExceptions.h"


WinSockInitializer::WinSockInitializer()
{
	WSADATA wsData;
	
	if (WSAStartup(MAKEWORD(2, 2), &wsData) != 0) 
	{
		throw SocketLayerExceptionSystem(std::format("WSAStartup failed! GLE = {}", WSAGetLastError()));
	}
}


WinSockInitializer::~WinSockInitializer()
{
	WSACleanup();
}

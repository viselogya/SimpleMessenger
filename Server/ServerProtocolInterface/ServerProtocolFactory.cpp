#include "pch.h"
#include "ServerProtocolFactory.h"
#include "ServerHttp.h"


std::shared_ptr<ServerProtocol> ServerProtocolFactory::CreateServerHttp() const {
	return std::make_shared<ServerHttp>();
}

#include "pch.h"
#include "ClientProtocolFactory.h"
#include "ClientHttp.h"

std::shared_ptr<ClientProtocol> ClientProtocolFactory::CreateClientHttp() const {
	return std::make_shared<ClientHttp>();
}

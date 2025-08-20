#pragma once

class ClientProtocol;

class ClientProtocolFactory
{
public:

	std::shared_ptr<ClientProtocol> CreateClientHttp() const;

};


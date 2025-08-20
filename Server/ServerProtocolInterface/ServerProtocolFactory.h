#pragma once

class ServerProtocol;
class DatabaseManager;
class UserAuthentication;
class UserRegistration;
class ChatManager;

class ServerProtocolFactory
{
public:

	std::shared_ptr<ServerProtocol> CreateServerHttp() const;

};


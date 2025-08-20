#pragma once

#include "IHandler.hpp"

class UserService;
class ServerProtocol;

class GetAllUsersHandler : public IHandler
{
public:

	GetAllUsersHandler(UserService& userService, ServerProtocol& serverProtocol);

	~GetAllUsersHandler();

	void BindHandler(HttpRouter& httpRouter) const;

	std::string Handle(const boost::beast::http::request<boost::beast::http::string_body>& request) const;

private:

	UserService& m_userService;
	ServerProtocol& m_serverProtocol;

};


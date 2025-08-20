#pragma once

#include "IHandler.hpp"

class HttpRouter;
class UserService;
class RequestBodyParserFactory;
class JwtManager;
class ServerProtocol;

class LoginHandler : public IHandler
{
public:

	LoginHandler(UserService& userService, RequestBodyParserFactory& requestBodyParserFactory,
		JwtManager& jwtManager, ServerProtocol& serverProtocol);

	~LoginHandler() override = default;

	void BindHandler(HttpRouter& httpRouter) const override;

	std::string Handle(const boost::beast::http::request<boost::beast::http::string_body>& request) const override;

private:

	UserService& m_userService;
	RequestBodyParserFactory& m_requestBodyParserFactory;
	JwtManager& m_jwtManager;

	ServerProtocol& m_serverProtocol;
};

#pragma once

#include "IHandler.hpp"

class HttpRouter;
class JwtManager;
class RequestBodyParserFactory;
class ServerProtocol;

class RefreshHandler : public IHandler
{
public:

	explicit RefreshHandler(RequestBodyParserFactory& requestBodyParserFactory, JwtManager& jwtManager, 
		ServerProtocol& serverProtocol);

	~RefreshHandler() override = default;

	void BindHandler(HttpRouter& httpRouter) const override;

	std::string Handle(const boost::beast::http::request<boost::beast::http::string_body>& request) const override;

private:

	RequestBodyParserFactory& m_requestBodyParserFactory;
	JwtManager& m_jwtManager;
	ServerProtocol& m_serverProtocol;

};


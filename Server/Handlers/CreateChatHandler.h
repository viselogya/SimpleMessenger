#pragma once

#include "IHandler.hpp"

class HttpRouter;
class ChatManager;
class JwtManager;
class RequestBodyParserFactory;
class ServerProtocol;

class CreateChatHandler : public IHandler
{
public:

	explicit CreateChatHandler(ChatManager& chatManager, RequestBodyParserFactory& requestBodyParserFactory,
		JwtManager& jwtManager, ServerProtocol& serverProtocol);

	~CreateChatHandler() override = default;

	void BindHandler(HttpRouter& httpRouter) const override;

	std::string Handle(const boost::beast::http::request<boost::beast::http::string_body>& request) const override;

private:

	ChatManager& m_chatManager;
	JwtManager& m_jwtManager;
	RequestBodyParserFactory& m_requestBodyParserFactory;
	ServerProtocol& m_serverProtocol;

};


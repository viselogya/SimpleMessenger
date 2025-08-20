#pragma once

#include "IHandler.hpp"

class HttpRouter;
class JwtManager;
class ChatManager;
class RequestBodyParserFactory;
class ServerProtocol;

class AddUserInChatHandler : public IHandler
{
public:

	explicit AddUserInChatHandler(ChatManager& chatManager, RequestBodyParserFactory& requestBodyParserFactory,
		JwtManager& jwtManager, ServerProtocol& serverProtocol);

	~AddUserInChatHandler() override = default;

	void BindHandler(HttpRouter& httpRouter) const override;

	std::string Handle(const boost::beast::http::request<boost::beast::http::string_body>& request) const override;

private:

	ChatManager& m_chatManager;
	RequestBodyParserFactory& m_requestBodyParserFactory;
	JwtManager& m_jwtManager;
	ServerProtocol& m_serverProtocol;

};

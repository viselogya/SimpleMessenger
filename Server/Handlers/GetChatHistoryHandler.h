#pragma once

#include "IHandler.hpp"

class HttpRouter;
class ChatManager;
class JwtManager;
class ServerProtocol;
class RequestBodyParserFactory;

class GetChatHistoryHandler : public IHandler
{
public:

	explicit GetChatHistoryHandler(ChatManager& chatManager, RequestBodyParserFactory& requestBodyParserFactory,
		JwtManager& jwtManager, ServerProtocol& serverProtocol);

	~GetChatHistoryHandler() override = default;

	void BindHandler(HttpRouter& httpRouter) const override;

	std::string Handle(const boost::beast::http::request<boost::beast::http::string_body>& request) const override;

private:

	ChatManager& m_chatManager;
	RequestBodyParserFactory& m_requestBodyParserFactory;
	JwtManager& m_jwtManager;
	ServerProtocol& m_serverProtocol;

};


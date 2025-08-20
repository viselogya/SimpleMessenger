#pragma once

#include "IHandler.hpp"

class HttpRouter;
class ChatManager;
class JwtManager;
class ServerProtocol;

class GetUserChatsHandler : public IHandler
{
public:

	explicit GetUserChatsHandler(ChatManager& chatManager, JwtManager& jwtManager, ServerProtocol& serverProtocol);

	~GetUserChatsHandler() override = default;

	void BindHandler(HttpRouter& httpRouter) const override;

	std::string Handle(const boost::beast::http::request<boost::beast::http::string_body>& request) const override;

private:

	ChatManager& m_chatManager;
	JwtManager& m_jwtManager;
	ServerProtocol& m_serverProtocol;

};


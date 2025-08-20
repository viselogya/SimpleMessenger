#include "pch.h"
#include "GetUserChatsHandler.h"

#include "../ServerLogic/ChatManager.h"

#include "../StreamPipeline/HttpRouter.h"
#include "../StreamPipeline/JwtManager.h"

#include "../ServerProtocolInterface/ServerProtocol.hpp"

#include "../DTO/DTO.h"

namespace http = boost::beast::http;

GetUserChatsHandler::GetUserChatsHandler(ChatManager& chatManager, JwtManager& jwtManager, ServerProtocol& serverProtocol)
	:
	m_chatManager(chatManager),
	m_jwtManager(jwtManager),
	m_serverProtocol(serverProtocol)
{}


void GetUserChatsHandler::BindHandler(HttpRouter& httpRouter) const
{
	httpRouter.AddProtectedRoute({ "GET", "/chats" }, RouteHandler(std::bind_front(&GetUserChatsHandler::Handle, this)));
}


std::string GetUserChatsHandler::Handle(const http::request<http::string_body>& request) const
{
	int64_t userId = m_jwtManager.GetIdFromToken(*m_jwtManager.ExtractTokenFromRequest(request));
	auto chats = m_chatManager.GetUserChats(userId);

	return m_serverProtocol.MakeUserChatListResponse(chats);
}

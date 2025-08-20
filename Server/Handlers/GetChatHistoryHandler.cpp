#include "pch.h"
#include "GetChatHistoryHandler.h"
#include "HelperFuncs.h"

#include "../ServerLogic/ChatManager.h"

#include "../StreamPipeline/HttpRouter.h"
#include "../StreamPipeline/RequestBodyParserFactory.h"
#include "../StreamPipeline/RequestBodyParser.h"
#include "../StreamPipeline/JwtManager.h"

#include "../ServerProtocolInterface/ServerProtocol.hpp"

#include "../DTO/DTO.h"

GetChatHistoryHandler::GetChatHistoryHandler(ChatManager& chatManager, RequestBodyParserFactory& requestBodyParserFactory,
	JwtManager& jwtManager, ServerProtocol& serverProtocol)
	:
	m_chatManager(chatManager),
	m_requestBodyParserFactory(requestBodyParserFactory),
	m_jwtManager(jwtManager),
	m_serverProtocol(serverProtocol)
{}


void GetChatHistoryHandler::BindHandler(HttpRouter& httpRouter) const
{
	httpRouter.AddProtectedRoute({ "GET", "/chats/messages" }, RouteHandler(std::bind_front(&GetChatHistoryHandler::Handle, this)));
}


std::string GetChatHistoryHandler::Handle(const boost::beast::http::request<boost::beast::http::string_body>& request) const
{
	auto query = boost::urls::parse_origin_form(request.target());
	if (!query) {
		return m_serverProtocol.BadRequest();
	}
	
	auto chatIdIterator = query->params().find("chat_id");
	if (chatIdIterator == query->params().end()) {
		return m_serverProtocol.BadRequest();
	} 

	std::string chatIdStr = (*chatIdIterator).value;
	if (!HelperFuncs::isNumber(chatIdStr)) {
		return m_serverProtocol.BadRequest();
	}

	int64_t chatId = std::stoll(chatIdStr);

	if (int64_t userId = m_jwtManager.GetIdFromToken(*m_jwtManager.ExtractTokenFromRequest(request));
		!m_chatManager.IsMemberOfChat(userId, chatId))
	{
		return m_serverProtocol.Forbidden();
	}

	auto messages = m_chatManager.GetChatMessages(chatId);

	return m_serverProtocol.MakeChatHistoryResponse(messages);
}

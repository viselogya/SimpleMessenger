#include "pch.h"
#include "AddUserInChatHandler.h"

#include "../ServerLogic/ChatManager.h"

#include "../StreamPipeline/HttpRouter.h"
#include "../StreamPipeline/RequestBodyParserFactory.h"
#include "../StreamPipeline/RequestBodyParser.h"
#include "../StreamPipeline/JwtManager.h"

#include "../ServerProtocolInterface/ServerProtocol.hpp"

AddUserInChatHandler::AddUserInChatHandler(ChatManager& chatManager, RequestBodyParserFactory& requestBodyParserFactory,
	JwtManager& jwtManager, ServerProtocol& serverProtocol)
	:
	m_chatManager(chatManager),
	m_requestBodyParserFactory(requestBodyParserFactory),
	m_jwtManager(jwtManager),
	m_serverProtocol(serverProtocol)
{}


void AddUserInChatHandler::BindHandler(HttpRouter& httpRouter) const
{
	httpRouter.AddProtectedRoute({ "POST", "/chats/add_user" }, RouteHandler(std::bind_front(&AddUserInChatHandler::Handle, this)));
}

std::string AddUserInChatHandler::Handle(const boost::beast::http::request<boost::beast::http::string_body>& request) const
{
	std::shared_ptr<RequestBodyParser> bodyParser = m_requestBodyParserFactory.CreateRequestParser(request);

	std::string token = *m_jwtManager.ExtractTokenFromRequest(request);
	int64_t userId = m_jwtManager.GetIdFromToken(token);

	if (bodyParser == nullptr) {
		return m_serverProtocol.BadRequest();
	}

	std::optional<int64_t> chatId = bodyParser->GetInt64Value("chat_id");
	std::optional<int64_t> addedUserId = bodyParser->GetInt64Value("addedUser_id");

	if (!chatId.has_value() || !addedUserId.has_value() || !m_chatManager.IsMemberOfChat(userId, *chatId))
	{
		return m_serverProtocol.BadRequest();
	}

	if (m_chatManager.IsMemberOfChat(*addedUserId, *chatId))
	{
		return m_serverProtocol.Ok();
	}

	if (!m_chatManager.AddUserInChat(*chatId, *addedUserId)) {
		return m_serverProtocol.BadRequest();
	}

	return m_serverProtocol.Ok();
}

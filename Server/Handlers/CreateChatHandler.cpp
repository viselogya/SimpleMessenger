#include "pch.h"
#include "CreateChatHandler.h"

#include "../ServerLogic/ChatManager.h"

#include "../StreamPipeline/HttpRouter.h"
#include "../StreamPipeline/RequestBodyParserFactory.h"
#include "../StreamPipeline/RequestBodyParser.h"
#include "../StreamPipeline/JwtManager.h"

#include "../ServerProtocolInterface/ServerProtocol.hpp"

namespace http = boost::beast::http;

CreateChatHandler::CreateChatHandler(ChatManager& chatManager, RequestBodyParserFactory& requestBodyParserFactory, 
	JwtManager& jwtManager, ServerProtocol& serverProtocol)
	:
	m_chatManager(chatManager),
	m_jwtManager(jwtManager),
	m_requestBodyParserFactory(requestBodyParserFactory),
	m_serverProtocol(serverProtocol)
{}

void CreateChatHandler::BindHandler(HttpRouter& httpRouter) const
{
	httpRouter.AddProtectedRoute({ "POST", "/create_chat" }, RouteHandler(std::bind_front(&CreateChatHandler::Handle, this)));
}

std::string CreateChatHandler::Handle(const http::request<http::string_body>& request) const
{
	std::string token = *m_jwtManager.ExtractTokenFromRequest(request);
	int64_t userId = m_jwtManager.GetIdFromToken(token);

	std::shared_ptr<RequestBodyParser> bodyParser = m_requestBodyParserFactory.CreateRequestParser(request);

	if (bodyParser == nullptr) {
		return m_serverProtocol.BadRequest();
	}

	std::optional<std::string> chatName = bodyParser->GetStringValue("chat_name");
	if (!chatName.has_value()) {
		return m_serverProtocol.BadRequest();
	}

	size_t chatId = m_chatManager.CreateChat(userId, *chatName);

	return m_serverProtocol.MakeCreateChatResponse(chatId, *chatName);
}

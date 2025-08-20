#include "pch.h"
#include "SendMessageHandler.h"
#include "HelperFuncs.h"

#include "../ServerLogic/ChatManager.h"

#include "../StreamPipeline/HttpRouter.h"
#include "../StreamPipeline/RequestBodyParserFactory.h"
#include "../StreamPipeline/RequestBodyParser.h"
#include "../StreamPipeline/JwtManager.h"

#include "../ServerProtocolInterface/ServerProtocol.hpp"

SendMessageHandler::SendMessageHandler(ChatManager& chatManager, RequestBodyParserFactory& requestBodyParserFactory,
	JwtManager& jwtManager, ServerProtocol& serverProtocol)
	:
	m_chatManager(chatManager),
	m_requestBodyParserFactory(requestBodyParserFactory),
	m_jwtManager(jwtManager),
	m_serverProtocol(serverProtocol)
{}


void SendMessageHandler::BindHandler(HttpRouter& httpRouter) const
{
	httpRouter.AddProtectedRoute({ "POST", "/chats/messages/send_message" }, RouteHandler(
		std::bind_front(&SendMessageHandler::Handle, this)));
}

std::string SendMessageHandler::Handle(const boost::beast::http::request<boost::beast::http::string_body>& request) const
{
	std::shared_ptr<RequestBodyParser> bodyParser = m_requestBodyParserFactory.CreateRequestParser(request);
	std::string token = *m_jwtManager.ExtractTokenFromRequest(request);

	if (bodyParser == nullptr) {
		return m_serverProtocol.BadRequest();
	}

	int64_t userId = m_jwtManager.GetIdFromToken(token);
	std::optional<int64_t> chatId = bodyParser->GetInt64Value("chat_id");
	std::optional<std::string> text = bodyParser->GetStringValue("text");
	std::optional<std::string> timeStr = bodyParser->GetStringValue("time");

	if (!chatId.has_value() || !text.has_value() || !timeStr.has_value()) {
		return m_serverProtocol.BadRequest();
	}

	std::optional<std::chrono::system_clock::time_point> time = HelperFuncs::ChronoParse(*timeStr);

	if (!time.has_value()) {
		return m_serverProtocol.BadRequest();
	}

	if (!m_chatManager.IsMemberOfChat(userId, *chatId))
	{
		return m_serverProtocol.Forbidden();
	}

	m_chatManager.SendMessageToChat(*chatId, userId, *text, *time);

	return m_serverProtocol.Ok();
}

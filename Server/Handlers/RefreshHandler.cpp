#include "pch.h"
#include "RefreshHandler.h"

#include "../StreamPipeline/HttpRouter.h"
#include "../StreamPipeline/RequestBodyParserFactory.h"
#include "../StreamPipeline/RequestBodyParser.h"
#include "../StreamPipeline/JwtManager.h"

#include "../ServerProtocolInterface/ServerProtocol.hpp"

RefreshHandler::RefreshHandler(RequestBodyParserFactory& requestBodyParserFactory, JwtManager& jwtManager,
	ServerProtocol& serverProtocol)
	:
	m_requestBodyParserFactory(requestBodyParserFactory),
	m_jwtManager(jwtManager),
	m_serverProtocol(serverProtocol)
{}

void RefreshHandler::BindHandler(HttpRouter& httpRouter) const
{
	httpRouter.AddRoute({ "POST", "/token/refresh" }, RouteHandler(std::bind_front(&RefreshHandler::Handle, this)));
}

std::string RefreshHandler::Handle(const boost::beast::http::request<boost::beast::http::string_body>& request) const
{
	std::shared_ptr<RequestBodyParser> bodyParser = m_requestBodyParserFactory.CreateRequestParser(request);

	if (bodyParser == nullptr) {
		return m_serverProtocol.BadRequest();
	}

	std::optional<std::string> oldRefreshToken = bodyParser->GetStringValue("refresh_token");
	if (!oldRefreshToken.has_value() || m_jwtManager.VerifyToken(*oldRefreshToken) != VerificationStatus::OK) {
		return m_serverProtocol.BadRequest();
	}

	try
	{
		std::string username = m_jwtManager.GetNameFromToken(*oldRefreshToken);
		int64_t userId = m_jwtManager.GetIdFromToken(*oldRefreshToken);

		std::string newAccesToken = m_jwtManager.CreateAccesToken(userId, username);
		std::string newRefreshToken = m_jwtManager.CreateRefreshToken(userId, username);

		return m_serverProtocol.MakeLoginResponse(newAccesToken, newRefreshToken);
	}
	catch (...) {
		return m_serverProtocol.BadRequest();
	}
}

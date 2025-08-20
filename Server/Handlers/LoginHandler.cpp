#include "pch.h"
#include "LoginHandler.h"

#include "../ServerLogic/UserService.h"

#include "../StreamPipeline/HttpRouter.h"
#include "../StreamPipeline/JwtManager.h"
#include "../StreamPipeline/RequestBodyParserFactory.h"
#include "../StreamPipeline/RequestBodyParser.h"

#include "../ServerProtocolInterface/ServerProtocol.hpp"

LoginHandler::LoginHandler(UserService& userService, RequestBodyParserFactory& requestBodyParserFactory,
	JwtManager& jwtManager, ServerProtocol& serverProtocol)
	:
	m_userService(userService),
	m_requestBodyParserFactory(requestBodyParserFactory),
	m_jwtManager(jwtManager),
	m_serverProtocol(serverProtocol)
{}


void LoginHandler::BindHandler(HttpRouter& httpRouter) const
{
	httpRouter.AddRoute({ "POST", "/login" }, RouteHandler(std::bind_front(&LoginHandler::Handle, this)));
}


std::string LoginHandler::Handle(const boost::beast::http::request<boost::beast::http::string_body>& request) const
{
	std::shared_ptr<RequestBodyParser> bodyParser = m_requestBodyParserFactory.CreateRequestParser(request);

	if (bodyParser == nullptr) {
		return m_serverProtocol.BadRequest();
	}

	std::optional<std::string> username = bodyParser->GetStringValue("username");
	std::optional<std::string> password = bodyParser->GetStringValue("password");

	if (!username.has_value() || !password.has_value()) {
		return m_serverProtocol.BadRequest();
	}

	std::optional<int64_t> userId = m_userService.Authenticate(username.value(), password.value());
	if (!userId.has_value()) {
		return m_serverProtocol.Unauthorized();
	}

	std::string accesToken = m_jwtManager.CreateAccesToken(*userId, *username);
	std::string refreshToken = m_jwtManager.CreateRefreshToken(*userId, *username);

	return m_serverProtocol.MakeLoginResponse(accesToken, refreshToken);
}

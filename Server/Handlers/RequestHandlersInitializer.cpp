#include "pch.h"
#include "RequestHandlersInitializer.h"

#include "LoginHandler.h"
#include "RegistrationHandler.h"
#include "GetAllUsersHandler.h"
#include "CreateChatHandler.h"
#include "GetUserChatsHandler.h"
#include "GetChatHistoryHandler.h"
#include "SendMessageHandler.h"
#include "AddUserInChatHandler.h"
#include "JwtAuthenticationChecker.h"
#include "OptionsRequestHandler.h"
#include "RefreshHandler.h"


RequestHandlersInitializer::RequestHandlersInitializer(HttpRouter& httpRouter, ServerProtocol& serverProtocol,
	RequestBodyParserFactory& requestBodyParserFactory, ChatManager& chatManager,
	UserService& userService, JwtManager& jwtManager)
	:
	m_httpRouter(httpRouter),
	m_serverProtocol(serverProtocol),
	m_requestBodyParserFactory(requestBodyParserFactory),
	m_chatManager(chatManager),
	m_userService(userService),
	m_jwtManager(jwtManager)
{}


RequestHandlersInitializer::~RequestHandlersInitializer() = default;


void RequestHandlersInitializer::Initialize()
{
	m_jwtAuthenticationChecker = std::make_unique<JwtAuthenticationChecker>(m_jwtManager);

	m_jwtAuthenticationChecker->BindChecker(m_httpRouter);

	m_handlers.push_back(std::make_unique<LoginHandler>(m_userService, m_requestBodyParserFactory, m_jwtManager, m_serverProtocol));

	m_handlers.push_back(std::make_unique<RegistrationHandler>(m_userService, m_requestBodyParserFactory, m_jwtManager, m_serverProtocol));

	m_handlers.push_back(std::make_unique<GetAllUsersHandler>(m_userService, m_serverProtocol));

	m_handlers.push_back(std::make_unique<CreateChatHandler>(m_chatManager, m_requestBodyParserFactory, m_jwtManager, m_serverProtocol));

	m_handlers.push_back(std::make_unique<GetUserChatsHandler>(m_chatManager, m_jwtManager, m_serverProtocol));

	m_handlers.push_back(std::make_unique<GetChatHistoryHandler>(m_chatManager, m_requestBodyParserFactory, m_jwtManager, m_serverProtocol));

	m_handlers.push_back(std::make_unique<SendMessageHandler>(m_chatManager, m_requestBodyParserFactory, m_jwtManager, m_serverProtocol));

	m_handlers.push_back(std::make_unique<AddUserInChatHandler>(m_chatManager, m_requestBodyParserFactory, m_jwtManager, m_serverProtocol));

	m_handlers.push_back(std::make_unique<RefreshHandler>(m_requestBodyParserFactory, m_jwtManager, m_serverProtocol));

	m_handlers.push_back(std::make_unique<OptionsRequestHandler>(m_serverProtocol));

	for (const auto& handler : m_handlers) {
		handler->BindHandler(m_httpRouter);
	}
}

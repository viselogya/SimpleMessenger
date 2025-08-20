#pragma once

class HttpRouter;
class ServerProtocol;
class RequestBodyParser;
class RequestBodyParserFactory;
class ChatManager;
class UserService;
class JwtManager;
class JwtAuthenticationChecker;

class IHandler;

class RequestHandlersInitializer
{
public:

	explicit RequestHandlersInitializer(HttpRouter& httpRouter, ServerProtocol& serverProtocol,
		RequestBodyParserFactory& requestBodyParserFactory, ChatManager& chatManager,
		UserService& userService, JwtManager& jwtManager);

	void Initialize();

	~RequestHandlersInitializer();

private:

	std::vector<std::unique_ptr<IHandler>> m_handlers;

	std::unique_ptr<JwtAuthenticationChecker> m_jwtAuthenticationChecker{ nullptr };


	HttpRouter& m_httpRouter;
	ServerProtocol& m_serverProtocol;
	RequestBodyParserFactory& m_requestBodyParserFactory;
	ChatManager& m_chatManager;
	UserService& m_userService;
	JwtManager& m_jwtManager;
};


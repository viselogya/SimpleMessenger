#pragma once

#include <boost/asio/ssl/context.hpp>

class HttpRouter;
class RequestHandlers;
class ChatManager;
class DatabaseManager;
class Listener;
class ConnectionsRegistry;
class ThreadPool;
class Controller;
class ServerProtocol;
class UserAuthentication;
class UserRegistration;
class RequestBodyParserFactory;
class JwtManager;
class SignalsHandler;
class RequestHandlersInitializer;
class UserService;

struct HostConfig;

class Host
{
public:
	
	explicit Host(const HostConfig& hostConfig);

	~Host();

	void WaitUntilUsersDisconnect() const noexcept;

	void Start() const;

private:

	boost::asio::ssl::context m_sslContext{ boost::asio::ssl::context::tlsv13_server };

	std::unique_ptr<SignalsHandler> m_signalsHandler;

	std::unique_ptr<RequestHandlersInitializer> m_requestHandlersInitializer;

	std::shared_ptr<HttpRouter> m_httpRouter;

	std::shared_ptr<ChatManager> m_chatManager;

	std::shared_ptr<Listener> m_listener;

	std::shared_ptr<ConnectionsRegistry> m_connectionRegistry;

	std::shared_ptr<DatabaseManager> m_myMessengerDatabase;

	std::shared_ptr<ThreadPool> m_threadPool;

	std::shared_ptr<Controller> m_controller;

	std::shared_ptr<ServerProtocol> m_serverProtocol;

	std::shared_ptr<UserAuthentication> m_userAuthentication;

	std::shared_ptr<UserRegistration> m_userRegistration;

	std::unique_ptr<UserService> m_userService;

	std::shared_ptr<RequestBodyParserFactory> m_requestBodyParserFactory;

	std::unique_ptr<JwtManager> m_jwtManager;
};

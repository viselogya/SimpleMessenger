#include "pch.h"
#include "Host.h"
#include "HelperFuncs.h"
#include "СonfigStructs.h"
#include "SignalsHandler.h"
#include "ServerExceptions.h"

#include "../Logger/Logger.h"

#include "../SocketLayer/Listener.h"
#include "../SocketLayer/ConnectionsRegistry.h"
#include "../SocketLayer/ConnectionFactory.h"
#include "../SocketLayer/ThreadPool.h"

#include "../ServerLogic/ChatManager.h"
#include "../ServerLogic/UserAuthentication.h"
#include "../ServerLogic/UserRegistration.h"
#include "../ServerLogic/UserService.h"

#include "../StreamPipeline/Controller.h"
#include "../StreamPipeline/HttpRouter.h"
#include "../StreamPipeline/RequestBodyParserFactory.h"
#include "../StreamPipeline/JwtManager.h"

#include "../DatabaseManager/SQLiteDatabaseManagerFactory.h"

#include "../ServerProtocolInterface/ServerProtocolFactory.h"
#include "../ServerProtocolInterface/ServerHttp.h"

#include "../Handlers/RequestHandlersInitializer.h"

Host::~Host() = default;

Host::Host(const HostConfig& hostConfig)
{
	try
	{
		m_sslContext.use_certificate_chain_file(hostConfig.pathsConfig.certificatePath.string());
		m_sslContext.use_rsa_private_key_file(hostConfig.pathsConfig.privateKeyPath.string(), boost::asio::ssl::context::pem);

		auto databaseFactory = std::make_shared<SQLiteDatabaseManagerFactory>(hostConfig.pathsConfig.databasePath);
		m_myMessengerDatabase = databaseFactory->Create();
		
		m_connectionRegistry = std::make_shared<ConnectionsRegistry>();		
		m_threadPool = std::make_shared<ThreadPool>(hostConfig.serverConfig.numberOfThreads);
		m_chatManager = std::make_shared<ChatManager>(*m_myMessengerDatabase);
		m_userAuthentication = std::make_shared<UserAuthentication>(*m_myMessengerDatabase, hostConfig.passwordHashingConfig.salt, 
			hostConfig.passwordHashingConfig.iterations, hostConfig.passwordHashingConfig.hashLength);
		m_userRegistration = std::make_shared<UserRegistration>(*m_myMessengerDatabase, hostConfig.passwordHashingConfig.salt,
			hostConfig.passwordHashingConfig.iterations, hostConfig.passwordHashingConfig.hashLength);
		m_userService = std::make_unique<UserService>(*m_myMessengerDatabase, *m_userRegistration, *m_userAuthentication);

		m_jwtManager = std::make_unique<JwtManager>(hostConfig.jwtConfig);

		auto serverProtocolFactory = std::make_shared<ServerProtocolFactory>();

		m_serverProtocol = serverProtocolFactory->CreateServerHttp();
		m_httpRouter = std::make_shared<HttpRouter>(*m_serverProtocol);

		m_controller = std::make_shared<Controller>(*m_httpRouter, *m_serverProtocol);

		m_requestBodyParserFactory = std::make_shared<RequestBodyParserFactory>();

		m_requestHandlersInitializer = std::make_unique<RequestHandlersInitializer>(*m_httpRouter, *m_serverProtocol, *m_requestBodyParserFactory,
			*m_chatManager, *m_userService, *m_jwtManager);
		m_requestHandlersInitializer->Initialize();

		auto connectionFactory = std::make_shared<ConnectionFactory>(*m_connectionRegistry, *m_controller, *m_threadPool, m_sslContext);
		m_listener = std::make_shared<Listener>(connectionFactory, hostConfig.serverConfig.port);

		m_signalsHandler = std::make_unique<SignalsHandler>();
		
		m_signalsHandler->SetGetAllConnectionsFunction(std::bind_front(
			std::bind_front(&ConnectionsRegistry::GetAllConnections, m_connectionRegistry.get())));

		m_signalsHandler->SetStopListenerFunction(std::bind_front(&Listener::StopListen, m_listener.get()));
	}
	catch (const std::exception& ex)
	{
		Logger::Instance().Error(std::format("Exception in Host: {}", ex.what()));
	}
}


void Host::WaitUntilUsersDisconnect() const noexcept {
	m_threadPool->WaitAll();
}

void Host::Start() const {
	m_signalsHandler->Start();
	m_listener->StartListen();
}

#include "pch.h"
#include "Listener.h"
#include "HelperFuncs.h"
#include "SocketLayerExceptions.h"
#include "ConnectionFactory.h"
#include "Connection.h"

#include "../Logger/Logger.h"

using boost::asio::ip::tcp;

Listener::Listener(std::shared_ptr<ConnectionFactory> connectionFactory, const uint16_t port)
	:
	m_acceptor(m_ioContext, tcp::endpoint(tcp::v4(), port)),
	m_connectionFactory(std::move(connectionFactory))
{}

void Listener::StopListen() {
	m_isAcceptConnectionPoolActive = false;
	m_acceptor.close();
	Logger::Instance().Info("Listen socket are closed");
}

void Listener::StartListen() {
	Logger::Instance().Debug("Listening was started");

	AcceptConnectionPool();
}

void Listener::AcceptConnectionPool() {
	m_isAcceptConnectionPoolActive = true;
	
	while (m_isAcceptConnectionPoolActive) {
		try
		{
			AcceptConnection();
		}
		catch (const std::exception& ex)
		{
			Logger::Instance().Error(std::format("Error with create conection! GLE = {}", ex.what()));
		}
	}
}

void Listener::AcceptConnection() {
	boost::asio::ip::tcp::socket socket(m_ioContext);
	m_acceptor.accept(socket);


	m_connectionFactory->CreateConnection(std::move(socket))->Run();
}

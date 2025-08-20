#include "pch.h"
#include "ConnectionFactory.h"
#include "Connection.h"

ConnectionFactory::ConnectionFactory(ConnectionsRegistry& connectionsRegistry, Controller& controller,
	ThreadPool& threadPool, boost::asio::ssl::context& sslContext)
	:
	m_connectionsRegistry(connectionsRegistry),
	m_controller(controller),
	m_threadPool(threadPool),
	m_sslContext(sslContext)
{}

std::shared_ptr<Connection> ConnectionFactory::CreateConnection(boost::asio::ip::tcp::socket socket) {
	return std::make_shared<Connection>(m_connectionsRegistry, m_controller, m_threadPool, m_sslContext, std::move(socket));
}

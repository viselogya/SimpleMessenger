#pragma once

class Connection;
class ConnectionsRegistry;
class ThreadPool;
class Controller;

class boost::asio::ssl::context;

class ConnectionFactory {
public:

	ConnectionFactory(ConnectionsRegistry& connectionsRegistry, Controller& controller,
		ThreadPool& threadPool, boost::asio::ssl::context& sslContext);

	std::shared_ptr<Connection> CreateConnection(boost::asio::ip::tcp::socket socket);

private:

	ConnectionsRegistry& m_connectionsRegistry;
	Controller& m_controller;
	ThreadPool& m_threadPool;

	boost::asio::ssl::context& m_sslContext;
};


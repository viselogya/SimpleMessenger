#pragma once

class UserAuthentication;
class ConnectionFactory;
class SignalsHandler;

class Listener
{
public:
	
	explicit Listener(std::shared_ptr<ConnectionFactory> connectionFactory, const uint16_t port);

	void StartListen();

	void StopListen();

private:

	void AcceptConnection();
	
	void AcceptConnectionPool();

private:

	boost::asio::io_context m_ioContext{};

	boost::asio::ip::tcp::acceptor m_acceptor;

	std::shared_ptr<ConnectionFactory> m_connectionFactory;

	bool m_isAcceptConnectionPoolActive{ false };
};


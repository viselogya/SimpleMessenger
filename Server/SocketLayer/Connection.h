#pragma once

class ConnectionsRegistry;
class ThreadPool;
class Controller;

class Connection : public std::enable_shared_from_this<Connection>
{
public:

	Connection(ConnectionsRegistry& connectionsRegistry, Controller& controller,
		ThreadPool& threadPool, boost::asio::ssl::context& sslContext, boost::asio::ip::tcp::socket socket);

	~Connection();
	
	void Run();
	
	bool Send(const std::string& text);

	bool Receive();
	
	void CloseSocket() noexcept;

private:

	void ConnectionThread();

	void CloseConnection();

	std::string SocketToIp() const;

private:

	ConnectionsRegistry& m_connectionsRegistry;
	Controller& m_controller;
	ThreadPool& m_threadPool;
	
	uint64_t m_taskId{ 0 };

	boost::beast::flat_buffer m_buffer;
	std::optional<boost::beast::http::request_parser<boost::beast::http::string_body>> m_requstParser;

	boost::asio::ssl::stream<boost::asio::ip::tcp::socket> m_socket;
};


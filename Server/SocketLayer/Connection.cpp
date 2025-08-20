#include "pch.h"
#include "Connection.h"
#include "ConnectionsRegistry.h"
#include "ThreadPool.h"
#include "SocketLayerExceptions.h"
#include "HelperFuncs.h"

#include "../StreamPipeline/Controller.h"

#include "../Logger/Logger.h"

using boost::asio::ip::tcp;

Connection::Connection(ConnectionsRegistry& connectionsRegistry, Controller& controller,
	ThreadPool& threadPool, boost::asio::ssl::context& sslContext, boost::asio::ip::tcp::socket socket)
	:
	m_connectionsRegistry(connectionsRegistry),
	m_controller(controller),
	m_threadPool(threadPool),
	m_socket(std::move(socket), sslContext)
{}


Connection::~Connection()
{
	// костыль для логов
	if(static_cast<int>(m_socket.lowest_layer().native_handle()) != -1) {
		Logger::Instance().Info(std::format("Connection with IP = {}, SOCKET = {} disconnected",
			SocketToIp(), static_cast<int>(m_socket.lowest_layer().native_handle())));
	}

	CloseSocket();
}


void Connection::Run() {
	Logger::Instance().Info(std::format("Connection with IP = {}, SOCKET = {} connected", 
		SocketToIp(), static_cast<int>(m_socket.lowest_layer().native_handle())));
	
	m_socket.handshake(boost::asio::ssl::stream_base::server);

	m_connectionsRegistry.Add(shared_from_this());
	// todo: если здесь выскочит исключение, то что останется в m_connectionsRegistry?
	m_taskId = m_threadPool.AddTask(std::bind_front(&Connection::ConnectionThread, this));
}


bool Connection::Send(const std::string& text) {
	boost::system::error_code er;
	m_socket.write_some(boost::asio::buffer(text), er);

	if (er) {
		Logger::Instance().Debug(std::format("Error with send bytes on IP = {}, SOCKET = {}, GLE: {}",
			SocketToIp(), static_cast<int>(m_socket.lowest_layer().native_handle()), er.what()));

		return false;
	}

	return true;
}


bool Connection::Receive() {
	boost::system::error_code error;
	// todo: убрать магические числа
	size_t recvBytes = m_socket.read_some(m_buffer.prepare(4096), error);
	m_buffer.commit(recvBytes);

	if (error) {
		if(error != boost::asio::ssl::error::stream_truncated) {
			Logger::Instance().Debug(std::format("Error with received bytes on IP = {}, SOCKET = {}, GLE: {}",
				SocketToIp(), static_cast<int>(m_socket.lowest_layer().native_handle()), error.what()));

		}
		return false;
	}

	return true;
}


void Connection::CloseSocket() noexcept {
	boost::system::error_code er;

	/*m_socket.shutdown(er);

	if (er && er == boost::asio::error::eof ||
		er == boost::asio::error::connection_reset ||
		er == boost::asio::ssl::error::stream_truncated) 
	{
		Logger::Instance().Error(std::format("Error with shutdown SSL connection on SOCKET {}, GLE: {}",
			static_cast<int>(m_socket.lowest_layer().native_handle()), er.what()));
	}*/

	m_socket.lowest_layer().close(er);

	if (er) {
		Logger::Instance().Error(std::format("Error with close SOCKET! IP = {}, SOCKET = {}, GLE: {}",
			SocketToIp(), static_cast<int>(m_socket.lowest_layer().native_handle()), er.what()));
	}
}


void Connection::CloseConnection() {
	m_connectionsRegistry.Remove(shared_from_this());
}

std::string Connection::SocketToIp() const
{
	return m_socket.lowest_layer().remote_endpoint().address().to_string();
}

// debug
#include <boost/asio/buffers_iterator.hpp>

void Connection::ConnectionThread()
{
	try {
		while (true) 
		{
			if (m_buffer.size() == 0)
			{
				if (!Receive())
				{
					break;
				}
			}
			
			// debug
			auto data = m_buffer.data();
			std::string s(boost::asio::buffers_begin(data), boost::asio::buffers_end(data));
			std::cerr << s << "\n\n";

			Logger::Instance().Debug(std::format("Request was received on IP = {}, SOCKET = {} and method = {}",
				SocketToIp(), static_cast<int>(m_socket.lowest_layer().native_handle()), s.substr(0, 4)));


			std::optional<std::string> response = m_controller.GetResponse(m_buffer, m_requstParser);

			if (!response.has_value()) {
				continue;
			}

			// debug
			std::cerr << *response << "\n\n";

			Send(*response);

			Logger::Instance().Debug(std::format("Response was sent on IP = {}, SOCKET = {} and Status code = {}",
				SocketToIp(), static_cast<int>(m_socket.lowest_layer().native_handle()), response->substr(9, 3)));
		}
	}
 	catch (const std::exception& ex) {
		Logger::Instance().Error(std::format("Exception in Connection! IP = {}, SOCKET = {}, GLE: {}", 
			SocketToIp(), static_cast<int>(m_socket.lowest_layer().native_handle()), ex.what()));
	}

	CloseConnection();
}

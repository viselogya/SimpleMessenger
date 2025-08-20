#include "pch.h"
#include "Connection.h"
#include "HelperFuncs.h"
#include "ClientExceptions.h"

#include "../Logger/Logger.h"


Connection::Connection(const std::string& IP, const uint32_t port)
	:
	m_socket(::socket(AF_INET, SOCK_STREAM, NULL))
{
	m_context = SSL_CTX_new(TLS_client_method());
	Connect(IP, port);

	m_ssl = SSL_new(m_context);
	SSL_set_fd(m_ssl, static_cast<int>(m_socket));

	if (int retCode = SSL_connect(m_ssl); retCode <= 0) {
		throw ClientExceptionSystem(std::format("Handshake was not successful! GLE: {}", SSL_get_error(m_ssl, retCode)));
	}
}

Connection::~Connection() {
	CloseSocket();
}

void Connection::CloseSocket() const {
	::closesocket(m_socket);

	SSL_shutdown(m_ssl);
	SSL_free(m_ssl);
	SSL_CTX_free(m_context);
}

void Connection::Connect(const std::string& IP, const uint32_t portNumber) const 
{
	if (sockaddr_in hostInfo = HelperFuncs::CreateSockaddr_in(IP, portNumber); 
		::connect(m_socket, (sockaddr*)(&hostInfo), sizeof(hostInfo)) == SOCKET_ERROR) 
	{
		throw ClientExceptionSystem(std::format("Connection to Server is FAILED. Error: {}", WSAGetLastError()));
	}

	Logger::Instance().Info("Connection established SUCCESSFULLY");
}

bool Connection::SendRequest(const std::string& message) const 
{
	return SSL_write(m_ssl, message.data(), static_cast<int>(message.size()));
}

std::optional<std::string> Connection::ReceiveResponse() const {
	std::string text(32768, '\0');
	int recvBytes = SSL_read(m_ssl, text.data(), static_cast<int>(text.size()));

	if (recvBytes == 0) {
		Logger::Instance().Error(std::format("Error with SOCKET! GLE: {}", SSL_get_error(m_ssl, recvBytes)));

		return std::nullopt;
	}

	text.resize(recvBytes);

	return text;
}

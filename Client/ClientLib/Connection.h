#pragma once

class Protocol;

class Connection
{
public:

	Connection(const std::string& IP, const uint32_t port);

	~Connection();

	void CloseSocket() const;

	bool SendRequest(const std::string& message) const;

	std::optional<std::string> ReceiveResponse() const;

private:

	void Connect(const std::string& IP, const uint32_t portNumber) const;

private:

	SOCKET m_socket;

	SSL_CTX* m_context;

	SSL* m_ssl;
};

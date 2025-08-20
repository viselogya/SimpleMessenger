#pragma once

class Connection;

class SignalsHandler
{
public:

	explicit SignalsHandler();

	~SignalsHandler();

	void Start();

	void Stop() noexcept;

	void SetStopListenerFunction(std::function<void()> stopListen);

	void SetGetAllConnectionsFunction(std::function<std::vector<std::shared_ptr<Connection>>()> getAllConnections);


private:

	void OnSignalReceived(const boost::system::error_code& ec, int signalNumber) const;

private:

	boost::asio::io_context m_ioContext;
	boost::asio::signal_set m_signalSet;

	std::thread m_thread;

	std::function<void()> m_stopListen;
	std::function<std::vector<std::shared_ptr<Connection>>()> m_getAllConnections;
};


#include "pch.h"
#include "SignalsHandler.h"

#include "../SocketLayer/Connection.h"

#include "../Logger/Logger.h"

SignalsHandler::SignalsHandler()
    :
    m_signalSet(m_ioContext, SIGINT)
{}


SignalsHandler::~SignalsHandler()
{
    Stop();
}

void SignalsHandler::Start()
{
    m_signalSet.async_wait(std::bind(&SignalsHandler::OnSignalReceived, this, std::placeholders::_1, std::placeholders::_2));

    m_thread = std::thread([this] {
        m_ioContext.run();
        });
}

void SignalsHandler::Stop() noexcept
{
    m_ioContext.stop();

    if (m_thread.joinable()) {
        m_thread.join();
    }
}


void SignalsHandler::SetStopListenerFunction(std::function<void()> stopListen)
{
    m_stopListen = std::move(stopListen);
}

void SignalsHandler::SetGetAllConnectionsFunction(std::function<std::vector<
    std::shared_ptr<Connection>>()> getAllConnections) 
{
    m_getAllConnections = std::move(getAllConnections);
}


void SignalsHandler::OnSignalReceived(const boost::system::error_code& ec, 
    int signalNumber) const
{

    Logger::Instance().Info("CTRL+C signal was called");
    
    if (ec) {
        Logger::Instance().Error(std::format("Something wrong with getting signal! GLE = {}", ec.what()));
    }
    else {
        m_stopListen();
        {
            std::vector<std::shared_ptr<Connection>> connections = m_getAllConnections();
            for (auto& i : connections) {
                i->CloseSocket();
            }
        }
    }
}

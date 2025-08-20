#include "pch.h"
#include "ConnectionsRegistry.h"
#include "Connection.h"


#include "../Logger/Logger.h"


void ConnectionsRegistry::Add(std::shared_ptr<Connection> connection)
{
	std::scoped_lock lock(m_mutexConnectionsByUsers);

	m_connections.insert(connection);

	Logger::Instance().Info(std::format("Number of connections at the moment = {}", m_connections.size()));
}


void ConnectionsRegistry::Remove(std::shared_ptr<Connection> connection) 
{
	std::scoped_lock lock(m_mutexConnectionsByUsers);

	m_connections.erase(connection);

	Logger::Instance().Info(std::format("Number of connections at the moment = {}", m_connections.size()));
}


void ConnectionsRegistry::RemoveAll() 
{
	std::scoped_lock lock(m_mutexConnectionsByUsers);

	std::ranges::for_each(m_connections, [this](auto& i) { Remove(i); });
}


std::vector<std::shared_ptr<Connection>> ConnectionsRegistry::GetAllConnections() const
{
	std::scoped_lock lock(m_mutexConnectionsByUsers);

	std::vector<std::shared_ptr<Connection>> connections;
	
	for (auto i : m_connections) {
		connections.emplace_back(std::move(i));
	}

	return connections;
}

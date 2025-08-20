#pragma once

class Connection;

class ConnectionsRegistry
{
public:

	void Add(std::shared_ptr<Connection> connection);

	void Remove(std::shared_ptr<Connection> connection);

	void RemoveAll();

	std::vector<std::shared_ptr<Connection>> GetAllConnections() const;

private:

	mutable std::mutex m_mutexConnectionsByUsers;

	std::unordered_set<std::shared_ptr<Connection>> m_connections;
};


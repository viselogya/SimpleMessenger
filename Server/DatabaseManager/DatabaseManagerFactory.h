#pragma once

class DatabaseManager;

class DatabaseManagerFactory
{
public:

	DatabaseManagerFactory(const std::filesystem::path& databasePath);

	virtual ~DatabaseManagerFactory() = default;

	virtual std::shared_ptr<DatabaseManager> Create() const = 0;

protected:

	std::filesystem::path m_databasePath;
};


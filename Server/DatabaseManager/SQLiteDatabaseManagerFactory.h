#pragma once

#include "DatabaseManagerFactory.h"

class SQLiteDatabaseManagerFactory : public DatabaseManagerFactory
{
public:
	
	explicit SQLiteDatabaseManagerFactory(const std::filesystem::path& databasePath);

	std::shared_ptr<DatabaseManager> Create() const override;
};


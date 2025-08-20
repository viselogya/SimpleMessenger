#include "pch.h"
#include "SQLiteDatabaseManagerFactory.h"
#include "SQLiteDatabaseManager.h"

SQLiteDatabaseManagerFactory::SQLiteDatabaseManagerFactory(const std::filesystem::path& databasePath)
	:
	DatabaseManagerFactory(databasePath)
{}

std::shared_ptr<DatabaseManager> SQLiteDatabaseManagerFactory::Create() const {
	return std::make_shared<SQLiteDatabaseManager>(m_databasePath);
}
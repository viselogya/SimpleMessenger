#include "pch.h"
#include "DatabaseManagerFactory.h"

DatabaseManagerFactory::DatabaseManagerFactory(const std::filesystem::path& databasePath)
	:
	m_databasePath(databasePath)
{}
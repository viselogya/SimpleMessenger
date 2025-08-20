#pragma once

class DatabaseManager;

class UserAuthentication 
{
public:

	explicit UserAuthentication(DatabaseManager& databaseManager, const std::string salt,
		const int iterationsNumber, const int hashLength);

	std::optional<int64_t> Authentication(const std::string& userName, const std::string& password) const;

private:

	DatabaseManager& m_databaseManager;

	std::string m_salt;

	int m_iterationsNumber;

	int m_hashLength;
	
};


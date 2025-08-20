#pragma once

class DatabaseManager;

class UserRegistration
{
public:

	explicit UserRegistration(DatabaseManager& databaseManager, const std::string salt, 
		const int iterationsNumber, const int hashLength);

	// todo: Пароли нельзя передавать в открытом виде
	std::optional<int64_t> Registration(const std::string& userName, const std::string& password) const;

private:

	DatabaseManager& m_databaseManager;

	std::string m_salt;

	int m_iterationsNumber;

	int m_hashLength;

};


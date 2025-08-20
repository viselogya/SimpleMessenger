#include "pch.h"
#include "UserRegistration.h"

#include "../DatabaseManager/DatabaseManager.h"

UserRegistration::UserRegistration(DatabaseManager& databaseManager, const std::string salt,
	const int iterationsNumber, const int hashLength)
	:
	m_databaseManager(databaseManager),
	m_salt(salt),
	m_iterationsNumber(iterationsNumber),
	m_hashLength(hashLength)
{}

std::optional<int64_t> UserRegistration::Registration(const std::string& userName, const std::string& password) const
{
	std::string hash(m_hashLength, '\0');

	PKCS5_PBKDF2_HMAC(password.data(), static_cast<int>(password.size()), reinterpret_cast<const unsigned char*>(m_salt.data()),
		static_cast<int>(m_salt.size()), m_iterationsNumber, EVP_sha256(), m_hashLength, reinterpret_cast<unsigned char*>(hash.data()));

	if (!m_databaseManager.IsUserExists(userName)) {
		return m_databaseManager.InsertUser(userName, hash);
	}

	return std::nullopt;
}

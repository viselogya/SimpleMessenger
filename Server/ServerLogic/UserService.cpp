#include "pch.h"
#include "UserService.h"
#include "UserRegistration.h"
#include "UserAuthentication.h"

#include "../DTO/DTO.h"

#include "../DatabaseManager/DatabaseManager.h"


UserService::UserService(DatabaseManager& databaseManager, UserRegistration& userRegistration,
	UserAuthentication& userAuthentication)
	:
	m_databaseManager(databaseManager),
	m_userRegistration(userRegistration),
	m_userAuthentication(userAuthentication)
{}


UserService::~UserService() = default;


std::vector<UserDto> UserService::GetAllUsers() const
{
	return m_databaseManager.GetAllUsers();
}


std::optional<int64_t> UserService::Register(const std::string& username, const std::string& password) const
{
	return m_userRegistration.Registration(username, password);
}


std::optional<int64_t> UserService::Authenticate(const std::string& username, const std::string& password) const
{
	return m_userAuthentication.Authentication(username, password);
}

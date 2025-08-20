#pragma once

class DatabaseManager;
class UserRegistration;
class UserAuthentication;
struct UserDto;

class UserService
{
public:

	UserService(DatabaseManager& databaseManager, UserRegistration& userRegistration,
		UserAuthentication& userAuthentication);

	~UserService();

	std::vector<UserDto> GetAllUsers() const;

	std::optional<int64_t> Register(const std::string& username, const std::string& password) const;

	std::optional<int64_t> Authenticate(const std::string& username, const std::string& password) const;

private:

	DatabaseManager& m_databaseManager;
	UserRegistration& m_userRegistration;
	UserAuthentication& m_userAuthentication;

};

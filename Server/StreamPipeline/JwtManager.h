#pragma once

struct JwtConfig;

enum class VerificationStatus
{
	OK,
	TokenExpired,
	InvalidSignature,
	UnknownError
};

class JwtManager
{
public:

	explicit JwtManager(const JwtConfig& jwtConfig);

	std::optional<std::string> ExtractTokenFromRequest(const boost::beast::http::request<boost::beast::http::string_body>&) const;

	std::string CreateAccesToken(const int64_t userId, const std::string& username) const;

	std::string CreateRefreshToken(const int64_t userId, const std::string& username) const;

	VerificationStatus VerifyToken(const std::string& token) const;


	std::string GetNameFromToken(const std::string& token) const;

	int64_t GetIdFromToken(const std::string& token) const;

private:

	std::string m_secret;
	std::chrono::seconds m_accessTokenDuration;
	std::chrono::seconds m_refreshTokenDuration;

};


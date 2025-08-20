#include "pch.h"
#include "JwtManager.h"

#include "../Server/СonfigStructs.h"

using traits = jwt::traits::nlohmann_json;

namespace http = boost::beast::http;

JwtManager::JwtManager(const JwtConfig& jwtConfig)
	:
	m_secret(jwtConfig.secret),
	m_accessTokenDuration(jwtConfig.accessTokenDuration),
	m_refreshTokenDuration(jwtConfig.refreshTokenDuration)
{}


std::optional<std::string> JwtManager::ExtractTokenFromRequest(const http::request<http::string_body>& request) const
{
	auto authorizationHeaderValue = request.find(http::field::authorization);

	if (authorizationHeaderValue == request.end()) {
		return std::nullopt;
	}

	static std::string bearer = "Bearer";

	if (!authorizationHeaderValue->value().contains(bearer)) {
		return std::nullopt;
	}

	return authorizationHeaderValue->value().substr(bearer.size() + 1);
}

std::string JwtManager::CreateAccesToken(const int64_t userId, const std::string& username) const
{
	auto accestoken = jwt::create<traits>()
		.set_algorithm("HS256")
		.set_type("JWT")
		.set_subject(std::to_string(userId))
		.set_payload_claim("username", username)
		.set_expires_at(std::chrono::system_clock::now() + m_accessTokenDuration)
		.sign(jwt::algorithm::hs256{ m_secret });

	return accestoken;
}

std::string JwtManager::CreateRefreshToken(const int64_t userId, const std::string& username) const
{
	auto refreshToken = jwt::create<traits>()
		.set_algorithm("HS256")
		.set_type("JWT")
		.set_subject(std::to_string(userId))
		.set_payload_claim("username", username)
		.set_expires_at(std::chrono::system_clock::now() + m_refreshTokenDuration)
		.sign(jwt::algorithm::hs256{ m_secret });

	return refreshToken;
}

VerificationStatus JwtManager::VerifyToken(const std::string& token) const
{
	auto decodedToken = jwt::decode<traits>(token);
	auto verifier = jwt::verify<traits>()
		.allow_algorithm(jwt::algorithm::hs256{ m_secret });

	try
	{
		verifier.verify(decodedToken);
	}
	catch (const std::exception& ex) {
		const std::string whatStr = ex.what();
		if (whatStr == "invalid signature")
		{
			return VerificationStatus::InvalidSignature;
		}
		if (whatStr == "token expired")
		{
			return VerificationStatus::TokenExpired;
		}

		return VerificationStatus::UnknownError;
	}

	return VerificationStatus::OK;
}


std::string JwtManager::GetNameFromToken(const std::string& token) const
{
	auto decodedToken = jwt::decode<traits>(token);

	return decodedToken.get_payload_claim("username").as_string();
}

int64_t JwtManager::GetIdFromToken(const std::string& token) const
{
	auto decodedToken = jwt::decode<traits>(token);

	return std::stoll(decodedToken.get_subject());
}

#include "pch.h"
#include "JwtAuthenticationChecker.h"

#include "../StreamPipeline/JwtManager.h"
#include "../StreamPipeline/HttpRouter.h"


JwtAuthenticationChecker::JwtAuthenticationChecker(JwtManager& jwtManager)
	:
	m_jwtManager(jwtManager)
{}


void JwtAuthenticationChecker::BindChecker(HttpRouter& httpRouter) const
{
	httpRouter.SetAuthorizationValidate(std::bind_front(&JwtAuthenticationChecker::Check, this));
}


bool JwtAuthenticationChecker::Check(const boost::beast::http::request<boost::beast::http::string_body>& request) const
{
	std::optional<std::string> token = m_jwtManager.ExtractTokenFromRequest(request);

	if (!token.has_value())
	{
		return false;
	}

	return m_jwtManager.VerifyToken(*token) == VerificationStatus::OK;
}

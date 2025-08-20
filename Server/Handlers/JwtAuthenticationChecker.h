#pragma once


class HttpRouter;
class JwtManager;

class JwtAuthenticationChecker
{
public:

	explicit JwtAuthenticationChecker(JwtManager& jwtManager);

	~JwtAuthenticationChecker() = default;

	void BindChecker(HttpRouter& httpRouter) const;

	bool Check(const boost::beast::http::request<boost::beast::http::string_body>& request) const;

private:

	JwtManager& m_jwtManager;

};


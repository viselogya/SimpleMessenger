#pragma once

class ServerProtocol;

using RouteHandler = std::function<std::string(const boost::beast::http::request<boost::beast::http::string_body>&)>;
using MethodAndUrl = std::pair<std::string, std::string>;

struct MethodAndUrlHash {
	size_t operator()(const MethodAndUrl& a) const {
		size_t seed = 0;
		boost::hash_combine(seed, a.first);
		boost::hash_combine(seed, a.second);

		return seed;
	}
};

class HttpRouter
{
public:

	explicit HttpRouter(ServerProtocol& serverProtocol);

	void SetAuthorizationValidate(const std::function<bool(
		const boost::beast::http::request<boost::beast::http::string_body>&)>& isAuthorizationValidate);

	void AddRoute(const MethodAndUrl& methodAndUrl, const RouteHandler& handler);

	void AddProtectedRoute(const MethodAndUrl& methodAndUrl, const RouteHandler& handler);

	std::string RoutingRequest(const boost::beast::http::request<boost::beast::http::string_body>& request);
	
private:

	ServerProtocol& m_serverProtocol;

	std::unordered_map<MethodAndUrl, RouteHandler, MethodAndUrlHash> m_routes;
	std::unordered_map<MethodAndUrl, RouteHandler, MethodAndUrlHash> m_protectedRoutes;

	std::function<bool(const boost::beast::http::request<boost::beast::http::string_body>&)> m_isAuthorizationValidate;
};


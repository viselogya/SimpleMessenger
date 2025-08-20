#include "pch.h"
#include "HttpRouter.h"

#include "../ServerProtocolInterface/ServerProtocol.hpp"

namespace http = boost::beast::http;

HttpRouter::HttpRouter(ServerProtocol& serverProtocol)
	:
	m_serverProtocol(serverProtocol)
{}

void HttpRouter::SetAuthorizationValidate(const std::function<bool(
	const http::request<http::string_body>&)>& isAuthorizationValidate)
{
	m_isAuthorizationValidate = isAuthorizationValidate;
}


void HttpRouter::AddRoute(const MethodAndUrl& methodAndUrl, const RouteHandler& handler) 
{
	m_routes[methodAndUrl] = handler;
}


void HttpRouter::AddProtectedRoute(const MethodAndUrl& methodAndUrl, const RouteHandler& handler)
{
	m_protectedRoutes[methodAndUrl] = handler;
}


std::string HttpRouter::RoutingRequest(const boost::beast::http::request<http::string_body>& request)
{
	MethodAndUrl methodAndUrl = std::make_pair(request.method_string(), boost::urls::parse_origin_form(request.target())->path());

	if (m_protectedRoutes.contains(methodAndUrl)) 
	{
		if (!m_isAuthorizationValidate(request)) 
		{
			return m_serverProtocol.Unauthorized();
		}

		return m_protectedRoutes[methodAndUrl](request);
	}

	if (!m_routes.contains(methodAndUrl)) {
		return m_serverProtocol.NotFound();
	}

	return m_routes[methodAndUrl](request);
}

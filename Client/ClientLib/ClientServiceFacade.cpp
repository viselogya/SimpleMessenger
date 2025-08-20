#include "pch.h"
#include "ClientServiceFacade.h"
#include "Connection.h"

#include "../ClientProtocolInterface/HttpResponseAssembler.h"
#include "../ClientProtocolInterface/HttpResponseValidator.h"

#include "../Http/Request.h"
#include "../Http/Response.h"

ClientServiceFacade::ClientServiceFacade(Connection& connection, HttpResponseAssembler& httpResponseAssembler,
	HttpResponseValidator& httpResponseValidator)
	:
	m_connection(connection),
	m_httpResponseAssembler(httpResponseAssembler),
	m_httpResponseValidator(httpResponseValidator)
{}

std::optional<Http::Response> ClientServiceFacade::PerformRequest(const Http::Request& request) const
{
	if (!m_connection.SendRequest(request.ToString())) {
		return std::nullopt;
	}

	std::optional<std::string> response = m_httpResponseAssembler.AssembleHttpResponse(m_connection);

	if (!response.has_value()) {
		return std::nullopt;
	}

	if (!m_httpResponseValidator.IsResponseValid(*response)) {
		return std::nullopt;
	}

	Http::Response httpResponse;
	httpResponse.Parse(*response);

	return httpResponse;
}

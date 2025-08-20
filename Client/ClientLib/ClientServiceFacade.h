#pragma once

namespace Http {
	class Response;
	class Request;
}

class Connection;
class HttpResponseValidator;
class HttpResponseAssembler;

class ClientServiceFacade
{
public:

	explicit ClientServiceFacade(Connection& connection, HttpResponseAssembler& httpResponseAssembler,
		HttpResponseValidator& httpResponseValidator);

	std::optional<Http::Response> PerformRequest(const Http::Request& request) const;

private:

	Connection& m_connection;
	HttpResponseAssembler& m_httpResponseAssembler;
	HttpResponseValidator& m_httpResponseValidator;

};


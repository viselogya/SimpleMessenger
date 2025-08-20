#pragma once

#include "IHandler.hpp"

class HttpRouter;
class ServerProtocol;

class OptionsRequestHandler : public IHandler
{
public:

	explicit OptionsRequestHandler(ServerProtocol& serverProtocol);

	~OptionsRequestHandler() override = default;

	void BindHandler(HttpRouter& httpRouter) const override;

	std::string Handle(const boost::beast::http::request<boost::beast::http::string_body>& request) const override;

private:

	ServerProtocol& m_serverProtocol;

};


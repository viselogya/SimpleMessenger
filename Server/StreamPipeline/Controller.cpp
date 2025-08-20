#include "pch.h"
#include "Controller.h"
#include "HttpRouter.h"

#include "../Logger/Logger.h"

#include "../ServerProtocolInterface/ServerProtocol.hpp"

namespace http = boost::beast::http;

Controller::Controller(HttpRouter& httpRouter, ServerProtocol& serverProtocol)
	:
	m_httpRouter(httpRouter),
	m_serverProtocol(serverProtocol)
{}


Controller::~Controller() = default;


std::optional<std::string> Controller::GetResponse(boost::beast::flat_buffer& buffer, 
	std::optional<http::request_parser<http::string_body>>& requestParser)
{
	if (!requestParser.has_value()) {
		requestParser.emplace();
	}

	while(!requestParser->is_done())
	{
		boost::system::error_code er;
		size_t parsedBytes = requestParser->put(buffer.data(), er);
		buffer.consume(parsedBytes);

		if (er == boost::beast::http::error::need_more) {
			return std::nullopt;
		}

		if (er) {
			requestParser.reset();
			Logger::Instance().Debug(std::format("Error with parsing reqeust! GLE = {}", er.what()));

			return m_serverProtocol.BadRequest();
		}
	}
	
	std::string response = m_httpRouter.RoutingRequest(requestParser->get());
	requestParser.reset();

	return response;
}

#pragma once

class StringBuffer;
class HttpRequestCompletenessChecker;
class HttpRouter;
class RequestsSplitter;
class RequestValidator;
class ServerProtocol;

class Controller
{
public:

	explicit Controller(HttpRouter& m_httpRouter, ServerProtocol& serverProtocol);

	~Controller();

	std::optional<std::string> GetResponse(boost::beast::flat_buffer& buffer, 
		std::optional<boost::beast::http::request_parser<boost::beast::http::string_body>>& requestParser);

private:

	HttpRouter& m_httpRouter;
	ServerProtocol& m_serverProtocol;

};


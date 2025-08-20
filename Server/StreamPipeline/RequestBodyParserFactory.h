#pragma once

class RequestBodyParser;

class RequestBodyParserFactory
{
public:

	std::shared_ptr<RequestBodyParser> CreateRequestParser(
		const boost::beast::http::request<boost::beast::http::string_body>& request) const;

};


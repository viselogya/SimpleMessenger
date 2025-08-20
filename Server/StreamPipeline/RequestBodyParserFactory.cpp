#include "pch.h"
#include "RequestBodyParserFactory.h"
#include "XmlRequestBodyParser.h"
#include "JsonRequestBodyParser.h"

namespace http = boost::beast::http;

std::shared_ptr<RequestBodyParser> RequestBodyParserFactory::CreateRequestParser(
	const http::request<http::string_body>& request) const
{

	auto contentType = request.find(http::field::content_type);

	if (contentType == request.end()) {
		return nullptr;
	}

	std::shared_ptr<RequestBodyParser> bodyParser = nullptr;

	if (contentType->value() == "application/json")
	{
		bodyParser = std::make_shared<JsonRequestBodyParser>();
	}

	if (contentType->value() == "application/xml")
	{
		bodyParser = std::make_shared<XmlRequestBodyParser>();
	}

	if (!bodyParser->Parse(request.body())) {
		return nullptr;
	}

	return bodyParser;
}

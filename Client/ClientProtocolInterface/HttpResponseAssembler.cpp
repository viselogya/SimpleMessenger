#include "pch.h"
#include "HttpResponseAssembler.h"
#include "HttpResponseCompletenessChecker.h"

#include "../ClientLib/Connection.h"

HttpResponseAssembler::HttpResponseAssembler(HttpResponseCompletenessChecker& httpResponseCompletenessChecker)
	:
	m_httpResponseCompletenessChecker(httpResponseCompletenessChecker)
{}

HttpResponseAssembler::~HttpResponseAssembler() = default;

std::optional<std::string> HttpResponseAssembler::AssembleHttpResponse(const Connection& connection) const
{
	std::string buffer;
	CompletenessResponseStatus status = CompletenessResponseStatus::NotFull;

	while (status != CompletenessResponseStatus::Full)
	{
		std::optional<std::string> response = connection.ReceiveResponse();

		if (!response.has_value()) {
			return std::nullopt;
		}

		buffer += *response;

		status = m_httpResponseCompletenessChecker.IsResponseComplete(buffer);

		if (status == CompletenessResponseStatus::LimitExceeded) {
			return std::nullopt;
		}
	}

	return buffer;
}

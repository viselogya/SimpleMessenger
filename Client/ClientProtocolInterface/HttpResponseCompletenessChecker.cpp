#include "pch.h"
#include "HttpResponseCompletenessChecker.h"

HttpResponseCompletenessChecker::HttpResponseCompletenessChecker(const uint32_t limitRecvHeaderSymbols,
	const uint32_t limitRecvBodySymbols)
	:
	m_limitRecvHeaderSymbols(limitRecvHeaderSymbols),
	m_limitRecvBodySymbols(limitRecvBodySymbols)
{}

CompletenessResponseStatus HttpResponseCompletenessChecker::IsResponseComplete(const std::string& buffer) const
{
	std::string emptyLineSeparator = "\r\n\r\n";
	size_t emptyLineSeparatorPos = buffer.find(emptyLineSeparator);

	if (emptyLineSeparatorPos == std::string_view::npos) {
		if (buffer.size() > m_limitRecvHeaderSymbols) {
			return CompletenessResponseStatus::LimitExceeded;
		}

		return CompletenessResponseStatus::NotFull;
	}

	static const std::regex contentLengthRegex(R"(\r\nContent-Length: (\d+)\r\n)");
	std::smatch match;

	if (std::regex_search(buffer, match, contentLengthRegex) == false) {
		return CompletenessResponseStatus::Full;
	}

	uint32_t contentLength = std::stoul(match[1]);

	if (contentLength > m_limitRecvBodySymbols) {
		return CompletenessResponseStatus::LimitExceeded;
	}

	size_t bufferSizeAfterEmpryLine = buffer.size() - emptyLineSeparatorPos - emptyLineSeparator.size();

	if (bufferSizeAfterEmpryLine < contentLength) {
		return CompletenessResponseStatus::NotFull;
	}

	return CompletenessResponseStatus::Full;
}

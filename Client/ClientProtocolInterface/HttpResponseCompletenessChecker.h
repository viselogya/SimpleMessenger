#pragma once

enum class CompletenessResponseStatus {
	Full,
	NotFull,
	LimitExceeded
};

class HttpResponseCompletenessChecker
{
public:

	explicit HttpResponseCompletenessChecker(const uint32_t limitRecvHeaderSymbols, const uint32_t limitRecvBodySymbols);

	CompletenessResponseStatus IsResponseComplete(const std::string& buffer) const;

private:

	uint32_t m_limitRecvHeaderSymbols;
	uint32_t m_limitRecvBodySymbols;

};


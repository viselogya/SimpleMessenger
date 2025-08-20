#include "pch.h"
#include "HttpResponseValidator.h"

bool HttpResponseValidator::IsResponseValid(const std::string response) const
{
	std::string startLineRegexStr = R"(^HTTP/\d\.\d \d{3} [^\r\n]+\r\n)";
	std::string headerLinesRegexStr = R"(([A-Za-z-]+: [^\r\n]+\r\n)*\r\n)";
	std::string bodyRegexStr = R"([\s\S]*$)";
	std::string responseRegexStr = startLineRegexStr + headerLinesRegexStr + bodyRegexStr;

	static const std::regex regexResponse(responseRegexStr);
	
	return std::regex_match(response, regexResponse);
}

#pragma once

class Connection;
class HttpResponseCompletenessChecker;

class HttpResponseAssembler
{
public:

	explicit HttpResponseAssembler(HttpResponseCompletenessChecker& httpResponseCompletenessChecker);

	~HttpResponseAssembler();

	std::optional<std::string> AssembleHttpResponse(const Connection& connection) const;

private:

	HttpResponseCompletenessChecker& m_httpResponseCompletenessChecker;

};


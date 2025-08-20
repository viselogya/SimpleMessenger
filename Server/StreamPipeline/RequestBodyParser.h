#pragma once
class RequestBodyParser
{
public:

	virtual ~RequestBodyParser() = default;

	virtual bool Parse(const std::string& str) = 0;

	virtual std::optional<int64_t> GetInt64Value(const std::string& nodeName) const = 0;

	virtual std::optional<std::string> GetStringValue(const std::string& nodeName) const = 0;

};


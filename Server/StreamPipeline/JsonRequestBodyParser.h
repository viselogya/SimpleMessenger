#pragma once

#include "RequestBodyParser.h"

class JsonRequestBodyParser : public RequestBodyParser
{
public:

	~JsonRequestBodyParser() override = default;

	bool Parse(const std::string& str) override;

	std::optional<int64_t> GetInt64Value(const std::string& nodeName) const override;

	std::optional<std::string> GetStringValue(const std::string& nodeName) const override;

private:
	
	nlohmann::json m_json;

};


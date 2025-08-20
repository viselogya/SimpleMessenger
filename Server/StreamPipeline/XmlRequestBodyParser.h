#pragma once

#include "RequestBodyParser.h"

class XmlRequestBodyParser : public RequestBodyParser
{
public:

	~XmlRequestBodyParser() override = default;

	bool Parse(const std::string& str) override;

	std::optional<int64_t> GetInt64Value(const std::string& nodeName) const override;

	std::optional<std::string> GetStringValue(const std::string& nodeName) const override;

private:

	pugi::xml_document m_doc;

};


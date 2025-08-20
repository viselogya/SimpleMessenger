#include "pch.h"
#include "XmlRequestBodyParser.h"
#include "HelperFuncs.h"

bool XmlRequestBodyParser::Parse(const std::string& str)
{
	return m_doc.load_string(str.data());
}

std::optional<int64_t> XmlRequestBodyParser::GetInt64Value(const std::string& nodeName) const
{ 
	auto nodePath = m_doc.select_node(std::format("/main/{}", nodeName).data());

	if (!nodePath) {
		return std::nullopt;
	}

	std::string value = nodePath.node().value();

	if (!HelperFuncs::isNumber(value)) {
		return std::nullopt;
	}

	return std::stoll(value);
}

std::optional<std::string> XmlRequestBodyParser::GetStringValue(const std::string& nodeName) const
{
	auto nodePath = m_doc.select_node(std::format("/main/{}", nodeName).data());

	if (!nodePath) {
		return std::nullopt;
	}

	return nodePath.node().value();
}

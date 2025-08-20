#include "pch.h"
#include "JsonRequestBodyParser.h"

bool JsonRequestBodyParser::Parse(const std::string& str)
{
	if (!nlohmann::json::accept(str)) {
		return false;
	}

	m_json = nlohmann::json::parse(str);

	return true;
}


std::optional<int64_t> JsonRequestBodyParser::GetInt64Value(const std::string& nodeName) const
{
	if (!m_json.contains(nodeName))
	{
		return std::nullopt;
	}

	if (!m_json[nodeName].is_number()) {
		return std::nullopt;
	}

	return m_json[nodeName];
}

std::optional<std::string> JsonRequestBodyParser::GetStringValue(const std::string& nodeName) const
{
	if (!m_json.contains(nodeName))
	{
		return std::nullopt;
	}

	if (!m_json[nodeName].is_string()) {
		return std::nullopt;
	}

	return m_json[nodeName];
}

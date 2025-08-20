#include "pch.h"
#include "HelperFuncs.h"

bool HelperFuncs::isNumber(const std::string& s)
{
	auto it = s.begin();

	while (it != s.end() && std::isdigit(*it)) ++it;

	return !s.empty() && it == s.end();
}

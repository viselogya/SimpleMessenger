#pragma once

struct DatabaseException : public std::runtime_error
{
	using std::runtime_error::runtime_error;
};
#pragma once

struct SocketLayerExceptionSystem : public std::runtime_error
{
	using std::runtime_error::runtime_error;
};
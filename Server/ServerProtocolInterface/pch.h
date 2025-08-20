#pragma once

// stl
#include <chrono>
#include <string>

// JsonLib
#include <nlohmann/json.hpp>

// JwtLib
#define JWT_DISABLE_PICOJSON
#include <jwt-cpp/traits/nlohmann-json/traits.h>


// BOOST
#include <boost/beast/http/write.hpp>
#include <boost/beast/http/message.hpp>
#include <boost/beast/http/string_body.hpp>
#include <boost/beast/http/empty_body.hpp>
#include <boost/lexical_cast.hpp>

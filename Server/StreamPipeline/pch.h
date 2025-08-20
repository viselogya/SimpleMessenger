#pragma once

// stl
#include <chrono>
#include <functional>
#include <sstream>
#include <string>
#include <string_view>
#include <regex>
#include <unordered_map>
#include <memory>
#include <iostream>
#include <fstream>
#include <mutex>


// JSON_LIB
#include <nlohmann/json.hpp>


// XML_LIB
#include <pugixml.hpp>


// JWT-CPP
#define JWT_DISABLE_PICOJSON
#include <jwt-cpp/traits/nlohmann-json/traits.h>


// BOOST
#include <boost/beast/core/flat_buffer.hpp>
#include <boost/beast/http/parser.hpp>
#include <boost/beast/http/string_body.hpp>
#include <boost/beast/http/message.hpp>

#include <boost/functional/hash_fwd.hpp>

#include <boost/url/parse.hpp>

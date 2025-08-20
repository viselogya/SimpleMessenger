#pragma once

// WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>

// STL
#include <algorithm>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <format>
#include <functional>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <memory>
#include <mutex>
#include <optional>
#include <queue>
#include <cstdio>
#include <string>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <vector>


// OPEN_SSL
#include <openssl/ssl.h>
#include <openssl/err.h>


// BOOST
#include <boost/asio/io_context.hpp>
#include <boost/asio/buffer.hpp>

#include <boost/asio/ssl/stream_base.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <boost/asio/ssl/context.hpp>

#include <boost/asio/ip/tcp.hpp>

#include <boost/beast/core/flat_buffer.hpp>

#include <boost/beast/http/string_body.hpp>
#include <boost/beast/http/parser.hpp>

#include <boost/system/error_code.hpp>

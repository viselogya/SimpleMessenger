#pragma once

// WINDOWS
#include <WinSock2.h>
#include <WS2tcpip.h>

// STL
#include <stdio.h>
#include <algorithm>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <optional>
#include <queue>
#include <string>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <vector>

// JSON_LIB
#include <nlohmann/json.hpp>

// OPEN_SSL
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/x509.h>
#include <openssl/x509v3.h>
#include <openssl/pem.h>
#include <openssl/evp.h>
#include <openssl/ec.h>
#include <openssl/kdf.h>
#include <openssl/pkcs12.h>
#include <openssl/hmac.h>

// BOOST
#include <boost/asio/io_context.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio/signal_set.hpp>

#include <boost/asio/ssl/stream_base.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <boost/asio/ssl/context.hpp>

#include <boost/asio/ip/tcp.hpp>

#include <boost/beast/core/flat_buffer.hpp>

#include <boost/beast/http/string_body.hpp>
#include <boost/beast/http/parser.hpp>

#include <boost/system/error_code.hpp>

#include <boost/functional/hash_fwd.hpp>

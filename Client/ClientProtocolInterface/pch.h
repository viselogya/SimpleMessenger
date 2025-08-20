#pragma once

//windows
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>

// stl
#include <string>
#include <chrono>
#include <memory>
#include <regex>

// json
#include <nlohmann/json.hpp>

// OpenSSLlib
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

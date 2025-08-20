#pragma once

struct ServerConfig {
    uint16_t port;
    int32_t numberOfThreads;
};


struct PathsConfig {
    std::filesystem::path databasePath;
    std::filesystem::path logFilePath;
    std::filesystem::path certificatePath;
    std::filesystem::path privateKeyPath;
};


struct HttpLimits {
    uint32_t maxHeaderSize;
    uint32_t maxBodySize;
};


struct PasswordHashingConfig {
    std::string salt;
    uint32_t iterations;
    uint32_t hashLength;
};

struct JwtConfig {
    std::string secret;
    std::chrono::seconds accessTokenDuration;
    std::chrono::seconds refreshTokenDuration;
};


struct HostConfig {
    ServerConfig serverConfig;
    PathsConfig pathsConfig;
    PasswordHashingConfig passwordHashingConfig;
    JwtConfig jwtConfig;
};


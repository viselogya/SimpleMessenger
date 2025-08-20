#pragma once

class ClientConfig
{
public:

	ClientConfig();

	std::string GetIP() const;

	uint32_t GetPort() const;

	std::filesystem::path GetLogFilePath() const;

private:

	mutable std::mutex m_mutexConfig;

	nlohmann::json m_config;
};


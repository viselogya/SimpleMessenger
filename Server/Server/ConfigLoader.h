#pragma once

using json = nlohmann::json;

class ConfigLoader
{
public:

	explicit ConfigLoader();
;
	
	uint16_t GetPort() const;


	std::filesystem::path GetUsersDatabasePath() const;

	std::filesystem::path GetLogFilePath() const;

	std::filesystem::path GetCertificatePath() const;

	std::filesystem::path GetPrivateKeyPath() const;

	
	int32_t GetNumberOfThreadsInThreadPool() const;

	uint32_t GetMaxHeaderSize() const;

	uint32_t GetMaxBodySize() const;

	std::string GetSalt() const;

	uint32_t GetNumberOfIterations() const;

	uint32_t GetHashLength() const;


	std::string GetJwtSecrete() const;

	std::chrono::seconds GetAccesTokenDuration() const;

	std::chrono::seconds GetRefreshTokenDuration() const;

private:

	mutable std::mutex m_mutexConfig;

	nlohmann::json m_config;
}; 


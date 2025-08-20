#include "pch.h"
#include "ClientConfig.h"
#include "HelperFuncs.h"

using json = nlohmann::json;

ClientConfig::ClientConfig()
{
	try
	{
		std::ifstream configStream(HelperFuncs::GetAppFolderPath() / "../../configs/ClientConfig.json");
		m_config = json::parse(configStream);
	}
	catch (const std::exception& ex)
	{
		std::cout << ex.what();
		throw;
	}
}

std::string ClientConfig::GetIP() const {
	std::scoped_lock lock(m_mutexConfig);

	return m_config["IPv4"];
}

uint32_t ClientConfig::GetPort() const {
	std::scoped_lock lock(m_mutexConfig);

	return m_config["port"];
}

std::filesystem::path ClientConfig::GetLogFilePath() const {
	std::scoped_lock lock(m_mutexConfig);

	return m_config["LogFilePath"];
}

#include "pch.h"
#include "ConfigLoader.h"
#include "HelperFuncs.h"
#include "ServerExceptions.h"

ConfigLoader::ConfigLoader()
{
	try
	{
		std::ifstream configStream(HelperFuncs::GetAppFolderPath() / "../../config/ServerConfig.json");
		m_config = json::parse(configStream);
	}
	catch (const std::exception& ex)
	{
		std::cout << ex.what();
		throw;
	}
}

uint16_t ConfigLoader::GetPort() const 
{
	return m_config["Server"]["Port"];
}

std::filesystem::path ConfigLoader::GetUsersDatabasePath() const 
{
	return m_config["Paths"]["DatabasePath"];
}

std::filesystem::path ConfigLoader::GetLogFilePath() const 
{
	return m_config["Paths"]["LogFilePath"];
}

std::filesystem::path ConfigLoader::GetCertificatePath() const
{
	return m_config["Paths"]["CertificatePath"];
}

std::filesystem::path ConfigLoader::GetPrivateKeyPath() const
{
	return m_config["Paths"]["PrivateKeyPath"];
}

int32_t ConfigLoader::GetNumberOfThreadsInThreadPool() const 
{
	return m_config["Server"]["Threads"];
}

uint32_t ConfigLoader::GetMaxHeaderSize() const 
{
	return m_config["HttpLimits"]["LimitHttpRequestHeadersSymbols"];
}

uint32_t ConfigLoader::GetMaxBodySize() const 
{
	return m_config["HttpLimits"]["LimitHttpRequestBodySymbols"];
}

std::string ConfigLoader::GetSalt() const
{
	return m_config["Security"]["PasswordHashing"]["Salt"];
}

uint32_t ConfigLoader::GetNumberOfIterations() const
{
	return m_config["Security"]["PasswordHashing"]["Iterations"];
}

uint32_t ConfigLoader::GetHashLength() const
{
	return m_config["Security"]["PasswordHashing"]["HashLength"];
}

std::string ConfigLoader::GetJwtSecrete() const
{
	return m_config["Security"]["Jwt"]["Secret"];
}


std::chrono::seconds ConfigLoader::GetAccesTokenDuration() const
{
	return std::chrono::seconds(m_config["Security"]["Jwt"]["AccesTokenDuration"]);
}

std::chrono::seconds ConfigLoader::GetRefreshTokenDuration() const
{
	return std::chrono::seconds(m_config["Security"]["Jwt"]["RefreshTokenDuration"]);
}

#include "pch.h"
#include "ConfigLoader.h"
#include "Host.h"
#include "СonfigStructs.h"

#include "../SocketLayer/WinSockInitializer.h"
#include "../Logger/Logger.h"

int main() {
	
	std::unique_ptr<ConfigLoader> configLoader;
	
	try
	{
		configLoader = std::make_unique<ConfigLoader>();

		Logger::Instance().InizializePath(configLoader->GetLogFilePath());
	}
	catch (const std::exception& ex)
	{
		std::cerr << ex.what() << std::endl;

		return 1;
	}

	try
	{
		WinSockInitializer winSockInitializer;
		
 		ServerConfig serverConfig{ configLoader->GetPort(), configLoader->GetNumberOfThreadsInThreadPool() };
		PathsConfig pathsConfig{ configLoader->GetUsersDatabasePath(), configLoader->GetLogFilePath(), configLoader->GetCertificatePath(),
			configLoader->GetPrivateKeyPath() };
		PasswordHashingConfig passwordHashingConfig{ configLoader->GetSalt(), configLoader->GetNumberOfIterations(),
			configLoader->GetHashLength() };
		JwtConfig jwtConfig{ configLoader->GetJwtSecrete(), configLoader->GetAccesTokenDuration(), configLoader->GetRefreshTokenDuration() };

		HostConfig hostConfig{ std::move(serverConfig), std::move(pathsConfig), std::move(passwordHashingConfig),
			std::move(jwtConfig) };

		auto host = std::make_unique<Host>(hostConfig);

		host->Start();
		host->WaitUntilUsersDisconnect();
	}
	catch (const std::exception& ex)
	{
		Logger::Instance().Fatal(std::format("main exception: {}", ex.what()));
		return 1;
	}
	
	Logger::Instance().Info("The main function has completed its work");

	return 0;
}

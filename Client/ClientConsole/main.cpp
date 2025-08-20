#include "pch.h"

#include "../ClientLib/WinSockInitializer.h"
#include "../ClientLib/ClientConfig.h"
#include "../ClientLib/Connection.h"
#include "../ClientLib/SignalsHandler.h"
#include "../ClientLib/ClientServiceFacade.h"

#include "../ClientProtocolInterface/HttpResponseAssembler.h"
#include "../ClientProtocolInterface/HttpResponseCompletenessChecker.h"
#include "../ClientProtocolInterface/HttpResponseValidator.h"
#include "../ClientProtocolInterface/ClientProtocolFactory.h"
#include "../ClientProtocolInterface/ClientHttp.h"

#include "../Logger/Logger.h"

#include "../ConsoleUserInterface/ConsoleUserInterface.h"

#include "../ClientModel/ClientModelFacade.h"

int main() {

	std::unique_ptr<ClientConfig> clientConfig;

	try
	{
		clientConfig = std::make_unique<ClientConfig>();

		Logger::Instance().InizializePath(clientConfig->GetLogFilePath());
	}
	catch (const std::exception& ex)
	{
		std::cerr << ex.what() << std::endl;

		return 1;
	}

	try {

		WinSockInitializer winSockInitializer;
		
		auto connection = std::make_unique<Connection>(clientConfig->GetIP(),
			clientConfig->GetPort());

		auto httpResponseValidator = std::make_unique<HttpResponseValidator>();
		// todo: магические числа убрать
		auto httpResponseCompletenessChecker = std::make_unique<HttpResponseCompletenessChecker>(400, 10000);
		auto httpResponseAssembler = std::make_unique<HttpResponseAssembler>(*httpResponseCompletenessChecker);
		auto clientServiceFacade = std::make_unique<ClientServiceFacade>(*connection, *httpResponseAssembler, *httpResponseValidator);

		auto clientProtocolFactory = std::make_unique<ClientProtocolFactory>();
		auto clientProtocolHttp = clientProtocolFactory->CreateClientHttp();
		
		auto clientModelFacade = std::make_unique<ClientModelFacade>(*clientProtocolHttp, *clientServiceFacade);

		auto userInterface = std::make_shared<ConsoleUserInterface>(*clientModelFacade);

		SignalsHandler::Instance().SetStopUserSession(std::make_unique<std::function<void()>>(
			std::bind_front(&ConsoleUserInterface::Stop, userInterface.get())));

		SignalsHandler::Instance().SetCloseSocket(std::make_unique<std::function<void()>>(
			std::bind_front(&Connection::CloseSocket, connection.get())));

		userInterface->Run();

	}
	catch (const std::exception& ex) {
		Logger::Instance().Error(std::format("main exception: {}", ex.what()));
	}

	Logger::Instance().Info("The main function has completed its work");
	system("pause");
}

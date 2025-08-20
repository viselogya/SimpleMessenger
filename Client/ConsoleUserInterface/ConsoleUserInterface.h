#pragma once

#include "../ClientModel/ClientModelObserver.h"

class ClientModelFacade;

struct ChatMessageDTO;

namespace keywords {
	const std::string CREATE_CHAT = "CREATE";
	const std::string SHOW_CHATS = "SHOW";
	const std::string SELECT_CHAT = "SELECT";
	const std::string EXIT = "EXIT";
	const std::string ADD_USER_IN_CHAT = "ADD";
	const std::string OPTIONS = "OPTIONS";
};

class ConsoleUserInterface : public ClientModelObserver, public std::enable_shared_from_this<ConsoleUserInterface>
{
public:

	explicit ConsoleUserInterface(ClientModelFacade& clientModelFacade);

	void Run();
 
	void Stop();

	void OnNetworkError(const std::string& context) const override;

	void OnServerError(const std::string& context) const override;

	//virtual void OnNewMessage(const ChatMessageDTO& chatMessage) = 0;

	void OnSelectChat() const override;

private:

	std::string GetUserInput() const;

	void RouteUserInput(const std::string& input) const;

private:

	void ViewChats() const;

	void ViewChatMessage(const ChatMessageDTO& chatMessage) const;

	void ViewChatHistory() const;

	void ViewOptions() const;

private:

	void ControlMainMenu() const;

	void ControlLoginMenu() const;

	void ControlRegisterMenu() const;

	void ControlSelectChatMenu() const;

	void ControlCreateChatMenu() const;

	void ControlAddUserInChatMenu() const;

	void ControlExitChat() const;

	void ControlSendMessage(const std::string& message) const;

private:

	ClientModelFacade& m_clientModelFacade;

	bool isActive{ false };
};


#include "pch.h"
#include "ConsoleUserInterface.h"
#include "HelperFuncs.h"

#include "../ClientModel/ClientModelFacade.h"

#include "../DTO/DTO.h"


ConsoleUserInterface::ConsoleUserInterface(ClientModelFacade& clientModelFacade)
	:
	m_clientModelFacade(clientModelFacade)
{}


std::string ConsoleUserInterface::GetUserInput() const
{
	std::string	input;
	std::getline(std::cin, input);

	return input;
}


void ConsoleUserInterface::Stop()
{
	isActive = false;
}


void ConsoleUserInterface::Run()
{
	m_clientModelFacade.SetModelObserver(shared_from_this());

	isActive = true;

	while(isActive && !m_clientModelFacade.IsAuthorized()) {
		ControlMainMenu();
	}

	if (isActive) 
	{
		ViewOptions();
	}

	while (isActive) 
	{
		RouteUserInput(GetUserInput());
	}
}


void ConsoleUserInterface::RouteUserInput(const std::string& input) const
{
	// todo: можно применить паттерн State, но будет усложнение архитектуры
	if (input == keywords::CREATE_CHAT) {
		ControlCreateChatMenu();
	}
	else if (input == keywords::SHOW_CHATS) {
		ViewChats();
	}
	else if (input == keywords::SELECT_CHAT) {
		ControlSelectChatMenu();
	}
	else if (input == keywords::ADD_USER_IN_CHAT) {
		ControlAddUserInChatMenu();
	}
	else if (input == keywords::EXIT) {
		ControlExitChat();
	}
	else if(input == keywords::OPTIONS){
		ViewOptions();
	}
	else {
		ControlSendMessage(input);
	}
}


void ConsoleUserInterface::OnNetworkError(const std::string& context) const
{
	std::cout << std::format("NETWORK ERROR: {}\n", context);
}


void ConsoleUserInterface::OnServerError(const std::string& context) const
{
	std::cout << std::format("SERVER ERROR: {}\n", context);
}


void ConsoleUserInterface::OnSelectChat() const
{
	ViewChatHistory();
}


void ConsoleUserInterface::ControlMainMenu() const
{
	std::cout << "Select:\nRegister (write 1)\nLog in (write 2)\n";
	std::string choiceStr = GetUserInput();

	if (choiceStr == "1") {
		ControlRegisterMenu();
	}
	else if (choiceStr == "2") {
		ControlLoginMenu();
	}
	else {
		std::cout << "Incorrect input\n";
	}
}


void ConsoleUserInterface::ControlLoginMenu() const
{
	std::cout << "Enter your username and password\n";
	std::string username = GetUserInput();
	std::string passord = GetUserInput();
	
	m_clientModelFacade.Login(username, passord);
}


void ConsoleUserInterface::ControlRegisterMenu() const
{
	std::cout << "Enter your username and password\n";
	std::string username = GetUserInput();
	std::string passord = GetUserInput();

	m_clientModelFacade.Registration(username, passord);
}


void ConsoleUserInterface::ControlSelectChatMenu() const
{
	std::cout << "Enter chat number\n";
	std::string chatNumberStr = GetUserInput();

	if (!HelperFuncs::isNumber(chatNumberStr)) {
		std::cout << "Invalid number\n";

		return;
	}

	int64_t chatNum = std::stoll(chatNumberStr);

	m_clientModelFacade.SelectChat(chatNum);
}


void ConsoleUserInterface::ControlCreateChatMenu() const
{
	std::cout << "Enter chat name\n";
	std::string chatName = GetUserInput();

	m_clientModelFacade.CreateChat(chatName);
}


void ConsoleUserInterface::ControlAddUserInChatMenu() const
{
	std::cout << "Еnter the name of the user you want to add to the chat\n";
	std::string addedUsername = GetUserInput();

	m_clientModelFacade.AddUserInChat(m_clientModelFacade.GetCurrentChatId(), addedUsername);
}


void ConsoleUserInterface::ControlExitChat() const
{
	m_clientModelFacade.ExitChat();

	std::cout << "You left the chat (if you were in it)\n";
}


void ConsoleUserInterface::ControlSendMessage(const std::string& message) const
{
	m_clientModelFacade.SendMessage(m_clientModelFacade.GetCurrentChatId(), message);
}

void ConsoleUserInterface::ViewChats() const
{
	std::vector<ChatDTO> chatsInfo = m_clientModelFacade.GetChats();
	for (size_t i = 0; i < chatsInfo.size(); ++i) {
		std::cout << std::format("chat number: {}: chat name: {}\n", i + 1, chatsInfo[i].name);
	}
}


void ConsoleUserInterface::ViewChatMessage(const ChatMessageDTO& chatMessage) const
{
	std::cout << std::format("User: {0}\nmessage:{1}\ntime:{2:%F} {2:%R}\n", chatMessage.senderUserName,
		chatMessage.text, chatMessage.time);
}


void ConsoleUserInterface::ViewChatHistory() const
{
	std::vector<ChatMessageDTO> history = m_clientModelFacade.GetChatHistory(m_clientModelFacade.GetCurrentChatId());
	
	for (const auto& message : history) {
		ViewChatMessage(message);
	}
}


void ConsoleUserInterface::ViewOptions() const
{
	std::cout << "Commands for interacting with the system:\n";
	std::cout << std::format("Write \"{}\" to add a person to the chat\n", keywords::ADD_USER_IN_CHAT);
	std::cout << std::format("Write \"{}\" to show chats menu\n", keywords::SHOW_CHATS);
	std::cout << std::format("Write \"{}\" to select chat\n", keywords::SELECT_CHAT);
	std::cout << std::format("Write \"{}\" to exit chat\n", keywords::EXIT);
	std::cout << std::format("Write \"{}\" to create chat\n", keywords::CREATE_CHAT);
	std::cout << std::format("Write \"{}\" to show this(options) menu\n", keywords::OPTIONS);
	std::cout << std::endl;
	std::cout << "If you write something that is not on this list, it will be considered as a chat message\n";
}

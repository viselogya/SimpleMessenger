#pragma once

#include "UserData.h"
#include "UserSessionData.h"

class ClientProtocol;
class ClientServiceFacade;
class ClientModelObserver;

struct ChatDTO;
struct ChatMessageDTO;

class ClientModelFacade
{
public:

	explicit ClientModelFacade(ClientProtocol& clientProtocol, ClientServiceFacade& clientServiceFacade);

	void SetModelObserver(std::shared_ptr<ClientModelObserver> clientModelObserver);


	bool IsAuthorized() const;

	int64_t GetCurrentChatId() const;

	void ExitChat();

	void SelectChat(const int64_t chatId);


	void Login(const std::string& username, const std::string& password);

	void Registration(const std::string& username, const std::string& password);

	void AddUserInChat(const int64_t chatId, const std::string& addedUsername) const;

	void CreateChat(const std::string& chatname) const;

	void SendMessage(const int64_t chatId, const std::string& text) const;


	std::vector<ChatDTO> GetChats();

	std::vector<ChatMessageDTO> GetChatHistory(const int64_t chatId) const;

private:

	std::shared_ptr<ClientModelObserver> m_observer;

	ClientProtocol& m_clientProtocol;
	ClientServiceFacade& m_clientServiceFacade;

	UserData m_userData;
	UserSessionData m_userSessionData;
};


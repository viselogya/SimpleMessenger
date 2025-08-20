#pragma once

struct ChatDTO;
struct ChatMessageDTO;

class Chat;
class DatabaseManager;

class ChatManager
{
public:
	
	explicit ChatManager(DatabaseManager& databaseManager);

	std::vector<ChatDTO> GetUserChats(const int64_t userId) const;

	int64_t CreateChat(const int64_t userId, const std::string& chatName) const;

	std::vector<ChatMessageDTO> GetChatMessages(const int64_t chatId) const;

	void SendMessageToChat(const int64_t chatId, const int64_t userId, const std::string& text,
		const std::chrono::system_clock::time_point& time);

	bool AddUserInChat(const int64_t chatId, const int64_t addedUserId);

	bool IsMemberOfChat(const int64_t userId, const int64_t chatId) const;

private:

	DatabaseManager& m_databaseManager;
};

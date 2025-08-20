#pragma once

struct UserDto;
struct ChatDTO;
struct ChatMessageDTO;

class DatabaseManager {
public:
	
	virtual ~DatabaseManager() = default;

	virtual int64_t InsertUser(const std::string& username, const std::string& password) = 0;

	virtual std::optional<int64_t> GetUserId(const std::string& name) const = 0;

	virtual bool IsUserExists(const std::string& username) const = 0;

	virtual bool IsUserExists(const std::string& username, const std::string& password) const = 0;

	virtual std::vector<UserDto> GetAllUsers() const = 0;


	virtual int64_t InsertChat(const std::string& name) = 0;

	virtual std::vector<ChatDTO> GetUserChats(const int64_t user_id) const = 0;


	virtual void InsertUserChat(const int64_t user_id, const int64_t chat_id) = 0;


	virtual void InsertChatMessage(const int64_t chat_id, const int64_t senderUser_id, const std::string& text,
		const std::chrono::system_clock::time_point time) = 0;

	virtual std::vector<ChatMessageDTO> GetChatMessages(const int64_t chat_id) const = 0;


	virtual bool IsMemberOfChat(const int64_t userId, const int64_t chatId) const = 0;

};

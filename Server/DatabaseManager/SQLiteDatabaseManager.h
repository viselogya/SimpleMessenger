#pragma once

#include "DatabaseManager.h"

struct UserDto;
struct ChatDTO;
struct ChatMessageDTO;

struct sqlite3;
struct sqlite3_stmt;

class SQLiteDatabaseManager : public DatabaseManager
{
public:

	explicit SQLiteDatabaseManager(const std::filesystem::path& databasePath);

	~SQLiteDatabaseManager() override;

	int64_t InsertUser(const std::string& username, const std::string& password) override;

	std::optional<int64_t> GetUserId(const std::string& name) const override;

	bool IsUserExists(const std::string& username) const override;

	bool IsUserExists(const std::string& username, const std::string& password) const override;

	std::vector<UserDto> GetAllUsers() const override;


	int64_t InsertChat(const std::string& name) override;


	void InsertUserChat(const int64_t user_id, const int64_t chat_id) override;

	std::vector<ChatDTO> GetUserChats(const int64_t user_id) const override;


	void InsertChatMessage(const int64_t chat_id, const int64_t senderUser_id, const std::string& text,
		const std::chrono::system_clock::time_point time) override;

	std::vector<ChatMessageDTO> GetChatMessages(const int64_t chat_id) const override;


	bool IsMemberOfChat(const int64_t userId, const int64_t chatId) const override;

private:

	void FinalizeAllStatements();

	void CreateTables();

	void PrepareAllStatements();

	sqlite3_stmt* PrepareStatement(const char* sqlRequest);

private:

	mutable std::mutex m_mutexDatabaseManager;

	sqlite3* m_myMessengerDatabase{ nullptr };

	sqlite3_stmt* m_insertUser{ nullptr };
	sqlite3_stmt* m_getUserId{ nullptr };
	sqlite3_stmt* m_isUserExistsByName{ nullptr };
	sqlite3_stmt* m_IsUserExistsByNameAndPassword{ nullptr };
	sqlite3_stmt* m_insertChat{ nullptr };
	sqlite3_stmt* m_getAllUsers{ nullptr };
	sqlite3_stmt* m_insertUserChat{ nullptr };
	sqlite3_stmt* m_getUserChats{ nullptr };
	sqlite3_stmt* m_insertChatMessage{ nullptr };
	sqlite3_stmt* m_getChatMessages{ nullptr };
	sqlite3_stmt* m_isMemberOfChat{ nullptr };
};

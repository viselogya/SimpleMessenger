#include "pch.h"
#include "SQLiteDatabaseManager.h"
#include "DatabaseExceptions.h"

#include "../DTO/DTO.h"

#include "../Logger/Logger.h"

SQLiteDatabaseManager::SQLiteDatabaseManager(const std::filesystem::path& databasePath) {
	if (sqlite3_open(databasePath.string().data(), &m_myMessengerDatabase) == SQLITE_OK) {
		Logger::Instance().Debug("Connection to UsersDatabase established");
	}
	else {
		sqlite3_close(m_myMessengerDatabase);

		throw DatabaseException(std::format("Error with open MyMenegerDatabase: {}", sqlite3_errmsg(m_myMessengerDatabase)));
	}

	CreateTables();
	PrepareAllStatements();

	Logger::Instance().Debug("DatabaseManager was opened/created correctly");
}

SQLiteDatabaseManager::~SQLiteDatabaseManager() {
	FinalizeAllStatements();
	sqlite3_close(m_myMessengerDatabase);
}

void SQLiteDatabaseManager::FinalizeAllStatements() {
	sqlite3_finalize(m_insertUser);
	sqlite3_finalize(m_getUserId);
	sqlite3_finalize(m_isUserExistsByName);
	sqlite3_finalize(m_IsUserExistsByNameAndPassword);
	sqlite3_finalize(m_insertChat);
	sqlite3_finalize(m_insertUserChat);
	sqlite3_finalize(m_getUserChats);
	sqlite3_finalize(m_insertChatMessage);
	sqlite3_finalize(m_getChatMessages);
}

void SQLiteDatabaseManager::CreateTables() {
	char* error;

	if (sqlite3_exec(m_myMessengerDatabase, "PRAGMA foreign_keys = ON;", nullptr, nullptr, &error) != SQLITE_OK) {

		std::string errMsg = error;

		sqlite3_free(error);
		sqlite3_close(m_myMessengerDatabase);

		throw DatabaseException(std::format("sqlite3_exec error with enable PRAMG foreign_keys = ON: {}", errMsg));
	}

	const char* createTables = R"(
		CREATE TABLE IF NOT EXISTS Users(
			id				INTEGER	PRIMARY KEY AUTOINCREMENT,
			name			TEXT NOT NULL,
			password		TEXT
		);

		CREATE TABLE IF NOT EXISTS Chats(
			id				INTEGER PRIMARY KEY AUTOINCREMENT,
			name			TEXT
		);

		CREATE TABLE IF NOT EXISTS UsersChats(
			user_id			INTEGER NOT NULL,
			chat_id			INTEGER NOT NULL,
			FOREIGN KEY (user_id) REFERENCES Users(id),
			FOREIGN KEY (chat_id) REFERENCES Chats(id)
		);

		CREATE TABLE IF NOT EXISTS ChatsMessages (
			id              INTEGER PRIMARY KEY AUTOINCREMENT,
			chat_id         INTEGER NOT NULL,
			senderUser_id   INTEGER NOT NULL,
			text			TEXT NOT NULL,
			time			DATETIME NOT NULL,
			FOREIGN KEY (chat_id) REFERENCES Chats(id),
			FOREIGN KEY (senderUser_id) REFERENCES Users(id)
		);
	)";

	if (sqlite3_exec(m_myMessengerDatabase, createTables, nullptr, nullptr, &error) == SQLITE_ERROR) {
		std::string errMsg = error;

		sqlite3_free(error);
		sqlite3_close(m_myMessengerDatabase);


		throw DatabaseException(std::format("sqlite3_exec error with create tables: {}", errMsg));
	}
}

sqlite3_stmt* SQLiteDatabaseManager::PrepareStatement(const char* sqlRequest) {
	sqlite3_stmt* result = nullptr;
	
	if (sqlite3_prepare_v2(m_myMessengerDatabase, sqlRequest, -1, &result, nullptr) != SQLITE_OK) {
		throw DatabaseException(std::format("Error while preparing statement in SqliteDatabaseManager! GLE: {}",
			sqlite3_errmsg(m_myMessengerDatabase)));
	}

	return result;
}

void SQLiteDatabaseManager::PrepareAllStatements() {
	const char* insertUserSqlRequest = "INSERT INTO Users(name, password) VALUES(?, ?);";
	m_insertUser = PrepareStatement(insertUserSqlRequest);

	const char* getUserIdsqlRequest = "SELECT id FROM Users WHERE name = ?;";
	m_getUserId = PrepareStatement(getUserIdsqlRequest);

	const char* isUserExistsByNameSqlRequest = "SELECT EXISTS(SELECT 1 FROM Users WHERE name = ?);";
	m_isUserExistsByName = PrepareStatement(isUserExistsByNameSqlRequest);

	const char* isUserExistsByNameAndPasswordSqlRequest = "SELECT EXISTS(SELECT 1 FROM Users WHERE name = ? AND password = ?);";
	m_IsUserExistsByNameAndPassword = PrepareStatement(isUserExistsByNameAndPasswordSqlRequest);

	const char* insertChatSqlRequest = "INSERT INTO Chats(name) VALUES(?);";
	m_insertChat = PrepareStatement(insertChatSqlRequest);

	const char* getAllUserSqlRequest = "SELECT id, name FROM Users;";
	m_getAllUsers = PrepareStatement(getAllUserSqlRequest);

	const char* insertUserChatSqlRequest = "INSERT INTO UsersChats(user_id, chat_id) VALUES(?, ?);";
	m_insertUserChat = PrepareStatement(insertUserChatSqlRequest);

	const char* getUserChatsSqlRequest = R"(
		SELECT Chats.id, Chats.name
		FROM UsersChats
		INNER JOIN Chats
		ON UsersChats.chat_id = Chats.id
		WHERE user_id = ?;
	)";
	m_getUserChats = PrepareStatement(getUserChatsSqlRequest);

	const char* insertChatMessageSqlRequest = "INSERT INTO ChatsMessages(chat_id, senderUser_id, text, time) VALUES(?, ?, ?, ?);";
	m_insertChatMessage = PrepareStatement(insertChatMessageSqlRequest);

	const char* getChatMessagesSqlRequest = R"(
		SELECT Users.id, Users.name, text, time
		FROM ChatsMessages
		INNER JOIN Users
		ON ChatsMessages.senderUser_id = Users.id
		WHERE chat_id = ?;
	)";
	m_getChatMessages = PrepareStatement(getChatMessagesSqlRequest);

	const char* isMemberOfChatSqlRequest = "SELECT EXISTS(SELECT 1 FROM UsersChats WHERE user_id = ? AND chat_id = ?);";
	m_isMemberOfChat = PrepareStatement(isMemberOfChatSqlRequest);
}

int64_t SQLiteDatabaseManager::InsertUser(const std::string& username, const std::string& password) {
	std::scoped_lock lock(m_mutexDatabaseManager);

	sqlite3_bind_text(m_insertUser, 1, username.data(), static_cast<int>(username.size()), SQLITE_STATIC);
	sqlite3_bind_text(m_insertUser, 2, password.data(), static_cast<int>(password.size()), SQLITE_STATIC);

	if (sqlite3_step(m_insertUser) != SQLITE_DONE) {
		throw DatabaseException(std::format("Error in InsertUser with step in database! GLE: {}", sqlite3_errmsg(m_myMessengerDatabase)));
	}

	sqlite3_int64 id = sqlite3_last_insert_rowid(m_myMessengerDatabase);

	sqlite3_reset(m_insertUser);
	sqlite3_clear_bindings(m_insertUser);

	return static_cast<size_t>(id);
}

std::optional<int64_t> SQLiteDatabaseManager::GetUserId(const std::string& name) const {
	std::scoped_lock lock(m_mutexDatabaseManager);

	std::optional<int64_t> id = std::nullopt;
	sqlite3_bind_text(m_getUserId, 1, name.data(), static_cast<int>(name.size()), SQLITE_STATIC);
		
	if (sqlite3_step(m_getUserId) == SQLITE_ROW) {
		id = sqlite3_column_int(m_getUserId, 0);
	}
	else {
		throw DatabaseException(std::format("Error in GetUserId with step in database! GLE: {}", sqlite3_errmsg(m_myMessengerDatabase)));
	}

	sqlite3_reset(m_getUserId);
	sqlite3_clear_bindings(m_getUserId);

	return id;
}


bool SQLiteDatabaseManager::IsUserExists(const std::string& username) const {
	std::scoped_lock lock(m_mutexDatabaseManager);

	sqlite3_bind_text(m_isUserExistsByName, 1, username.data(), static_cast<int>(username.size()), SQLITE_STATIC);
	int step = sqlite3_step(m_isUserExistsByName);
	bool isExists = false;

	if (step == SQLITE_ROW) {
		isExists = (sqlite3_column_int(m_isUserExistsByName, 0) == 1);
	}
	else {
		throw DatabaseException(std::format("Error in IsUserExists(by name) with step in database! GLE: {}", sqlite3_errmsg(m_myMessengerDatabase)));
	}

	sqlite3_reset(m_isUserExistsByName);
	sqlite3_clear_bindings(m_isUserExistsByName);

	return isExists;
}

bool SQLiteDatabaseManager::IsUserExists(const std::string& username, const std::string& password) const {
	std::scoped_lock lock(m_mutexDatabaseManager);

	sqlite3_bind_text(m_IsUserExistsByNameAndPassword, 1, username.data(), static_cast<int>(username.size()), SQLITE_STATIC);
	sqlite3_bind_text(m_IsUserExistsByNameAndPassword, 2, password.data(), static_cast<int>(password.size()), SQLITE_STATIC);
	bool isExists = false;

	if (sqlite3_step(m_IsUserExistsByNameAndPassword) == SQLITE_ROW) {
		isExists = (sqlite3_column_int(m_IsUserExistsByNameAndPassword, 0) == 1);
	}
	else {
		throw DatabaseException(std::format("SQL IsUserExists(by name and password) error: {}", sqlite3_errmsg(m_myMessengerDatabase)));
	}

	sqlite3_reset(m_IsUserExistsByNameAndPassword);
	sqlite3_clear_bindings(m_IsUserExistsByNameAndPassword);

	return isExists;
}

std::vector<UserDto> SQLiteDatabaseManager::GetAllUsers() const
{
	std::scoped_lock lock(m_mutexDatabaseManager);

	std::vector<UserDto> users;
	while (sqlite3_step(m_getAllUsers) == SQLITE_ROW) {
		int64_t userId = sqlite3_column_int64(m_getAllUsers, 0);
		std::string username = reinterpret_cast<const char*>(sqlite3_column_text(m_getAllUsers, 1));
	
		users.emplace_back(userId, username);
	}

	return users;
}


int64_t SQLiteDatabaseManager::InsertChat(const std::string& name) {
	std::scoped_lock lock(m_mutexDatabaseManager);

	sqlite3_bind_text(m_insertChat, 1, name.data(), static_cast<int>(name.size()), SQLITE_STATIC);

	if (sqlite3_step(m_insertChat) != SQLITE_DONE) {
		throw DatabaseException(std::format("Error in InsertChat in database: {}", sqlite3_errmsg(m_myMessengerDatabase)));
	}

	sqlite3_int64 id = sqlite3_last_insert_rowid(m_myMessengerDatabase);

	sqlite3_reset(m_insertChat);
	sqlite3_clear_bindings(m_insertChat);

	return static_cast<size_t>(id);
}

void SQLiteDatabaseManager::InsertUserChat(const int64_t user_id, const int64_t chat_id) {
	std::scoped_lock lock(m_mutexDatabaseManager);

	sqlite3_bind_int64(m_insertUserChat, 1, user_id);
	sqlite3_bind_int64(m_insertUserChat, 2, chat_id);

	if (sqlite3_step(m_insertUserChat) != SQLITE_DONE) {
		throw DatabaseException(std::format("Error in InsertUserChat in database: {}", sqlite3_errmsg(m_myMessengerDatabase)));
	}

	sqlite3_reset(m_insertUserChat);
	sqlite3_clear_bindings(m_insertUserChat);
}


std::vector<ChatDTO> SQLiteDatabaseManager::GetUserChats(const int64_t user_id) const {
	std::scoped_lock lock(m_mutexDatabaseManager);

	std::vector<ChatDTO> chats;
	sqlite3_bind_int64(m_getUserChats, 1, user_id);

	while (sqlite3_step(m_getUserChats) == SQLITE_ROW) {
		int64_t chat_id = sqlite3_column_int64(m_getUserChats, 0);
		std::string chatName = reinterpret_cast<const char*>(sqlite3_column_text(m_getUserChats, 1));

		chats.emplace_back(chat_id, chatName);
	}

	sqlite3_reset(m_getUserChats);
	sqlite3_clear_bindings(m_getUserChats);

	return chats;
}

void SQLiteDatabaseManager::InsertChatMessage(const int64_t chat_id, const int64_t senderUser_id, const std::string& text,
	const std::chrono::system_clock::time_point time)
{
	std::scoped_lock lock(m_mutexDatabaseManager);

	sqlite3_bind_int64(m_insertChatMessage, 1, chat_id);
	sqlite3_bind_int64(m_insertChatMessage, 2, senderUser_id);
	sqlite3_bind_text(m_insertChatMessage, 3, text.data(), static_cast<int>(text.size()), SQLITE_STATIC);
	std::string timeFormat = std::format("{0:%F} {0:%R}", time);
	sqlite3_bind_text(m_insertChatMessage, 4, timeFormat.data(), static_cast<int>(timeFormat.size()), SQLITE_STATIC);

	if (sqlite3_step(m_insertChatMessage) != SQLITE_DONE) {
		throw DatabaseException(std::format("Error in InsertChatMessage in database: {}", sqlite3_errmsg(m_myMessengerDatabase)));
	}

	sqlite3_reset(m_insertChatMessage);
	sqlite3_clear_bindings(m_insertChatMessage);
}

std::vector<ChatMessageDTO> SQLiteDatabaseManager::GetChatMessages(const int64_t chat_id) const {
	std::scoped_lock lock(m_mutexDatabaseManager);

	std::vector<ChatMessageDTO> chatMessages;
	sqlite3_bind_int64(m_getChatMessages, 1, chat_id);

	while (sqlite3_step(m_getChatMessages) == SQLITE_ROW)
	{
		int64_t user_id = sqlite3_column_int64(m_getChatMessages, 0);
		std::string userName = reinterpret_cast<const char*>(sqlite3_column_text(m_getChatMessages, 1));
		std::string text = reinterpret_cast<const char*>(sqlite3_column_text(m_getChatMessages, 2));
		std::string timeStr = reinterpret_cast<const char*>(sqlite3_column_text(m_getChatMessages, 3));

		std::stringstream stream(timeStr);
		std::chrono::system_clock::time_point time;
			
		stream >> std::chrono::parse("%F %R", time);

		chatMessages.emplace_back(user_id, userName, text, time);
	}

	sqlite3_reset(m_getChatMessages);
	sqlite3_clear_bindings(m_getChatMessages);

	return chatMessages;
}


bool SQLiteDatabaseManager::IsMemberOfChat(const int64_t userId, const int64_t chatId) const
{
	std::scoped_lock lock(m_mutexDatabaseManager);

	sqlite3_bind_int64(m_isMemberOfChat, 1, userId);
	sqlite3_bind_int64(m_isMemberOfChat, 2, chatId);
	bool isMemberOfChat = false;

	if (sqlite3_step(m_isMemberOfChat) == SQLITE_ROW) {
		isMemberOfChat = (sqlite3_column_int(m_isMemberOfChat, 0) == 1);
	}
	else {
		throw DatabaseException(std::format("SQL IsUserExists(by name and password) error: {}", sqlite3_errmsg(m_myMessengerDatabase)));
	}

	sqlite3_reset(m_isMemberOfChat);
	sqlite3_clear_bindings(m_isMemberOfChat);

	return isMemberOfChat;
}

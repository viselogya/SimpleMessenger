#include "pch.h"
#include "ChatManager.h"

#include "../DTO/DTO.h"

#include "../DatabaseManager/DatabaseManager.h"

ChatManager::ChatManager(DatabaseManager& databaseManager)
	:
	m_databaseManager(databaseManager)
{}


std::vector<ChatDTO> ChatManager::GetUserChats(const int64_t userId) const {
	return m_databaseManager.GetUserChats(userId);
}


int64_t ChatManager::CreateChat(const int64_t userId, const std::string& chatName) const {
	int64_t chatId = m_databaseManager.InsertChat(chatName);
	m_databaseManager.InsertUserChat(userId, chatId);

	return chatId;
}


std::vector<ChatMessageDTO> ChatManager::GetChatMessages(const int64_t chatId) const {
	return m_databaseManager.GetChatMessages(chatId);
}


void ChatManager::SendMessageToChat(const int64_t chatId, const int64_t userId, const std::string& text,
	const std::chrono::system_clock::time_point& time)
{
	m_databaseManager.InsertChatMessage(chatId, userId, text, time);
}


// todo: а что если такого addedUserId нет?
bool ChatManager::AddUserInChat(const int64_t chatId, const int64_t addedUserId)
{
	m_databaseManager.InsertUserChat(addedUserId, chatId);
	return true;
}


bool ChatManager::IsMemberOfChat(const int64_t userId, const int64_t chatId) const
{
	return m_databaseManager.IsMemberOfChat(userId, chatId);
}

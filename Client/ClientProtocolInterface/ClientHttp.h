#pragma once

#include "ClientProtocol.h"

class ClientHttp : public ClientProtocol
{
public:

	~ClientHttp() override = default;

	Http::Request GetRegistrationUserRequest(const std::string& userName, const std::string& password) const override;

	Http::Request GetLoginUserRequest(const std::string& userName, const std::string& password) const override;

	Http::Request GetCreateChatRequest(const std::string& chatName, const int64_t userId) const override;

	Http::Request GetChatsRequest(const int64_t userId) const override;

	Http::Request GetChatHistoryRequest(const int64_t chatId) const override;

	Http::Request GetSendMessageToChatRequest(const int64_t chatId, const int64_t userId, const std::string& text,
		const std::chrono::system_clock::time_point& time) const override;

	Http::Request GetAddUserInChatRequest(const int64_t chatId, const std::string& additionalUserName) const override;
};

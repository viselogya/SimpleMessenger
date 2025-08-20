#pragma once

namespace Http {
	class Request;
	class Response;
}

class ClientProtocol
{
public:

	virtual ~ClientProtocol() = default;

	virtual Http::Request GetRegistrationUserRequest(const std::string& userName, const std::string& password) const = 0;

	virtual Http::Request GetLoginUserRequest(const std::string& userName, const std::string& password) const = 0;

	virtual Http::Request GetCreateChatRequest(const std::string& chatName, const int64_t userId) const = 0;

	virtual Http::Request GetChatsRequest(const int64_t userId) const = 0;

	virtual Http::Request GetChatHistoryRequest(const int64_t chatId) const = 0;

	virtual Http::Request GetSendMessageToChatRequest(const int64_t chatId, const int64_t userId, const std::string& text,
		const std::chrono::system_clock::time_point& time) const = 0;

	virtual Http::Request GetAddUserInChatRequest(const int64_t chatId, const std::string& additionalUserName) const = 0;
};

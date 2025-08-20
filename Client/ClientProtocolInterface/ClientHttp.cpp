#include "pch.h"
#include "ClientHttp.h"

#include "../Http/Request.h"

using namespace nlohmann;
using Http::Request;

Http::Request ClientHttp::GetRegistrationUserRequest(const std::string& userName, const std::string& password) const {
	Request registrationUserRequest;

	registrationUserRequest.SetMethod(Http::Method::POST);
	registrationUserRequest.SetUrl("/registration");
	registrationUserRequest.SetHeader("Content-Type", "application/json");

	json body;
	body["username"] = userName;
	body["password"] = password;

	registrationUserRequest.SetBody(body.dump());

	return registrationUserRequest;
}


Http::Request ClientHttp::GetLoginUserRequest(const std::string& userName, const std::string& password) const {
	Request loginUserRequest;

	loginUserRequest.SetMethod(Http::Method::POST);
	loginUserRequest.SetUrl("/login");
	loginUserRequest.SetHeader("Content-Type", "application/json");

	json body;
	body["username"] = userName;
	body["password"] = password;

	loginUserRequest.SetBody(body.dump());

	return loginUserRequest;
}


Http::Request ClientHttp::GetCreateChatRequest(const std::string& chatName, const int64_t userId) const {
	Request createChatRequest;

	createChatRequest.SetMethod(Http::Method::POST);

	createChatRequest.SetUrl("/create_chat");
	createChatRequest.SetHeader("Content-Type","application/json");


	json body;
	body["user_id"] = userId;
	body["chat_name"] = chatName;

	createChatRequest.SetBody(body.dump());

	return createChatRequest;
}


Http::Request ClientHttp::GetChatsRequest(const int64_t userId) const {
	Request showChats;

	showChats.SetMethod(Http::Method::GET);
	showChats.SetUrl("/chats");
	showChats.SetHeader("Content-Type", "application/json");

	json body;
	body["user_id"] = userId;
	
	showChats.SetBody(body.dump());

	return showChats;
}


Http::Request ClientHttp::GetChatHistoryRequest(const int64_t chatId) const {
	Request selectChatRequest;

	selectChatRequest.SetMethod(Http::Method::GET);
	selectChatRequest.SetUrl("/chats/messages");
	selectChatRequest.SetHeader("Content-Type", "application/json");

	json body;
	body["chat_id"] = chatId;

	selectChatRequest.SetBody(body.dump());

	return selectChatRequest;
}


Http::Request ClientHttp::GetSendMessageToChatRequest(const int64_t chatId, const int64_t userId, const std::string& text,
	const std::chrono::system_clock::time_point& time) const
{
	Request sendMessageToChatRequest;
	sendMessageToChatRequest.SetMethod(Http::Method::POST);
	sendMessageToChatRequest.SetUrl("/chats/messages/send_message");
	sendMessageToChatRequest.SetHeader("Content-Type", "application/json");

	json body;
	body["chat_id"] = chatId;
	body["user_id"] = userId;
	body["text"] = text;
	body["time"] = std::format("{0:%F} {0:%R}", time);

	sendMessageToChatRequest.SetBody(body.dump());

	return sendMessageToChatRequest;
}

Http::Request ClientHttp::GetAddUserInChatRequest(const int64_t chatId, const std::string& additionalUserName) const {
	Request request;

	request.SetMethod(Http::Method::POST);
	request.SetUrl("/chats/add_user");
	request.SetHeader("Content-Type", "application/json");

	json body;
	body["chat_id"] = chatId;
	body["username"] = additionalUserName;

	request.SetBody(body.dump());

	return request;
}

#include "pch.h"
#include "ServerHttp.h"

#include "../ServerLogic/UserAuthentication.h"
#include "../ServerLogic/UserRegistration.h"
#include "../ServerLogic/ChatManager.h"

#include "../DatabaseManager/DatabaseManager.h"

#include "../DTO/DTO.h"

using namespace nlohmann;

namespace http = boost::beast::http;

template<typename T>
std::string ServerHttp::ResponseToString(const http::response<T>& response) const
{
	return boost::lexical_cast<std::string>(response);
}

std::string ServerHttp::OkForOptions() const
{
	// todo: нужно будет доделать с access_control_allow_origin
	http::response<http::empty_body> response;
	response.version(11);
	response.result(http::status::ok);
	response.set(http::field::access_control_allow_origin, "*");
	response.set(http::field::access_control_allow_methods, "POST, GET");
	response.set(http::field::access_control_allow_headers, "Content-Type, Authorization");

	response.prepare_payload();
	return ResponseToString(response);
}


std::string ServerHttp::MakeLoginResponse(const std::string& accessToken, const std::string& refreshToken) const
{
	http::response<http::string_body> response;
	response.version(11);
	response.result(http::status::ok);
	response.set(http::field::content_type, "application/json");
	response.set(http::field::access_control_allow_origin, "*");
	response.set(http::field::connection, "keep-alive");
	std::string cookie = std::format("refresh_token={}; Path=/; HttpOnly; Secure; SameSite=None; Mag-Age={}", refreshToken, 604800);
	response.set(http::field::set_cookie, cookie);

	json responseBody;
	responseBody["access_token"] = accessToken;
	responseBody["token_type"] = "Bearer";

	response.body() = responseBody.dump();
	response.prepare_payload();

	return ResponseToString(response);
}


std::string ServerHttp::MakeCreateChatResponse(const int64_t chatId, const std::string& chatName) const
{
	http::response<http::string_body> response;

	response.version(11);
	response.result(http::status::created);
	response.set(http::field::access_control_allow_origin, "*");
	response.set(http::field::connection, "keep-alive");

	json responseBody;
	responseBody["chat_id"] = chatId;
	responseBody["chat_name"] = chatName;

	response.body() = responseBody.dump();
	response.prepare_payload();

	return ResponseToString(response);
}


std::string ServerHttp::MakeUserChatListResponse(const std::vector<ChatDTO>& chats) const
{
	http::response<http::string_body> response;
	response.version(11);
	response.result(http::status::ok);
	response.set(http::field::content_type, "application/json");
	response.set(http::field::access_control_allow_origin, "*");
	response.set(http::field::connection, "keep-alive");

	json responseBody;
	responseBody["chats"] = nlohmann::json::array();
	for (const auto& chat : chats) {
		responseBody["chats"].push_back({ {"id", chat.id }, { "name", chat.name} });
	}

	response.body() = responseBody.dump();
	response.prepare_payload();

	return ResponseToString(response);
}


std::string ServerHttp::MakeChatHistoryResponse(const std::vector<ChatMessageDTO>& messages) const
{
	http::response<http::string_body> response;
	response.version(11);
	response.result(http::status::ok);
	response.set(http::field::content_type, "application/json");
	response.set(http::field::access_control_allow_origin, "*");
	response.set(http::field::connection, "keep-alive");

	json responseBody;

	responseBody["messages"] = nlohmann::json::array();

	for (const auto& message : messages) {
		std::string messageTime = std::format("{0:%F} {0:%R}", message.time);

		responseBody["messages"].push_back({
			{ "user_id", message.senderUser_id },
			{ "username", message.senderUserName },
			{ "text", message.text },
			{ "time", messageTime }
			});
	}

	response.body() = responseBody.dump();
	response.prepare_payload();

	return ResponseToString(response);
}


std::string ServerHttp::MakeUserListResponse(const std::vector<UserDto>& users) const
{
	http::response<http::string_body> response;
	response.version(11);
	response.result(http::status::ok);
	response.set(http::field::access_control_allow_origin, "*");
	response.set(http::field::connection, "keep-alive");

	json responseBody;
	responseBody["users"] = json::array();
	
	for (const auto& user : users) {
		responseBody["users"].push_back({
			{ "user_id", user.userId },
			{ "username", user.username }
			});
	}

	response.body() = responseBody.dump();
	response.prepare_payload();

	return ResponseToString(response);
}


std::string ServerHttp::Ok() const
{
	http::response<http::empty_body> response;
	response.version(11);
	response.result(http::status::ok);
	response.set(http::field::access_control_allow_origin, "*");
	response.set(http::field::connection, "keep-alive");
	response.prepare_payload();

	return ResponseToString(response);
}

std::string ServerHttp::BadRequest() const
{
	http::response<http::empty_body> response;
	response.version(11);
	response.result(http::status::bad_request);
	response.set(http::field::access_control_allow_origin, "*");
	response.set(http::field::connection, "keep-alive");
	response.prepare_payload();

	return ResponseToString(response);
}


std::string ServerHttp::NotFound() const
{
	http::response<http::empty_body> response;
	response.version(11);
	response.result(http::status::not_found);
	response.set(http::field::access_control_allow_origin, "*");
	response.set(http::field::connection, "keep-alive");
	response.prepare_payload();

	return ResponseToString(response);
}


std::string ServerHttp::ContentTooLarge() const
{
	http::response<http::empty_body> response;
	response.version(11);
	response.result(http::status::payload_too_large);
	response.set(http::field::access_control_allow_origin, "*");
	response.set(http::field::connection, "keep-alive");
	response.prepare_payload();

	return ResponseToString(response);
}

std::string ServerHttp::Unauthorized() const
{
	http::response<http::empty_body> response;
	response.version(11);
	response.result(http::status::unauthorized);
	response.set(http::field::access_control_allow_origin, "*");
	response.set(http::field::connection, "keep-alive");
	response.prepare_payload();

	return ResponseToString(response);
}

std::string ServerHttp::Forbidden() const
{
	http::response<http::empty_body> response;
	response.version(11);
	response.result(http::status::forbidden);
	response.set(http::field::access_control_allow_origin, "*");
	response.set(http::field::connection, "keep-alive");
	response.prepare_payload();

	return ResponseToString(response);
}

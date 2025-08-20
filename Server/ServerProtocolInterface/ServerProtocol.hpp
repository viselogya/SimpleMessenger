#pragma once

struct ChatDTO;
struct ChatMessageDTO;
struct UserDto;

namespace Http {
	class Response;
}

class ServerProtocol
{
public:

	virtual ~ServerProtocol() = default;

	virtual std::string MakeLoginResponse(const std::string& accesToken, const std::string& refreshToken) const = 0;

	virtual std::string MakeCreateChatResponse(const int64_t chatId, const std::string& chatName) const = 0;

	virtual std::string MakeUserChatListResponse(const std::vector<ChatDTO>& chats) const = 0;

	virtual std::string MakeChatHistoryResponse(const std::vector<ChatMessageDTO>& messages) const = 0;

	virtual std::string MakeUserListResponse(const std::vector<UserDto>& users) const = 0;

	virtual std::string OkForOptions() const = 0;

	virtual std::string Ok() const = 0;

	virtual std::string BadRequest() const = 0;

	virtual std::string NotFound() const = 0;

	virtual std::string ContentTooLarge() const = 0;

	virtual std::string Unauthorized() const = 0;

	virtual std::string Forbidden() const = 0;
};

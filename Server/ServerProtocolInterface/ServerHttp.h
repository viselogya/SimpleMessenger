#pragma once

#include "ServerProtocol.hpp"

class ServerHttp : public ServerProtocol
{
public:

	~ServerHttp() override = default;

	std::string MakeLoginResponse(const std::string& accesToken, const std::string& refreshToken) const override;

	std::string MakeCreateChatResponse(const int64_t chatId, const std::string& chatName) const override;

	std::string MakeUserChatListResponse(const std::vector<ChatDTO>& chats) const override;

	std::string MakeChatHistoryResponse(const std::vector<ChatMessageDTO>& messages) const override;

	std::string MakeUserListResponse(const std::vector<UserDto>& users) const override;

	std::string OkForOptions() const override;

	std::string Ok() const override;

	std::string BadRequest() const override;

	std::string NotFound() const override;

	std::string ContentTooLarge() const override;

	std::string Unauthorized() const override;

	std::string Forbidden() const override;

private:

	template<typename T>
	std::string ResponseToString(const boost::beast::http::response<T>& response) const;

};

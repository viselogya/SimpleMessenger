#pragma once

#include "../DTO/DTO.h"

struct UserSessionData
{

	int64_t currentChatId{ -1 };
	std::vector<ChatDTO> chats;

};

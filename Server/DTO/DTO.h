#pragma once


struct UserDto
{
	int64_t userId;
	std::string username;
};


struct ChatDTO 
{
	int64_t id;
	std::string name;

	bool operator==(const ChatDTO& other) const {
		return id == other.id;
	}

	struct HashFunction
	{
		size_t operator()(const ChatDTO& self) const
		{
			return self.id;
		}
	};
};


struct ChatMessageDTO
{
	int64_t senderUser_id;
	std::string senderUserName;
	std::string text;
	std::chrono::system_clock::time_point time;
};

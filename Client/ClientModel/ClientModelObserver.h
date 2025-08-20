#pragma once

//struct ChatMessageDTO;

class ClientModelObserver
{
public:

	virtual ~ClientModelObserver() = default;

	
	virtual void OnNetworkError(const std::string& context) const = 0;

	virtual void OnServerError(const std::string& context) const = 0;

	//virtual void OnNewMessage(const ChatMessageDTO& chatMessage) = 0;

	virtual void OnSelectChat() const = 0;

};


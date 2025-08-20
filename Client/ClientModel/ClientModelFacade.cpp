#include "pch.h"
#include "ClientModelFacade.h"
#include "ClientModelObserver.h"
#include "HelperFuncs.h"

#include "../DTO/DTO.h"

#include "../ClientProtocolInterface/ClientProtocol.h"

#include "../ClientLib/ClientServiceFacade.h"

#include "../Http/Request.h"
#include "../Http/Response.h"

using namespace nlohmann;

ClientModelFacade::ClientModelFacade(ClientProtocol& clientProtocol, ClientServiceFacade& clientServiceFacade)
	:
	m_clientProtocol(clientProtocol),
	m_clientServiceFacade(clientServiceFacade),
	m_userData(UserData()),
	m_userSessionData(UserSessionData())
{}


void ClientModelFacade::SetModelObserver(std::shared_ptr<ClientModelObserver> clientModelObserver)
{
	m_observer = std::move(clientModelObserver);
}


bool ClientModelFacade::IsAuthorized() const
{
	return m_userData.userId != -1;
}


int64_t ClientModelFacade::GetCurrentChatId() const
{
	return m_userSessionData.currentChatId;
}


void ClientModelFacade::ExitChat()
{
	m_userSessionData.currentChatId = -1;
}


void ClientModelFacade::SelectChat(const int64_t chatId)
{
	m_userSessionData.currentChatId = chatId;

	m_observer->OnSelectChat();
}


void ClientModelFacade::Login(const std::string& username, const std::string& password)
{
	std::optional<Http::Response> httpResponse = m_clientServiceFacade.PerformRequest(
		m_clientProtocol.GetLoginUserRequest(username, password));

	if (!httpResponse.has_value()) {
		m_observer->OnNetworkError("connection problems");

		return;
	}

	if (httpResponse->GetStatusCode() != 200 || httpResponse->GetHeader("Content-Type") != "application/json") {
		m_observer->OnServerError(std::format("{} {}", httpResponse->GetStatusCode(), httpResponse->GetStatusText()));

		return;
	}

	json body = json::parse(httpResponse->GetBody());

	m_userData.userId = body["user_id"];
	m_userData.username = username;
}


void ClientModelFacade::Registration(const std::string& username, const std::string& password)
{
	std::optional<Http::Response> httpResponse = m_clientServiceFacade.PerformRequest(
		m_clientProtocol.GetRegistrationUserRequest(username, password));

	if (!httpResponse.has_value()) {
		m_observer->OnNetworkError("connection problems");

		return;
	}

	if (httpResponse->GetStatusCode() != 200 || httpResponse->GetHeader("Content-Type") != "application/json") {
		m_observer->OnServerError(std::format("{} {}", httpResponse->GetStatusCode(), httpResponse->GetStatusText()));

		return;
	}

	json body = json::parse(httpResponse->GetBody());

	m_userData.userId = body["user_id"];
	m_userData.username = username;
}


void ClientModelFacade::AddUserInChat(const int64_t chatId, const std::string& addedUsername) const
{
	std::optional<Http::Response> httpResponse = m_clientServiceFacade.PerformRequest(
		m_clientProtocol.GetAddUserInChatRequest(chatId, addedUsername));

	if (!httpResponse.has_value()) {
		m_observer->OnNetworkError("connection problems");

		return;
	}

	if (httpResponse->GetStatusCode() != 200) {
		m_observer->OnServerError(std::format("{} {}", httpResponse->GetStatusCode(), httpResponse->GetStatusText()));

		return;
	}
}


void ClientModelFacade::CreateChat(const std::string& chatname) const
{
	std::optional<Http::Response> httpResponse = m_clientServiceFacade.PerformRequest(
		m_clientProtocol.GetCreateChatRequest(chatname, m_userData.userId));

	if (!httpResponse.has_value()) {
		m_observer->OnNetworkError("connection problems");

		return;
	}

	if (httpResponse->GetStatusCode() != 200) {
		m_observer->OnServerError(std::format("{} {}", httpResponse->GetStatusCode(), httpResponse->GetStatusText()));

		return;
	}

}


void ClientModelFacade::SendMessage(const int64_t chatId, const std::string& text) const
{
	if (chatId == -1) {
		// todo: трогать модель
		return;
	}

	auto time = std::chrono::system_clock::now();

	std::optional<Http::Response> httpResponse = m_clientServiceFacade.PerformRequest(
		m_clientProtocol.GetSendMessageToChatRequest(chatId, m_userData.userId, text, time));

	if (!httpResponse.has_value()) {
		m_observer->OnNetworkError("connection problems");

		return;
	}

	if (httpResponse->GetStatusCode() != 200) {
		m_observer->OnServerError(std::format("{} {}", httpResponse->GetStatusCode(), httpResponse->GetStatusText()));

		return;
	}

}


std::vector<ChatDTO> ClientModelFacade::GetChats()
{
	std::optional<Http::Response> httpResponse = m_clientServiceFacade.PerformRequest(
		m_clientProtocol.GetChatsRequest(m_userData.userId));

	if (!httpResponse.has_value()) {
		m_observer->OnNetworkError("connection problems");

		return {};
	}

	if (httpResponse->GetStatusCode() != 200) {
		m_observer->OnServerError(std::format("{} {}", httpResponse->GetStatusCode(), httpResponse->GetStatusText()));

		return {};
	}

	json body = json::parse(httpResponse->GetBody());
	
	m_userSessionData.chats.clear();
	m_userSessionData.chats.reserve(body["chats"].size());

	for (const auto& info : body["chats"]) {
		m_userSessionData.chats.emplace_back(info["id"], info["name"]);
	}

	return m_userSessionData.chats;
}


std::vector<ChatMessageDTO> ClientModelFacade::GetChatHistory(const int64_t chatId) const
{
	std::optional<Http::Response> httpResponse = m_clientServiceFacade.PerformRequest(
		m_clientProtocol.GetChatHistoryRequest(chatId));

	if (!httpResponse.has_value()) {
		m_observer->OnNetworkError("connection problems");

		return {};
	}

	if (httpResponse->GetStatusCode() != 200) {
		m_observer->OnServerError(std::format("{} {}", httpResponse->GetStatusCode(), httpResponse->GetStatusText()));

		return {};
	}

	json body = json::parse(httpResponse->GetBody());

	std::vector<ChatMessageDTO> messagesInfo;

	for (const auto& info : body["messages"]) {
		messagesInfo.emplace_back(info["user_id"], info["username"],
			info["text"], HelperFuncs::ChronoParse(info["time"]));
	}

	return messagesInfo;
}

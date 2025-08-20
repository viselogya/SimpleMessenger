#include "pch.h"
#include "OptionsRequestHandler.h"

#include "../StreamPipeline/HttpRouter.h"

#include "../ServerProtocolInterface/ServerProtocol.hpp"

namespace http = boost::beast::http;

OptionsRequestHandler::OptionsRequestHandler(ServerProtocol& serverProtocol)
	:
	m_serverProtocol(serverProtocol)
{}


void OptionsRequestHandler::BindHandler(HttpRouter& httpRouter) const
{
	std::vector<std::string> endpoints = { "/login", "/registration", "/users", "/create_chat", "/chats", "/chats/messages",
	"/chats/messages/send_message", "/chats/add_user", "/token/refresh" };

	for (auto& endpoint : endpoints) {
		httpRouter.AddRoute({ "OPTIONS", std::move(endpoint) }, std::bind_front(&OptionsRequestHandler::Handle, this));
	}
}


std::string OptionsRequestHandler::Handle(const http::request<http::string_body>& request) const
{
	return m_serverProtocol.OkForOptions();
}

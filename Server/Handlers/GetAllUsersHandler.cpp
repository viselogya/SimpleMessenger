#include "pch.h"
#include "GetAllUsersHandler.h"

#include "../DTO/DTO.h"

#include "../StreamPipeline/HttpRouter.h"

#include "../ServerProtocolInterface/ServerProtocol.hpp"

#include "../ServerLogic/UserService.h"

GetAllUsersHandler::GetAllUsersHandler(UserService& userService, ServerProtocol& serverProtocol)
	:
	m_userService(userService),
	m_serverProtocol(serverProtocol)
{}


GetAllUsersHandler::~GetAllUsersHandler() = default;


void GetAllUsersHandler::BindHandler(HttpRouter& httpRouter) const
{
	httpRouter.AddRoute({ "GET", "/users" }, std::bind_front(&GetAllUsersHandler::Handle, this));
}


std::string GetAllUsersHandler::Handle(const boost::beast::http::request<boost::beast::http::string_body>& request) const
{
	return m_serverProtocol.MakeUserListResponse(m_userService.GetAllUsers());
}

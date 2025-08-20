#pragma once

class HttpRouter;

class IHandler 
{
public:
	
	virtual ~IHandler() = default;

	virtual void BindHandler(HttpRouter& httpRouter) const = 0;

	virtual std::string Handle(const boost::beast::http::request<boost::beast::http::string_body>& request) const = 0;

};

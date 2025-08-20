#pragma once
class SignalsHandler
{
public:

	static SignalsHandler& Instance();

	void SetCloseSocket(std::unique_ptr<std::function<void()>> closeSocket) const;

	void SetStopUserSession(std::unique_ptr<std::function<void()>> stopUserSession);

	static BOOL WINAPI CtrlHandler(DWORD fdwCtrlType);
	
private:
	
	inline static std::unique_ptr<std::function<void()>> m_closeSocket;

	inline static std::unique_ptr<std::function<void()>> m_stopUserSession;
};


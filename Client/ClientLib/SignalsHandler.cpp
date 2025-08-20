#include "pch.h"
#include "SignalsHandler.h"
#include "../Logger/Logger.h"

SignalsHandler& SignalsHandler::Instance() {
    static SignalsHandler signalsHandler;
    return signalsHandler;
}


void SignalsHandler::SetCloseSocket(std::unique_ptr<std::function<void()>> closeSocket) const {
    m_closeSocket = std::move(closeSocket);
}

void SignalsHandler::SetStopUserSession(std::unique_ptr<std::function<void()>> stopUserSession) {
    m_stopUserSession = std::move(stopUserSession);

    SetConsoleCtrlHandler((PHANDLER_ROUTINE)CtrlHandler, TRUE);
}

BOOL WINAPI SignalsHandler::CtrlHandler(DWORD fdwCtrlType) {
    switch (fdwCtrlType)
    {
        // Handle the CTRL-C signal.
    case CTRL_C_EVENT:
        Logger::Instance().Info("CTRL+C signal was called");

        (*m_closeSocket)();
        (*m_stopUserSession)();

        return TRUE;

    default:
        return FALSE;
    }
}

#include "pch.h"
#include "HelperFuncs.h"
#include "ServerExceptions.h"

std::filesystem::path HelperFuncs::GetAppFolderPath()
{
	WCHAR buff[MAX_PATH];
	DWORD sz = ::GetModuleFileNameW(NULL, buff, std::size(buff));

	if (sz == 0 || GetLastError() == ERROR_INSUFFICIENT_BUFFER) 
	{
		throw ServerExceptionSystem("ERROR WITH GET_MODUEL_FILENAMEW");
	}

	const std::filesystem::path path{ buff };

	return path.parent_path();
}
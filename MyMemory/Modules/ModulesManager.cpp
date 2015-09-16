#include <Pch/StdAfx.h>

MyMemory::Modules::ModulesManager::ModulesManager(RemoteProcess^ remoteProcess)
{
	m_remoteProcess = remoteProcess;
}

MyMemory::Modules::RemoteModule^ MyMemory::Modules::ModulesManager::default::get(String^ s)
{
	return GetModule(s);
}

MyMemory::Modules::RemoteModule^ MyMemory::Modules::ModulesManager::GetModule(String^ name)
{

	HMODULE hMods[1024];
	WCHAR buffer[MAX_PATH];
	DWORD cbNeeded;
	pin_ptr<const wchar_t> wName = PtrToStringChars(name);

	if (EnumProcessModules(m_remoteProcess->ProcessHandle.ToPointer(), hMods, sizeof(hMods), &cbNeeded))
	{
		for (unsigned int i = 0; i < (cbNeeded / sizeof(HMODULE)); i++)
		{
			HMODULE hModule = hMods[i];
			if (hModule && GetModuleBaseNameW(m_remoteProcess->ProcessHandle.ToPointer(), hModule, buffer, sizeof(buffer)) && _wcsicmp(buffer, wName) == 0)
			{
				return gcnew MyMemory::Modules::RemoteModule(m_remoteProcess, IntPtr(hModule));
			}
		}
	}

	return nullptr;

}

List<MyMemory::Modules::RemoteModule^>^ MyMemory::Modules::ModulesManager::Modules::get()
{

	List<MyMemory::Modules::RemoteModule^>^ results = gcnew List<MyMemory::Modules::RemoteModule^>();

	HMODULE hMods[1024];
	DWORD cbNeeded;

	if (EnumProcessModules(m_remoteProcess->ProcessHandle.ToPointer(), hMods, sizeof(hMods), &cbNeeded))
	{
		for (unsigned int i = 0; i < (cbNeeded / sizeof(HMODULE)); i++)
		{
			HMODULE hModule = hMods[i];
			if (hModule)
			{
				results->Add(gcnew MyMemory::Modules::RemoteModule(m_remoteProcess, IntPtr(hModule)));
			}
		}
	}

	return results;

}
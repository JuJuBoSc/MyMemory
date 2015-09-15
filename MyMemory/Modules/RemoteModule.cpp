#include <Pch/StdAfx.h>

MyMemory::Modules::RemoteModule::RemoteModule(RemoteProcess^ process, IntPtr hModule) : 
	MyMemory::Memory::RemotePointer(process, hModule)
{
	m_hModule = (HMODULE)hModule.ToPointer();
	WCHAR buffer[MAX_PATH];

	GetModuleFileNameExW(this->Process->ProcessHandle.ToPointer(), m_hModule, buffer, sizeof(buffer));
	m_fullName = gcnew String(buffer);

	GetModuleBaseNameW(this->Process->ProcessHandle.ToPointer(), m_hModule, buffer, sizeof(buffer));
	m_baseName = gcnew String(buffer);

	MODULEINFO moduleInfo;
	GetModuleInformation(Process->ProcessHandle.ToPointer(), (HMODULE)hModule.ToPointer(), &moduleInfo, sizeof(moduleInfo));

	m_sizeOfImage = moduleInfo.SizeOfImage;
	m_baseAddress = IntPtr(moduleInfo.lpBaseOfDll);
	m_entryPointAddress = IntPtr(moduleInfo.EntryPoint);
}

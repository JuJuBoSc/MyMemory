#include <Pch/StdAfx.h>

MyMemory::Modules::RemoteModule::RemoteModule(RemoteProcess^ process, IntPtr hModule) : 
	MyMemory::Memory::RemotePointer(process, hModule)
{
	m_hModule = (HMODULE)hModule.ToPointer();
}

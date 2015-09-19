#include <Pch/StdAfx.h>

MyMemory::Hooks::HooksManager::HooksManager(RemoteProcess^ remoteProcess)
{
	m_remoteProcess = remoteProcess;
	m_hooks = gcnew List<MyMemory::Hooks::HookBase^>();
}

MyMemory::Hooks::HooksManager::~HooksManager()
{
	while (m_hooks->Count > 0)
	{
		delete m_hooks[0];
	}
	m_hooks->Clear();
}

MyMemory::Hooks::HookJmp^ MyMemory::Hooks::HooksManager::CreateJmpHook(IntPtr lpAddress, int length)
{
	return gcnew MyMemory::Hooks::HookJmp(m_remoteProcess, lpAddress, length);
}

MyMemory::Hooks::HookJmp^ MyMemory::Hooks::HooksManager::CreateAndApplyJmpHook(IntPtr lpAddress, int length)
{
	MyMemory::Hooks::HookJmp^ hook = CreateJmpHook(lpAddress, length);
	hook->Apply();
	return hook;
}

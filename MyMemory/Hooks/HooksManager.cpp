#include <Pch/StdAfx.h>

MyMemory::Hooks::HooksManager::HooksManager(RemoteProcess^ remoteProcess)
{
	m_remoteProcess = remoteProcess;
	m_hooks = gcnew List<MyMemory::Hooks::HookBase^>();
}

MyMemory::Hooks::HooksManager::~HooksManager()
{

	while (m_hooks->Count > 0)
		delete m_hooks[0];

	for each (IntPtr pFakeVtable in m_fakeVtables.Keys)
		Process->MemoryManager->FreeRawMemory(pFakeVtable);
	m_fakeVtables.Clear();

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

MyMemory::Hooks::HookVmtOverwrite^ MyMemory::Hooks::HooksManager::CreateVmtOverwriteHook(IntPtr vTable, int functionIndex)
{
	return gcnew MyMemory::Hooks::HookVmtOverwrite(m_remoteProcess, vTable, functionIndex);
}

MyMemory::Hooks::HookVmtOverwrite^ MyMemory::Hooks::HooksManager::CreateAndApplyVmtOverwriteHook(IntPtr vTable, int functionIndex)
{
	MyMemory::Hooks::HookVmtOverwrite^ hook = CreateVmtOverwriteHook(vTable, functionIndex);
	hook->Apply();
	return hook;
}

MyMemory::Hooks::HookVmtCopy^ MyMemory::Hooks::HooksManager::CreateVmtCopyHook(IntPtr pObject, int functionIndex)
{
	return gcnew MyMemory::Hooks::HookVmtCopy(m_remoteProcess, pObject, functionIndex);
}

MyMemory::Hooks::HookVmtCopy^ MyMemory::Hooks::HooksManager::CreateAndApplyVmtCopyHook(IntPtr pObject, int functionIndex)
{
	MyMemory::Hooks::HookVmtCopy^ hook = CreateVmtCopyHook(pObject, functionIndex);
	hook->Apply();
	return hook;
}

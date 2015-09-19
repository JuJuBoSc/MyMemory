#include <Pch/StdAfx.h>

MyMemory::Hooks::HookVmtOverwrite::HookVmtOverwrite(RemoteProcess^ remoteProcess, IntPtr vTable, int functionIndex)
	: MyMemory::Hooks::HookBase(remoteProcess)
{
	m_isApplied = false;
	m_vtableAddress = vTable;
	m_functionIndex = functionIndex;
	m_originalAddress = remoteProcess->MemoryManager->Read<IntPtr>(vTable + functionIndex * IntPtr::Size);
	m_hookVmtAllocatedMemory = Process->MemoryManager->AllocateMemory(0x10000);
	m_detourPointer = m_hookVmtAllocatedMemory->AllocateOfChunk("Detour", 5000);

	GenerateDetour();
}

MyMemory::Hooks::HookVmtOverwrite::~HookVmtOverwrite()
{
	if (IsApplied)
	{
		Remove();
		Sleep(200); // Just in case we are still in the hook
	}
	delete m_hookVmtAllocatedMemory;
}

void MyMemory::Hooks::HookVmtOverwrite::GenerateDetour()
{

	List<String^> asmDetour;

	asmDetour.AddRange(Utils::Assembly::Generate_PushRegistersAndFlags());
	asmDetour.AddRange(Utils::Assembly::Generate_WaitForSingleObject(m_hookMutexHandle, INFINITE));
	asmDetour.AddRange(Utils::Assembly::Generate_PopRegistersAndFlags());
	asmDetour.AddRange(GenerateSaveRegisters());
	asmDetour.AddRange(Utils::Assembly::Generate_PushRegistersAndFlags());
	asmDetour.AddRange(GenerateInvokeCallbacks());
	asmDetour.AddRange(Utils::Assembly::Generate_PopRegistersAndFlags());
	asmDetour.AddRange(Utils::Assembly::Generate_PushRegistersAndFlags());
	asmDetour.AddRange(Utils::Assembly::Generate_ReleaseMutex(m_hookMutexHandle));
	asmDetour.AddRange(Utils::Assembly::Generate_PopRegistersAndFlags());

#if _WIN64 || __amd64__
	asmDetour.Add(String::Format("jmp [rip+0]"));
	asmDetour.Add(String::Format("originalAddress: dq 0x{0:X}", m_originalAddress.ToInt64()));
#else
	asmDetour.Add(String::Format("jmp 0x{0:X}", m_originalAddress.ToInt64()));
#endif

	Process->Yasm->Inject(asmDetour.ToArray(), m_detourPointer);

}

void MyMemory::Hooks::HookVmtOverwrite::Apply()
{
	if (m_isApplied)
		return;

	Process->MemoryManager->Write<IntPtr>(m_vtableAddress + m_functionIndex * IntPtr::Size, m_detourPointer);

	m_isApplied = true;
}

void MyMemory::Hooks::HookVmtOverwrite::Remove()
{
	if (!m_isApplied)
		return;

	Process->MemoryManager->Write<IntPtr>(m_vtableAddress + m_functionIndex * IntPtr::Size, m_originalAddress);

	m_isApplied = false;
}

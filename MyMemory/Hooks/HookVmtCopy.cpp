#include <Pch/StdAfx.h>

MyMemory::Hooks::HookVmtCopy::HookVmtCopy(RemoteProcess^ remoteProcess, IntPtr pObject, int functionIndex)
	: MyMemory::Hooks::HookBase(remoteProcess)
{

	m_objectAddress = pObject;
	m_functionIndex = functionIndex;
	m_originalVtableAddress = remoteProcess->MemoryManager->Read<IntPtr>(pObject);
	m_originalAddress = remoteProcess->MemoryManager->Read<IntPtr>(m_originalVtableAddress + functionIndex * IntPtr::Size);
	m_hookVmtAllocatedMemory = Process->MemoryManager->AllocateMemory(0x10000);
	m_detourPointer = m_hookVmtAllocatedMemory->AllocateOfChunk("Detour", 5000);

	if (remoteProcess->HooksManager->m_fakeVtables.ContainsKey(m_originalVtableAddress))
	{
		m_fakeVtablePointer = m_originalVtableAddress;
		m_isOriginalVtable = false;
	}
	else
	{
		m_isOriginalVtable = true;
		int vmtCount = CountVmt(m_originalVtableAddress);

		if (vmtCount == 0)
			throw gcnew Exception("The vTable was empty !");

		m_fakeVtablePointer = remoteProcess->MemoryManager->AllocateRawMemory(vmtCount * IntPtr::Size);
		remoteProcess->MemoryManager->WriteBytes(m_fakeVtablePointer, remoteProcess->MemoryManager->ReadBytes(m_originalVtableAddress, vmtCount * IntPtr::Size));
		remoteProcess->HooksManager->m_fakeVtables.Add(m_fakeVtablePointer, m_originalVtableAddress);
	}

	Process->MemoryManager->Write<IntPtr>(m_objectAddress, m_fakeVtablePointer);

	GenerateDetour();

}

MyMemory::Hooks::HookVmtCopy::~HookVmtCopy()
{
	if (IsApplied)
	{
		Remove();
		Sleep(200); // Just in case we are still in the hook
	}
	if (m_isOriginalVtable)
	{
		Process->MemoryManager->Write<IntPtr>(m_objectAddress, m_originalVtableAddress);
	}
	delete m_hookVmtAllocatedMemory;
}

unsigned long MyMemory::Hooks::HookVmtCopy::CountVmt(IntPtr pVtable)
{
	unsigned long result = 0;
	for (int i = 0; ; i++)
	{
		IntPtr pFunction = Process->MemoryManager->Read<IntPtr>(pVtable + i * IntPtr::Size);
		MEMORY_BASIC_INFORMATION mbi;
		if (pFunction.ToPointer() && 
			VirtualQueryEx(Process->ProcessHandle.ToPointer(), pFunction.ToPointer(), &mbi, sizeof(mbi)) != 0 && 
			((mbi.AllocationProtect == PAGE_EXECUTE) || (mbi.AllocationProtect == PAGE_EXECUTE_READ) || (mbi.AllocationProtect == PAGE_EXECUTE_READWRITE) || (mbi.AllocationProtect == PAGE_EXECUTE_WRITECOPY)))
		{
			result++;
		}
		else
		{
			break;
		}
	}
	return result;
}

void MyMemory::Hooks::HookVmtCopy::GenerateDetour()
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

void MyMemory::Hooks::HookVmtCopy::Apply()
{
	if (m_isApplied)
		return;

	Process->MemoryManager->Write<IntPtr>(m_fakeVtablePointer + m_functionIndex * IntPtr::Size, m_detourPointer);

	m_isApplied = true;
}

void MyMemory::Hooks::HookVmtCopy::Remove()
{
	if (!m_isApplied)
		return;

	Process->MemoryManager->Write<IntPtr>(m_fakeVtablePointer + m_functionIndex * IntPtr::Size, m_originalAddress);

	m_isApplied = false;
}

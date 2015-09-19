#include <Pch/StdAfx.h>

MyMemory::Hooks::HookJmp::HookJmp(RemoteProcess^ remoteProcess, IntPtr lpAddress, unsigned long length)
	: MyMemory::Hooks::HookBase(remoteProcess)
{

#if _WIN64 || __amd64__
	if (length < 14)
		throw gcnew ArgumentException("Length must be at least 14 bytes !", "length");
#else
	if (length < 5)
		throw gcnew ArgumentException("Length must be at least 5 bytes !", "length");
#endif

	m_isApplied = false;
	m_address = lpAddress;
	m_length = length;
	m_originalBytes = Process->MemoryManager->ReadBytes(m_address, m_length);
	m_hookJmpAllocatedMemory = Process->MemoryManager->AllocateMemory(0x10000);
	m_trampolinePointer = m_hookJmpAllocatedMemory->AllocateOfChunk("Trampoline", 100 + length);
	m_detourPointer = m_hookJmpAllocatedMemory->AllocateOfChunk("Detour", 5000);

	GenerateTrampoline();
	GenerateDetour();
	GenerateJmp();
		
}

MyMemory::Hooks::HookJmp::~HookJmp()
{
	if (IsApplied)
	{
		Remove();
		Sleep(200); // Just in case we are still in the hook
	}
	delete m_hookJmpAllocatedMemory;
}

void MyMemory::Hooks::HookJmp::GenerateTrampoline()
{
	Process->MemoryManager->WriteBytes(m_trampolinePointer, m_originalBytes);
	List<String^> asmTrampoline;
#if _WIN64 || __amd64__
	asmTrampoline.Add(String::Format("jmp [rip+0]"));
	asmTrampoline.Add(String::Format("originalCode: dq 0x{0:X}", (m_address + m_length).ToInt64()));
#else
	asmTrampoline.Add(String::Format("jmp 0x{0:X}", (m_address + m_length).ToInt64()));
#endif
	Process->Yasm->Inject(asmTrampoline.ToArray(), (m_trampolinePointer + m_length));
}

void MyMemory::Hooks::HookJmp::GenerateDetour()
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
	asmDetour.Add(String::Format("trampoline: dq 0x{0:X}", m_trampolinePointer.ToInt64()));
#else
	asmDetour.Add(String::Format("jmp 0x{0:X}", m_trampolinePointer.ToInt64()));
#endif

	Process->Yasm->Inject(asmDetour.ToArray(), m_detourPointer);

}

void MyMemory::Hooks::HookJmp::GenerateJmp()
{
	List<String^> asmJmp;
#if _WIN64 || __amd64__
	asmJmp.Add(String::Format("jmp [rip+0]"));
	asmJmp.Add(String::Format("detour: dq 0x{0:X}", m_detourPointer.ToInt64()));
#else
	asmJmp.Add(String::Format("jmp 0x{0:X}", m_detourPointer.ToInt64()));
#endif
	m_jmpBytes = Process->Yasm->Assemble(asmJmp.ToArray(), m_address);
}

void MyMemory::Hooks::HookJmp::Apply()
{

	if (m_isApplied)
		return;

	Process->MemoryManager->WriteBytes(m_address, m_jmpBytes);

	m_isApplied = true;

}

void MyMemory::Hooks::HookJmp::Remove()
{

	if (!m_isApplied)
		return;

	Process->MemoryManager->WriteBytes(m_address, m_originalBytes);

	m_isApplied = false;

}


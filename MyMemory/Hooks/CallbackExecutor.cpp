#include <Pch/StdAfx.h>

void MyMemory::Hooks::CallbackExecutor::SetFrameLock(bool value)
{
	Hook->Process->MemoryManager->Write<int>(m_executeFrameLockRequiredPointer, Convert::ToInt32(value));
}

MyMemory::Hooks::CallbackExecutor::CallbackExecutor(HookBase^ hook)
	: MyMemory::Hooks::CallbackBase(hook)
{

	m_frameLockMutex = gcnew Mutex();
	m_executorAllocatedMemory = Hook->Process->MemoryManager->AllocateMemory(0x20000);
	m_entryPointPointer = m_executorAllocatedMemory->AllocateOfChunk("EntryPoint", 0x5000);
	m_executeCodePointer = m_executorAllocatedMemory->AllocateOfChunk("ExecuteCodePointer", 0x10000);
	m_executeReturnValuePointer = m_executorAllocatedMemory->AllocateOfChunk("ExecuteReturnValue", 8);
	m_executeFrameLockRequiredPointer = m_executorAllocatedMemory->AllocateOfChunk("FrameLockRequired", 4);
	m_executeFrameLockedPointer = m_executorAllocatedMemory->AllocateOfChunk("FrameLocked", 4);

	m_executeRequiredEventName = "Global\\" + Utils::Randoms::GetRandomString(32, 64);
	m_executeFinishedEventName = "Global\\" + Utils::Randoms::GetRandomString(32, 64);

	m_executeRequiredRemoteHandle = Utils::Assembly::Remote_CreateEvent(Hook->Process, m_executeRequiredEventName, false, false);
	m_executeFinishedRemoteHandle = Utils::Assembly::Remote_CreateEvent(Hook->Process, m_executeFinishedEventName, false, false);
	
	if (!m_executeRequiredRemoteHandle.ToPointer() || !m_executeFinishedRemoteHandle.ToPointer())
		throw gcnew Exception("Unable to create remote event for executor !");

	IntPtr pExecuteRequiredEventName = Marshal::StringToHGlobalAnsi(m_executeRequiredEventName);
	IntPtr pExecuteFinishedEventName = Marshal::StringToHGlobalAnsi(m_executeFinishedEventName);
	
	m_executeRequiredLocalHandle = IntPtr(OpenEventA(EVENT_MODIFY_STATE | SYNCHRONIZE, false, (LPCSTR)pExecuteRequiredEventName.ToPointer()));
	m_executeFinishedLocalHandle = IntPtr(OpenEventA(EVENT_MODIFY_STATE | SYNCHRONIZE, false, (LPCSTR)pExecuteFinishedEventName.ToPointer()));

	Marshal::FreeHGlobal(pExecuteRequiredEventName);
	Marshal::FreeHGlobal(pExecuteFinishedEventName);

	if (!m_executeRequiredLocalHandle.ToPointer() || !m_executeFinishedLocalHandle.ToPointer())
		throw gcnew Exception("Unable to open remote event for executor !");



	GenerateExecutor();
}

MyMemory::Hooks::CallbackExecutor::~CallbackExecutor()
{
	delete m_executorAllocatedMemory;
	CloseHandle(m_executeRequiredLocalHandle.ToPointer());
	CloseHandle(m_executeFinishedLocalHandle.ToPointer());
}

generic <typename T> T MyMemory::Hooks::CallbackExecutor::Execute(array<String^>^ mnemonics)
{
	Monitor::Enter(this);
	try
	{
		Hook->Process->Yasm->Inject(mnemonics, m_executeCodePointer);
		SetEvent(m_executeRequiredLocalHandle.ToPointer());
		WaitForSingleObject(m_executeFinishedLocalHandle.ToPointer(), 10000);
		return Hook->Process->MemoryManager->Read<T>(m_executeReturnValuePointer);
	}
	finally
	{
		Monitor::Exit(this);
	}
}

MyMemory::Hooks::CallbackExecutor::FrameLock^ MyMemory::Hooks::CallbackExecutor::LockFrame()
{
	return gcnew FrameLock(this);
}

void MyMemory::Hooks::CallbackExecutor::GenerateExecutor()
{
	List<String^> asmExecutor;

#if _WIN64 || __amd64__
	// Start
	asmExecutor.Add(String::Format("@start_executor:"));

	// Check if we need execution
	asmExecutor.AddRange(Utils::Assembly::Generate_WaitForSingleObject(m_executeRequiredRemoteHandle, 0));
	asmExecutor.Add(String::Format("test eax, eax"));
	asmExecutor.Add(String::Format("jne @executor_check_framelock"));

	// Reset return value
	asmExecutor.Add(String::Format("xor rcx, rcx"));
	asmExecutor.Add(String::Format("mov rax, 0x{0:X}", m_executeReturnValuePointer.ToInt64()));
	asmExecutor.Add(String::Format("mov [rax], rcx"));

	// Execute callback
	asmExecutor.Add(String::Format("mov rax, 0x{0:X}", m_executeCodePointer.ToInt64()));
	asmExecutor.Add(String::Format("call rax"));

	// Store return value
	asmExecutor.Add(String::Format("mov rcx, 0x{0:X}", m_executeReturnValuePointer.ToInt64()));
	asmExecutor.Add(String::Format("mov [rcx], rax"));

	// Signal execution complete
	asmExecutor.AddRange(Utils::Assembly::Generate_SetEvent(m_executeFinishedRemoteHandle));

	// Check frame lock
	asmExecutor.Add(String::Format("@executor_check_framelock:"));
	asmExecutor.Add(String::Format("mov rax, 0x{0:X}", m_executeFrameLockRequiredPointer.ToInt64()));
	asmExecutor.Add(String::Format("mov eax, [rax]"));
	asmExecutor.Add(String::Format("test eax, eax"));
	asmExecutor.Add(String::Format("je @exit_executor"));

	// Set frame locked
	asmExecutor.Add(String::Format("mov rax, 0x{0:X}", m_executeFrameLockedPointer.ToInt64()));
	asmExecutor.Add(String::Format("mov edx, 1"));
	asmExecutor.Add(String::Format("mov [rax], edx"));
	asmExecutor.Add(String::Format("jmp @start_executor"));

	// Exit
	asmExecutor.Add(String::Format("@exit_executor:"));

	// Set frame locked
	asmExecutor.Add(String::Format("mov rax, 0x{0:X}", m_executeFrameLockedPointer.ToInt64()));
	asmExecutor.Add(String::Format("xor edx, edx"));
	asmExecutor.Add(String::Format("mov [rax], edx"));

	// Return
	asmExecutor.Add(String::Format("retn"));
#else
	// Start
	asmExecutor.Add(String::Format("@start_executor:"));

	// Check if we need execution
	asmExecutor.AddRange(Utils::Assembly::Generate_WaitForSingleObject(m_executeRequiredRemoteHandle, 0));
	asmExecutor.Add(String::Format("test eax, eax"));
	asmExecutor.Add(String::Format("jne @executor_check_framelock"));

	// Reset return value
	asmExecutor.Add(String::Format("xor ecx, ecx"));
	asmExecutor.Add(String::Format("mov [0x{0:X}], ecx", m_executeReturnValuePointer.ToInt64()));

	// Execute callback
	asmExecutor.Add(String::Format("call 0x{0:X}", m_executeCodePointer.ToInt64()));

	// Store return value
	asmExecutor.Add(String::Format("mov [0x{0:X}], eax", m_executeReturnValuePointer.ToInt64()));

	// Signal execution complete
	asmExecutor.AddRange(Utils::Assembly::Generate_SetEvent(m_executeFinishedRemoteHandle));

	// Check frame lock
	asmExecutor.Add(String::Format("@executor_check_framelock:"));
	asmExecutor.Add(String::Format("mov eax, [0x{0:X}]", m_executeFrameLockRequiredPointer.ToInt64()));
	asmExecutor.Add(String::Format("test eax, eax"));
	asmExecutor.Add(String::Format("je @exit_executor"));

	// Set frame locked
	asmExecutor.Add(String::Format("mov edx, 1"));
	asmExecutor.Add(String::Format("mov [0x{0:X}], edx", m_executeFrameLockedPointer.ToInt64()));
	asmExecutor.Add(String::Format("jmp @start_executor"));

	// Exit
	asmExecutor.Add(String::Format("@exit_executor:"));

	// Set frame locked
	asmExecutor.Add(String::Format("xor edx, edx"));
	asmExecutor.Add(String::Format("mov [0x{0:X}], edx", m_executeFrameLockedPointer.ToInt64()));

	// Return
	asmExecutor.Add(String::Format("retn"));
#endif

	Hook->Process->Yasm->Inject(asmExecutor.ToArray(), m_entryPointPointer);
}

MyMemory::Hooks::CallbackExecutor::FrameLock::FrameLock(CallbackExecutor^ executor)
{
	m_executor = executor;
	m_executor->m_frameLockMutex->WaitOne();
	m_executor->SetFrameLock(true);
}

MyMemory::Hooks::CallbackExecutor::FrameLock::~FrameLock()
{
	m_executor->SetFrameLock(false);
	m_executor->m_frameLockMutex->ReleaseMutex();
}

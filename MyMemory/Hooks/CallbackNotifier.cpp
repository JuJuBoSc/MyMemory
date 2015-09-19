#include <Pch/StdAfx.h>

MyMemory::Hooks::CallbackNotifier::CallbackNotifier(HookBase^ hook)
	: MyMemory::Hooks::CallbackBase(hook)
{
	m_callbacks = gcnew List<NotifyCallback^>();
	m_stopRequired = false;
	m_notifierAllocatedMemory = Hook->Process->MemoryManager->AllocateMemory(0x20000);
	m_entryPointPointer = m_notifierAllocatedMemory->AllocateOfChunk("EntryPoint", 0x5000);
	m_notifyEnabledPointer = m_notifierAllocatedMemory->AllocateOfChunk("NotifyEnabled", 4);
	m_threadIdPointer = m_notifierAllocatedMemory->AllocateOfChunk("ThreadId", 4);

	m_notifierReachedEventName = "Global\\" + Utils::Randoms::GetRandomString(32, 64);
	m_notifierHandledEventName = "Global\\" + Utils::Randoms::GetRandomString(32, 64);

	m_notifierReachedRemoteHandle = Utils::Assembly::Remote_CreateEvent(Hook->Process, m_notifierReachedEventName, false, false);
	m_notifierHandledRemoteHandle = Utils::Assembly::Remote_CreateEvent(Hook->Process, m_notifierHandledEventName, false, false);

	if (!m_notifierReachedRemoteHandle.ToPointer() || !m_notifierHandledRemoteHandle.ToPointer())
		throw gcnew Exception("Unable to create remote event for notifier !");

	IntPtr pNotifierReachedEventName = Marshal::StringToHGlobalAnsi(m_notifierReachedEventName);
	IntPtr pNotifierHandledEventName = Marshal::StringToHGlobalAnsi(m_notifierHandledEventName);

	m_notifierReachedLocalHandle = IntPtr(OpenEventA(EVENT_MODIFY_STATE | SYNCHRONIZE, false, (LPCSTR)pNotifierReachedEventName.ToPointer()));
	m_notifierHandledLocalHandle = IntPtr(OpenEventA(EVENT_MODIFY_STATE | SYNCHRONIZE, false, (LPCSTR)pNotifierHandledEventName.ToPointer()));

	Marshal::FreeHGlobal(pNotifierReachedEventName);
	Marshal::FreeHGlobal(pNotifierHandledEventName);

	if (!m_notifierReachedLocalHandle.ToPointer() || !m_notifierHandledLocalHandle.ToPointer())
		throw gcnew Exception("Unable to open remote event for notifier !");
	
	GenerateNotifier();

	m_threadWaitForEvents = gcnew Thread(gcnew ThreadStart(this, &CallbackNotifier::WaitForEvents));
	m_threadWaitForEvents->IsBackground = true;
	m_threadWaitForEvents->Start();

}

MyMemory::Hooks::CallbackNotifier::~CallbackNotifier()
{
	m_stopRequired = true;
	delete m_notifierAllocatedMemory;
	CloseHandle(m_notifierReachedLocalHandle.ToPointer());
	CloseHandle(m_notifierHandledLocalHandle.ToPointer());
}

bool MyMemory::Hooks::CallbackNotifier::AddCallback(NotifyCallback^ callback)
{
	Monitor::Enter(m_callbacks);
	try
	{
		if (!m_callbacks->Contains(callback))
		{
			m_callbacks->Add(callback);
			Hook->Process->MemoryManager->Write<int>(m_notifyEnabledPointer, m_callbacks->Count > 0 ? 1 : 0);
			return true;
		}
		return false;
	}
	finally
	{
		Monitor::Exit(m_callbacks);
	}
}

bool MyMemory::Hooks::CallbackNotifier::RemoveCallback(NotifyCallback^ callback)
{
	Monitor::Enter(m_callbacks);
	try
	{
		if (m_callbacks->Contains(callback))
		{
			m_callbacks->Remove(callback);
			Hook->Process->MemoryManager->Write<int>(m_notifyEnabledPointer, m_callbacks->Count > 0 ? 1 : 0);
			return true;
		}
		return false;
	}
	finally
	{
		Monitor::Exit(m_callbacks);
	}
}

void MyMemory::Hooks::CallbackNotifier::GenerateNotifier()
{
	List<String^> asmNotifier;

#if _WIN64 || __amd64__
	// Check if we should notify
	asmNotifier.Add(String::Format("mov rax, 0x{0:X}", m_notifyEnabledPointer.ToInt64()));
	asmNotifier.Add(String::Format("mov eax, [rax]"));
	asmNotifier.Add(String::Format("test eax, eax"));
	asmNotifier.Add(String::Format("je @notifier_exit"));

	// Get current thread id
	asmNotifier.AddRange(Utils::Assembly::Generate_GetCurrentThreadId());

	// Store thread id
	asmNotifier.Add(String::Format("mov rcx, 0x{0:X}", m_threadIdPointer.ToInt64()));
	asmNotifier.Add(String::Format("mov [rcx], rax"));

	// Signal event reached
	asmNotifier.AddRange(Utils::Assembly::Generate_SetEvent(m_notifierReachedRemoteHandle));

	// Wait for notify handled
	asmNotifier.AddRange(Utils::Assembly::Generate_WaitForSingleObject(m_notifierHandledRemoteHandle, INFINITE));

	// Return
	asmNotifier.Add(String::Format("@notifier_exit:"));
	asmNotifier.Add(String::Format("retn"));
#else
	// Check if we should notify
	asmNotifier.Add(String::Format("mov eax, [0x{0:X}]", m_notifyEnabledPointer.ToInt64()));
	asmNotifier.Add(String::Format("test eax, eax"));
	asmNotifier.Add(String::Format("je @notifier_exit"));

	// Get current thread id
	asmNotifier.AddRange(Utils::Assembly::Generate_GetCurrentThreadId());

	// Store thread id
	asmNotifier.Add(String::Format("mov [0x{0:X}], eax", m_threadIdPointer.ToInt64()));

	// Signal event reached
	asmNotifier.AddRange(Utils::Assembly::Generate_SetEvent(m_notifierReachedRemoteHandle));

	// Wait for notify handled
	asmNotifier.AddRange(Utils::Assembly::Generate_WaitForSingleObject(m_notifierHandledRemoteHandle, INFINITE));

	// Return
	asmNotifier.Add(String::Format("@notifier_exit:"));
	asmNotifier.Add(String::Format("retn"));
#endif

	Hook->Process->Yasm->Inject(asmNotifier.ToArray(), m_entryPointPointer);
}

void MyMemory::Hooks::CallbackNotifier::WaitForEvents()
{
	while (!m_stopRequired)
	{
		if (WaitForSingleObject(m_notifierReachedLocalHandle.ToPointer(), 1000) == WAIT_OBJECT_0)
		{
			NotifyArgs^ notifyArgs = gcnew NotifyArgs(Hook->BaseRegisters, Hook->Process->MemoryManager->Read<unsigned long>(m_threadIdPointer));
			Monitor::Enter(m_callbacks);
			for each (NotifyCallback^ callback in m_callbacks)
			{
				callback(notifyArgs);
			}
			Monitor::Exit(m_callbacks);
			SetEvent(m_notifierHandledLocalHandle.ToPointer());
		}
	}
}

MyMemory::Hooks::CallbackNotifier::NotifyArgs::NotifyArgs(MyMemory::Structures::BaseRegisters registers, unsigned long threadId)
{
	m_registers = registers;
	m_threadId = threadId;
}

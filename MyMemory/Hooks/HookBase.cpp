#include <Pch/StdAfx.h>

MyMemory::Hooks::HookBase::HookBase(RemoteProcess^ remoteProcess)
{
	remoteProcess->HooksManager->m_hooks->Add(this);
	m_callbacks = gcnew List<MyMemory::Hooks::CallbackBase^>();
	m_remoteProcess = remoteProcess;

	m_hookBaseAllocatedMemory = remoteProcess->MemoryManager->AllocateMemory(0x10000);
	m_registersPointer = m_hookBaseAllocatedMemory->AllocateOfChunk<Structures::BaseRegisters>("Registers");
	m_callbacksPointer = m_hookBaseAllocatedMemory->AllocateOfChunk("Callbacks", IntPtr::Size * 100);
	m_hookMutexHandle = Utils::Assembly::Remote_CreateMutex(remoteProcess, nullptr, false);

	if (!m_hookMutexHandle.ToPointer())
		throw gcnew Exception("Unable to create mutex for hook !");

	m_executor = gcnew MyMemory::Hooks::CallbackExecutor(this);
	m_notifier = gcnew MyMemory::Hooks::CallbackNotifier(this);

	AddCallback(m_executor);
	AddCallback(m_notifier);

}

MyMemory::Hooks::HookBase::~HookBase()
{

	if (m_remoteProcess->HooksManager->m_hooks->Contains(this))
		m_remoteProcess->HooksManager->m_hooks->Remove(this);

	delete m_hookBaseAllocatedMemory;

	while (m_callbacks->Count > 0)
		delete m_callbacks[0];
	
}

void MyMemory::Hooks::HookBase::AddCallback(MyMemory::Hooks::CallbackBase^ callback)
{
	if (!m_callbacks->Contains(callback))
	{
		m_callbacks->Add(callback);
		UpdateCallbacks();
	}
}

void MyMemory::Hooks::HookBase::RemoveCallback(MyMemory::Hooks::CallbackBase^ callback)
{
	if (m_callbacks->Contains(callback))
	{
		m_callbacks->Remove(callback);
		UpdateCallbacks();
	}
}

array<String^>^ MyMemory::Hooks::HookBase::GenerateSaveRegisters()
{
	return Utils::Assembly::Generate_SaveRegisters(m_registersPointer);
}

array<String^>^ MyMemory::Hooks::HookBase::GenerateInvokeCallbacks()
{
	List<String^> asmInvokeCallbacks;

#if _WIN64 || __amd64__
	asmInvokeCallbacks.Add(String::Format("push rax"));
	asmInvokeCallbacks.Add(String::Format("push rcx"));
	asmInvokeCallbacks.Add(String::Format("mov rcx, 0x{0:X}", m_callbacksPointer.ToInt64()));

	asmInvokeCallbacks.Add(String::Format("@callbacks_start:"));
	asmInvokeCallbacks.Add(String::Format("mov rax, [rcx]"));
	asmInvokeCallbacks.Add(String::Format("cmp rax, 0"));
	asmInvokeCallbacks.Add(String::Format("je @callbacks_end"));
	asmInvokeCallbacks.Add(String::Format("push rcx"));
	asmInvokeCallbacks.Add(String::Format("call rax"));
	asmInvokeCallbacks.Add(String::Format("pop rcx"));
	asmInvokeCallbacks.Add(String::Format("add rcx, 8"));
	asmInvokeCallbacks.Add(String::Format("jmp @callbacks_start"));

	asmInvokeCallbacks.Add(String::Format("@callbacks_end:"));
	asmInvokeCallbacks.Add(String::Format("pop rcx"));
	asmInvokeCallbacks.Add(String::Format("pop rax"));
#else
	asmInvokeCallbacks.Add(String::Format("push eax"));
	asmInvokeCallbacks.Add(String::Format("push ecx"));
	asmInvokeCallbacks.Add(String::Format("mov ecx, 0x{0:X}", m_callbacksPointer.ToInt64()));

	asmInvokeCallbacks.Add(String::Format("@callbacks_start:"));
	asmInvokeCallbacks.Add(String::Format("mov eax, [ecx]"));
	asmInvokeCallbacks.Add(String::Format("cmp eax, 0"));
	asmInvokeCallbacks.Add(String::Format("je @callbacks_end"));
	asmInvokeCallbacks.Add(String::Format("push ecx"));
	asmInvokeCallbacks.Add(String::Format("call eax"));
	asmInvokeCallbacks.Add(String::Format("pop ecx"));
	asmInvokeCallbacks.Add(String::Format("add ecx, 4"));
	asmInvokeCallbacks.Add(String::Format("jmp @callbacks_start"));

	asmInvokeCallbacks.Add(String::Format("@callbacks_end:"));
	asmInvokeCallbacks.Add(String::Format("pop ecx"));
	asmInvokeCallbacks.Add(String::Format("pop eax"));
#endif

	return asmInvokeCallbacks.ToArray();
}

void MyMemory::Hooks::HookBase::UpdateCallbacks()
{
	List<byte> callbacksPtr;
	for each (MyMemory::Hooks::CallbackBase^ callback in m_callbacks)
	{
		if (callback->EntryPointPointer.ToPointer())
		{
#if _WIN64 || __amd64__
			callbacksPtr.AddRange(BitConverter::GetBytes(callback->EntryPointPointer.ToInt64()));
#else
			callbacksPtr.AddRange(BitConverter::GetBytes(callback->EntryPointPointer.ToInt32()));
#endif
		}
	}
#if _WIN64 || __amd64__
	callbacksPtr.AddRange(BitConverter::GetBytes((unsigned long long)0));
#else
	callbacksPtr.AddRange(BitConverter::GetBytes((unsigned int)0));
#endif
	Process->MemoryManager->WriteBytes(m_callbacksPointer, callbacksPtr.ToArray());
}

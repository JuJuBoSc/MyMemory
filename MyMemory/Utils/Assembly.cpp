#include <Pch/StdAfx.h>


array<String^>^ MyMemory::Utils::Assembly::Generate_SaveRegisters(IntPtr pBaseRegisters)
{
	List<String^> asmSaveRegisters;

#if _WIN64 || __amd64__
	// This is definitely ugly but I couldn't find another way to do what I want to achieve in x64 :/

	// Push all registers onto the stack
	asmSaveRegisters.Add(String::Format("push rsp"));
	asmSaveRegisters.Add(String::Format("push rbp"));
	asmSaveRegisters.Add(String::Format("push rax"));
	asmSaveRegisters.Add(String::Format("push rbx"));
	asmSaveRegisters.Add(String::Format("push rcx"));
	asmSaveRegisters.Add(String::Format("push rdx"));
	asmSaveRegisters.Add(String::Format("push rsi"));
	asmSaveRegisters.Add(String::Format("push rdi"));
	asmSaveRegisters.Add(String::Format("push r8"));
	asmSaveRegisters.Add(String::Format("push r9"));
	asmSaveRegisters.Add(String::Format("push r10"));
	asmSaveRegisters.Add(String::Format("push r11"));
	asmSaveRegisters.Add(String::Format("push r12"));
	asmSaveRegisters.Add(String::Format("push r13"));
	asmSaveRegisters.Add(String::Format("push r14"));
	asmSaveRegisters.Add(String::Format("push r15"));

	// Copy them in our structures
	asmSaveRegisters.Add(String::Format("lea rsi, [rsp]"));
	asmSaveRegisters.Add(String::Format("mov rdi, 0x{0:X}", pBaseRegisters.ToInt64()));
	asmSaveRegisters.Add(String::Format("mov ecx, 0x10"));
	asmSaveRegisters.Add(String::Format("rep movsq"));

	// Pop all registers from the stack
	asmSaveRegisters.Add(String::Format("pop r15"));
	asmSaveRegisters.Add(String::Format("pop r14"));
	asmSaveRegisters.Add(String::Format("pop r13"));
	asmSaveRegisters.Add(String::Format("pop r12"));
	asmSaveRegisters.Add(String::Format("pop r11"));
	asmSaveRegisters.Add(String::Format("pop r10"));
	asmSaveRegisters.Add(String::Format("pop r9"));
	asmSaveRegisters.Add(String::Format("pop r8"));
	asmSaveRegisters.Add(String::Format("pop rdi"));
	asmSaveRegisters.Add(String::Format("pop rsi"));
	asmSaveRegisters.Add(String::Format("pop rdx"));
	asmSaveRegisters.Add(String::Format("pop rcx"));
	asmSaveRegisters.Add(String::Format("pop rbx"));
	asmSaveRegisters.Add(String::Format("pop rax"));
	asmSaveRegisters.Add(String::Format("pop rbp"));
	asmSaveRegisters.Add(String::Format("pop rsp"));
#else
	// x86 look's easy lmao
	asmSaveRegisters.Add(String::Format("mov [0x{0:X}], eax", (pBaseRegisters + 0x0).ToInt64()));
	asmSaveRegisters.Add(String::Format("mov [0x{0:X}], ebx", (pBaseRegisters + 0x4).ToInt64()));
	asmSaveRegisters.Add(String::Format("mov [0x{0:X}], ecx", (pBaseRegisters + 0x8).ToInt64()));
	asmSaveRegisters.Add(String::Format("mov [0x{0:X}], edx", (pBaseRegisters + 0xC).ToInt64()));
	asmSaveRegisters.Add(String::Format("mov [0x{0:X}], edi", (pBaseRegisters + 0x10).ToInt64()));
	asmSaveRegisters.Add(String::Format("mov [0x{0:X}], esi", (pBaseRegisters + 0x14).ToInt64()));
	asmSaveRegisters.Add(String::Format("mov [0x{0:X}], ebp", (pBaseRegisters + 0x18).ToInt64()));
	asmSaveRegisters.Add(String::Format("mov [0x{0:X}], esp", (pBaseRegisters + 0x1C).ToInt64()));
#endif

	return asmSaveRegisters.ToArray();
}

array<String^>^ MyMemory::Utils::Assembly::Generate_PushRegistersAndFlags()
{
	List<String^> asmPushRegistersAndFlags;

#if _WIN64 || __amd64__
	asmPushRegistersAndFlags.Add("push rax");
	asmPushRegistersAndFlags.Add("push rsp");
	asmPushRegistersAndFlags.Add("push rsp");
	asmPushRegistersAndFlags.Add("push rcx");
	asmPushRegistersAndFlags.Add("push rdx");
	asmPushRegistersAndFlags.Add("push rbx");
	asmPushRegistersAndFlags.Add("push rbp");
	asmPushRegistersAndFlags.Add("push rsi");
	asmPushRegistersAndFlags.Add("push rdi");
	asmPushRegistersAndFlags.Add("push r8");
	asmPushRegistersAndFlags.Add("push r9");
	asmPushRegistersAndFlags.Add("push r10");
	asmPushRegistersAndFlags.Add("push r11");
	asmPushRegistersAndFlags.Add("push r12");
	asmPushRegistersAndFlags.Add("push r13");
	asmPushRegistersAndFlags.Add("push r14");
	asmPushRegistersAndFlags.Add("push r15");
	asmPushRegistersAndFlags.Add("pushfq");
	asmPushRegistersAndFlags.Add("lea rsp,[rsp-16*16]");
	asmPushRegistersAndFlags.Add("vmovdqu [rsp+16*0],xmm0");
	asmPushRegistersAndFlags.Add("vmovdqu [rsp+16*1],xmm1");
	asmPushRegistersAndFlags.Add("vmovdqu [rsp+16*2],xmm2");
	asmPushRegistersAndFlags.Add("vmovdqu [rsp+16*3],xmm3");
	asmPushRegistersAndFlags.Add("vmovdqu [rsp+16*4],xmm4");
	asmPushRegistersAndFlags.Add("vmovdqu [rsp+16*5],xmm5");
	asmPushRegistersAndFlags.Add("vmovdqu [rsp+16*6],xmm6");
	asmPushRegistersAndFlags.Add("vmovdqu [rsp+16*7],xmm7");
	asmPushRegistersAndFlags.Add("vmovdqu [rsp+16*8],xmm8");
	asmPushRegistersAndFlags.Add("vmovdqu [rsp+16*9],xmm9");
	asmPushRegistersAndFlags.Add("vmovdqu [rsp+16*10],xmm10");
	asmPushRegistersAndFlags.Add("vmovdqu [rsp+16*11],xmm11");
	asmPushRegistersAndFlags.Add("vmovdqu [rsp+16*12],xmm12");
	asmPushRegistersAndFlags.Add("vmovdqu [rsp+16*13],xmm13");
	asmPushRegistersAndFlags.Add("vmovdqu [rsp+16*14],xmm14");
	asmPushRegistersAndFlags.Add("vmovdqu [rsp+16*15],xmm15");
#else
	asmPushRegistersAndFlags.Add("pushad");
	asmPushRegistersAndFlags.Add("pushfd");
#endif

	return asmPushRegistersAndFlags.ToArray();
}

array<String^>^ MyMemory::Utils::Assembly::Generate_PopRegistersAndFlags()
{
	List<String^> asmPopRegistersAndFlags;

#if _WIN64 || __amd64__
	asmPopRegistersAndFlags.Add("vmovdqu xmm15,[rsp+16*15]");
	asmPopRegistersAndFlags.Add("vmovdqu xmm14,[rsp+16*14]");
	asmPopRegistersAndFlags.Add("vmovdqu xmm13,[rsp+16*13]");
	asmPopRegistersAndFlags.Add("vmovdqu xmm12,[rsp+16*12]");
	asmPopRegistersAndFlags.Add("vmovdqu xmm11,[rsp+16*11]");
	asmPopRegistersAndFlags.Add("vmovdqu xmm10,[rsp+16*10]");
	asmPopRegistersAndFlags.Add("vmovdqu xmm9,[rsp+16*9]");
	asmPopRegistersAndFlags.Add("vmovdqu xmm8,[rsp+16*8]");
	asmPopRegistersAndFlags.Add("vmovdqu xmm7,[rsp+16*7]");
	asmPopRegistersAndFlags.Add("vmovdqu xmm6,[rsp+16*6]");
	asmPopRegistersAndFlags.Add("vmovdqu xmm5,[rsp+16*5]");
	asmPopRegistersAndFlags.Add("vmovdqu xmm4,[rsp+16*4]");
	asmPopRegistersAndFlags.Add("vmovdqu xmm3,[rsp+16*3]");
	asmPopRegistersAndFlags.Add("vmovdqu xmm2,[rsp+16*2]");
	asmPopRegistersAndFlags.Add("vmovdqu xmm1,[rsp+16*1]");
	asmPopRegistersAndFlags.Add("vmovdqu xmm0,[rsp+16*0]");
	asmPopRegistersAndFlags.Add("lea rsp,[rsp+16*16]");
	asmPopRegistersAndFlags.Add("popfq");
	asmPopRegistersAndFlags.Add("pop r15");
	asmPopRegistersAndFlags.Add("pop r14");
	asmPopRegistersAndFlags.Add("pop r13");
	asmPopRegistersAndFlags.Add("pop r12");
	asmPopRegistersAndFlags.Add("pop r11");
	asmPopRegistersAndFlags.Add("pop r10");
	asmPopRegistersAndFlags.Add("pop r9");
	asmPopRegistersAndFlags.Add("pop r8");
	asmPopRegistersAndFlags.Add("pop rdi");
	asmPopRegistersAndFlags.Add("pop rsi");
	asmPopRegistersAndFlags.Add("pop rbp");
	asmPopRegistersAndFlags.Add("pop rbx");
	asmPopRegistersAndFlags.Add("pop rdx");
	asmPopRegistersAndFlags.Add("pop rcx");
	asmPopRegistersAndFlags.Add("pop rsp");
	asmPopRegistersAndFlags.Add("pop rsp");
	asmPopRegistersAndFlags.Add("pop rax");
#else
	asmPopRegistersAndFlags.Add("popfd");
	asmPopRegistersAndFlags.Add("popad");
#endif

	return asmPopRegistersAndFlags.ToArray();
}

array<String^>^ MyMemory::Utils::Assembly::Generate_RtlEnterCriticalSection(IntPtr lpCriticalSection)
{
	List<String^> asmRtlEnterCriticalSection;

	void* pRtlEnterCriticalSection = GetProcAddress(GetModuleHandleA("ntdll.dll"), "RtlEnterCriticalSection");

	if (!pRtlEnterCriticalSection)
		throw gcnew Exception("Unable to find RtlEnterCriticalSection");

#if _WIN64 || __amd64__
	asmRtlEnterCriticalSection.Add(String::Format("sub rsp, 0x20"));
	asmRtlEnterCriticalSection.Add(String::Format("mov rcx, 0x{0:X}", lpCriticalSection.ToInt64()));
	asmRtlEnterCriticalSection.Add(String::Format("mov rax, 0x{0:X}", (Int64)pRtlEnterCriticalSection));
	asmRtlEnterCriticalSection.Add(String::Format("call rax"));
	asmRtlEnterCriticalSection.Add(String::Format("add rsp, 0x20"));
#else
	asmRtlEnterCriticalSection.Add(String::Format("push 0x{0:X}", lpCriticalSection.ToInt64()));
	asmRtlEnterCriticalSection.Add(String::Format("call 0x{0:X}", (Int64)pRtlEnterCriticalSection));
#endif

	return asmRtlEnterCriticalSection.ToArray();
}

array<String^>^ MyMemory::Utils::Assembly::Generate_RtlLeaveCriticalSection(IntPtr lpCriticalSection)
{
	List<String^> asmRtlLeaveCriticalSection;

	void* pRtlLeaveCriticalSection = GetProcAddress(GetModuleHandleA("ntdll.dll"), "RtlLeaveCriticalSection");

	if (!pRtlLeaveCriticalSection)
		throw gcnew Exception("Unable to find RtlLeaveCriticalSection");

#if _WIN64 || __amd64__
	asmRtlLeaveCriticalSection.Add(String::Format("sub rsp, 0x20"));
	asmRtlLeaveCriticalSection.Add(String::Format("mov rcx, 0x{0:X}", lpCriticalSection.ToInt64()));
	asmRtlLeaveCriticalSection.Add(String::Format("mov rax, 0x{0:X}", (Int64)pRtlLeaveCriticalSection));
	asmRtlLeaveCriticalSection.Add(String::Format("call rax"));
	asmRtlLeaveCriticalSection.Add(String::Format("add rsp, 0x20"));
#else
	asmRtlLeaveCriticalSection.Add(String::Format("push 0x{0:X}", lpCriticalSection.ToInt64()));
	asmRtlLeaveCriticalSection.Add(String::Format("call 0x{0:X}", (Int64)pRtlLeaveCriticalSection));
#endif

	return asmRtlLeaveCriticalSection.ToArray();
}

array<String^>^ MyMemory::Utils::Assembly::Generate_WaitForSingleObject(IntPtr hHandle, unsigned long dwMilliseconds)
{
	List<String^> asmWaitForSingleObject;

	void* pWaitForSingleObject = GetProcAddress(GetModuleHandleA("kernel32.dll"), "WaitForSingleObject");

	if (!pWaitForSingleObject)
		throw gcnew Exception("Unable to find WaitForSingleObject");

#if _WIN64 || __amd64__
	asmWaitForSingleObject.Add(String::Format("sub rsp, 0x20"));
	asmWaitForSingleObject.Add(String::Format("mov edx, 0x{0:X}", dwMilliseconds));
	asmWaitForSingleObject.Add(String::Format("mov rcx, 0x{0:X}", hHandle.ToInt64()));
	asmWaitForSingleObject.Add(String::Format("mov rax, 0x{0:X}", (Int64)pWaitForSingleObject));
	asmWaitForSingleObject.Add(String::Format("call rax"));
	asmWaitForSingleObject.Add(String::Format("add rsp, 0x20"));
#else
	asmWaitForSingleObject.Add(String::Format("push 0x{0:X}", dwMilliseconds));
	asmWaitForSingleObject.Add(String::Format("push 0x{0:X}", hHandle.ToInt64()));
	asmWaitForSingleObject.Add(String::Format("call 0x{0:X}", (Int64)pWaitForSingleObject));
#endif

	return asmWaitForSingleObject.ToArray();
}

array<String^>^ MyMemory::Utils::Assembly::Generate_ReleaseMutex(IntPtr hMutex)
{
	List<String^> asmReleaseMutex;

	void* pReleaseMutex = GetProcAddress(GetModuleHandleA("kernel32.dll"), "ReleaseMutex");

	if (!pReleaseMutex)
		throw gcnew Exception("Unable to find ReleaseMutex");

#if _WIN64 || __amd64__
	asmReleaseMutex.Add(String::Format("sub rsp, 0x20"));
	asmReleaseMutex.Add(String::Format("mov rcx, 0x{0:X}", hMutex.ToInt64()));
	asmReleaseMutex.Add(String::Format("mov rax, 0x{0:X}", (Int64)pReleaseMutex));
	asmReleaseMutex.Add(String::Format("call rax"));
	asmReleaseMutex.Add(String::Format("add rsp, 0x20"));
#else
	asmReleaseMutex.Add(String::Format("push 0x{0:X}", hMutex.ToInt64()));
	asmReleaseMutex.Add(String::Format("call 0x{0:X}", (Int64)pReleaseMutex));
#endif

	return asmReleaseMutex.ToArray();
}

array<String^>^ MyMemory::Utils::Assembly::Generate_SetEvent(IntPtr hEvent)
{
	List<String^> asmSetEvent;

	void* pSetEvent = GetProcAddress(GetModuleHandleA("kernel32.dll"), "SetEvent");

	if (!pSetEvent)
		throw gcnew Exception("Unable to find SetEvent");

#if _WIN64 || __amd64__
	asmSetEvent.Add(String::Format("sub rsp, 0x20"));
	asmSetEvent.Add(String::Format("mov rcx, 0x{0:X}", hEvent.ToInt64()));
	asmSetEvent.Add(String::Format("mov rax, 0x{0:X}", (Int64)pSetEvent));
	asmSetEvent.Add(String::Format("call rax"));
	asmSetEvent.Add(String::Format("add rsp, 0x20"));
#else
	asmSetEvent.Add(String::Format("push 0x{0:X}", hEvent.ToInt64()));
	asmSetEvent.Add(String::Format("call 0x{0:X}", (Int64)pSetEvent));
#endif

	return asmSetEvent.ToArray();
}

array<String^>^ MyMemory::Utils::Assembly::Generate_GetCurrentThreadId()
{
	List<String^> asmGetCurrentThreadId;

#if _WIN64 || __amd64__
	asmGetCurrentThreadId.Add(String::Format("mov rax, [gs:0x30]"));
	asmGetCurrentThreadId.Add(String::Format("mov eax, [rax+0x48]"));
#else
	asmGetCurrentThreadId.Add(String::Format("mov eax, [fs:0x24]"));
#endif

	return asmGetCurrentThreadId.ToArray();
}

bool MyMemory::Utils::Assembly::Remote_RtlInitializeCriticalSection(MyMemory::RemoteProcess^ process, IntPtr lpCriticalSection)
{
	List<String^> asmRtlInitializeCriticalSection;

	void* pRtlInitializeCriticalSection = GetProcAddress(GetModuleHandleA("ntdll.dll"), "RtlInitializeCriticalSection");

	if (!pRtlInitializeCriticalSection)
		throw gcnew Exception("Unable to find RtlInitializeCriticalSection");

#if _WIN64 || __amd64__
	asmRtlInitializeCriticalSection.Add(String::Format("sub rsp, 0x20"));
	asmRtlInitializeCriticalSection.Add(String::Format("mov rcx, 0x" + lpCriticalSection.ToString("X")));
	asmRtlInitializeCriticalSection.Add(String::Format("mov rax, 0x" + IntPtr(pRtlInitializeCriticalSection).ToString("X")));
	asmRtlInitializeCriticalSection.Add(String::Format("call rax"));
	asmRtlInitializeCriticalSection.Add(String::Format("add rsp, 0x20"));
	asmRtlInitializeCriticalSection.Add(String::Format("retn"));
#else
	asmRtlInitializeCriticalSection.Add(String::Format("push 0x{0:X}", lpCriticalSection.ToInt64()));
	asmRtlInitializeCriticalSection.Add(String::Format("call 0x{0:X}", (Int64)pRtlInitializeCriticalSection));
	asmRtlInitializeCriticalSection.Add(String::Format("retn"));
#endif

	return !process->Yasm->InjectAndExecute(asmRtlInitializeCriticalSection.ToArray()).ToPointer();
}

System::IntPtr MyMemory::Utils::Assembly::Remote_CreateEvent(MyMemory::RemoteProcess^ process, String^ eventName, bool manualReset, bool initialState)
{
	List<String^> asmCreateEvent;

	void* pCreateEventA = GetProcAddress(GetModuleHandleA("kernel32.dll"), "CreateEventA");

	if (!pCreateEventA)
		throw gcnew Exception("Unable to find CreateEventA");

	MyMemory::Memory::RemoteAllocatedMemory^ pEventName = process->MemoryManager->AllocateMemory(eventName->Length + 1);
	process->MemoryManager->WriteString(pEventName->Pointer, Encoding::ASCII, eventName);

#if _WIN64 || __amd64__
	asmCreateEvent.Add(String::Format("sub rsp, 0x20"));
	asmCreateEvent.Add(String::Format("mov r9, 0x{0:X}", pEventName->Pointer.ToInt64()));
	asmCreateEvent.Add(String::Format("mov r8d, {0}", Convert::ToInt32(initialState)));
	asmCreateEvent.Add(String::Format("mov edx, {0}", Convert::ToInt32(manualReset)));
	asmCreateEvent.Add(String::Format("xor rcx, rcx"));
	asmCreateEvent.Add(String::Format("mov rax, 0x{0:X}", (Int64)pCreateEventA));
	asmCreateEvent.Add(String::Format("call rax"));
	asmCreateEvent.Add(String::Format("add rsp, 0x20"));
	asmCreateEvent.Add(String::Format("retn"));
#else
	asmCreateEvent.Add(String::Format("push 0x{0:X}", pEventName->Pointer.ToInt64()));
	asmCreateEvent.Add(String::Format("push 0x{0:X}", Convert::ToInt32(initialState)));
	asmCreateEvent.Add(String::Format("push 0x{0:X}", Convert::ToInt32(manualReset)));
	asmCreateEvent.Add(String::Format("push 0"));
	asmCreateEvent.Add(String::Format("call 0x{0:X}", (Int64)pCreateEventA));
	asmCreateEvent.Add(String::Format("retn"));
#endif

	IntPtr hEvent = process->Yasm->InjectAndExecute(asmCreateEvent.ToArray());

	delete pEventName;

	return hEvent;

}

System::IntPtr MyMemory::Utils::Assembly::Remote_CreateMutex(MyMemory::RemoteProcess^ process, String^ mutexName, bool initialOwner)
{

	List<String^> asmCreateMutex;

	void* pCreateMutexA = GetProcAddress(GetModuleHandleA("kernel32.dll"), "CreateMutexA");

	if (!pCreateMutexA)
		throw gcnew Exception("Unable to find CreateMutexA");

	MyMemory::Memory::RemoteAllocatedMemory^ pMutexName = nullptr;

	if (mutexName)
	{
		pMutexName = process->MemoryManager->AllocateMemory(mutexName->Length + 1);
		process->MemoryManager->WriteString(pMutexName->Pointer, Encoding::ASCII, mutexName);
	}

#if _WIN64 || __amd64__
	asmCreateMutex.Add(String::Format("sub rsp, 0x20"));
	asmCreateMutex.Add(String::Format("mov r8, 0x{0:X}", pMutexName ? pMutexName->Pointer.ToInt64() : 0));
	asmCreateMutex.Add(String::Format("mov edx, {0}", Convert::ToInt32(initialOwner)));
	asmCreateMutex.Add(String::Format("xor rcx, rcx"));
	asmCreateMutex.Add(String::Format("mov rax, 0x{0:X}", (Int64)pCreateMutexA));
	asmCreateMutex.Add(String::Format("call rax"));
	asmCreateMutex.Add(String::Format("add rsp, 0x20"));
	asmCreateMutex.Add(String::Format("retn"));
#else
	asmCreateMutex.Add(String::Format("push 0x{0:X}", pMutexName ? pMutexName->Pointer.ToInt64() : 0));
	asmCreateMutex.Add(String::Format("push 0x{0:X}", Convert::ToInt32(initialOwner)));
	asmCreateMutex.Add(String::Format("push 0"));
	asmCreateMutex.Add(String::Format("call 0x{0:X}", (Int64)pCreateMutexA));
	asmCreateMutex.Add(String::Format("retn"));
#endif

	IntPtr hMutex = process->Yasm->InjectAndExecute(asmCreateMutex.ToArray());

	if (pMutexName)
	{
		delete pMutexName;
	}

	return hMutex;

}

#include <Pch/StdAfx.h>

MyMemory::Threads::RemoteThread::RemoteThread(RemoteProcess^ process, unsigned long threadId)
{

	m_remoteProcess = process;
	m_threadId = threadId;

	CLIENT_ID clientId;
	clientId.UniqueProcess = NULL;
	clientId.UniqueThread = (PVOID)threadId;

	OBJECT_ATTRIBUTES oa;
	InitializeObjectAttributes(&oa, NULL, 0, NULL, NULL);

	HANDLE hThread;
	if (NT_SUCCESS(NtOpenThread(&hThread, THREAD_ALL_ACCESS, &oa, &clientId)))
		m_threadHandle = hThread;

}

MyMemory::Threads::RemoteThread::RemoteThread(RemoteProcess^ process, IntPtr threadHandle)
{
	m_remoteProcess = process;
	m_threadHandle = threadHandle.ToPointer();
	m_threadId = GetThreadId(m_threadHandle);
}

MyMemory::Threads::RemoteThread::~RemoteThread()
{
	if (m_threadHandle)
	{
		NtClose(m_threadHandle);
		m_threadHandle = nullptr;
	}
}

bool MyMemory::Threads::RemoteThread::SuspendThread()
{
	return NT_SUCCESS(NtSuspendThread(m_threadHandle, nullptr));
}

bool MyMemory::Threads::RemoteThread::ResumeThread()
{
	return NT_SUCCESS(NtResumeThread(m_threadHandle, nullptr));
}

bool MyMemory::Threads::RemoteThread::GetThreadContext([Out] MyMemory::Structures::ThreadContext% context)
{
	MyMemory::Structures::ThreadContext ctx;
	ctx.ContextFlags = CONTEXT_ALL;
	void* pCtx = _aligned_malloc(Utils::MarshalCache<MyMemory::Structures::ThreadContext>::Size, 16);
	try
	{
		Marshal::StructureToPtr(ctx, IntPtr(pCtx), false);
		NTSTATUS result = NtGetContextThread(m_threadHandle, (LPCONTEXT)pCtx);
		if (NT_SUCCESS(result))
		{
			context = (MyMemory::Structures::ThreadContext)Marshal::PtrToStructure(IntPtr(pCtx), Utils::MarshalCache<MyMemory::Structures::ThreadContext>::RealType);
			return true;
		}
	}
	finally
	{
		_aligned_free(pCtx);
	}
	return false;
}

bool MyMemory::Threads::RemoteThread::SetThreadContext(MyMemory::Structures::ThreadContext% context)
{
	void* pCtx = _aligned_malloc(Utils::MarshalCache<MyMemory::Structures::ThreadContext>::Size, 16);
	try
	{
		Marshal::StructureToPtr(context, IntPtr(pCtx), false);
		return NT_SUCCESS(NtSetContextThread(m_threadHandle, (LPCONTEXT)pCtx));
	}
	finally
	{
		_aligned_free(pCtx);
	}
}

bool MyMemory::Threads::RemoteThread::TerminateThread(unsigned long exitCode)
{
	return ::TerminateThread(m_threadHandle, exitCode) != 0;
}

void MyMemory::Threads::RemoteThread::Join()
{
	NtWaitForSingleObject(m_threadHandle, FALSE, NULL);
}

unsigned long MyMemory::Threads::RemoteThread::ExitCodeThread::get()
{
	unsigned long exitCode;
	GetExitCodeThread(m_threadHandle, &exitCode);
	return exitCode;
}

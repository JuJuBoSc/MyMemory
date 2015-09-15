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
	ctx.ContextFlags = CONTEXT_FULL;
	array<byte> ^ bufferCtx = gcnew array<byte>(Utils::MarshalCache<MyMemory::Structures::ThreadContext>::Size);
	pin_ptr<Byte> pBufferCtx = &bufferCtx[0];
	Marshal::StructureToPtr(ctx, IntPtr(pBufferCtx), false);
	if (::GetThreadContext(m_threadHandle, (LPCONTEXT)pBufferCtx))
	{
		context = (MyMemory::Structures::ThreadContext)Marshal::PtrToStructure(IntPtr(pBufferCtx), Utils::MarshalCache<MyMemory::Structures::ThreadContext>::RealType);
		return true;
	}
	return false;
}

bool MyMemory::Threads::RemoteThread::SetThreadContext(MyMemory::Structures::ThreadContext% context)
{
	array<byte> ^ bufferCtx = gcnew array<byte>(Utils::MarshalCache<MyMemory::Structures::ThreadContext>::Size);
	pin_ptr<Byte> pBufferCtx = &bufferCtx[0];
	Marshal::StructureToPtr(context, IntPtr(pBufferCtx), false);
	return ::SetThreadContext(m_threadHandle, (LPCONTEXT)pBufferCtx) != 0;
}

unsigned long MyMemory::Threads::RemoteThread::ExitCodeThread::get()
{
	unsigned long exitCode;
	GetExitCodeThread(m_threadHandle, &exitCode);
	return exitCode;
}

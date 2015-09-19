#include <Pch/StdAfx.h>

MyMemory::Threads::RemoteThread::RemoteThread(RemoteProcess^ process, unsigned long threadId)
{
	m_remoteProcess = process;
	m_threadId = threadId;
	m_threadHandle = OpenThread(THREAD_ALL_ACCESS, false, threadId);
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
		CloseHandle(m_threadHandle);
		m_threadHandle = nullptr;
	}
}

bool MyMemory::Threads::RemoteThread::SuspendThread()
{
	return ::SuspendThread(m_threadHandle) >= 0;
}

bool MyMemory::Threads::RemoteThread::ResumeThread()
{
	return ::ResumeThread(m_threadHandle) >= 0;
}

bool MyMemory::Threads::RemoteThread::GetThreadContext([Out] MyMemory::Structures::ThreadContext% context)
{
	MyMemory::Structures::ThreadContext ctx;
	ctx.ContextFlags = CONTEXT_ALL;
	void* pCtx = _aligned_malloc(Utils::MarshalCache<MyMemory::Structures::ThreadContext>::Size, 16);
	try
	{
		Marshal::StructureToPtr(ctx, IntPtr(pCtx), false);
		if (::GetThreadContext(m_threadHandle, (LPCONTEXT)pCtx))
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
		return ::SetThreadContext(m_threadHandle, (LPCONTEXT)pCtx) == TRUE;
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
	WaitForSingleObject(m_threadHandle, INFINITE);
}

unsigned long MyMemory::Threads::RemoteThread::ExitCodeThread::get()
{
	unsigned long exitCode;
	GetExitCodeThread(m_threadHandle, &exitCode);
	return exitCode;
}

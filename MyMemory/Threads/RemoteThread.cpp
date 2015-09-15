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

#include <Pch/StdAfx.h>

MyMemory::Threads::ThreadsManager::ThreadsManager(RemoteProcess^ remoteProcess)
{
	m_remoteProcess = remoteProcess;
	m_mainThread = GetMainThread();
}

MyMemory::Threads::RemoteThread^ MyMemory::Threads::ThreadsManager::GetMainThread()
{
	MyMemory::Threads::RemoteThread^ mainTread = nullptr;
	::FILETIME smallestFtCreate;

	for each (MyMemory::Threads::RemoteThread^ thread in Threads)
	{
		::FILETIME ftCreate, ftExit, ftKernel, ftUser;
		if (GetThreadTimes(thread->ThreadHandle.ToPointer(), &ftCreate, &ftExit, &ftKernel, &ftUser))
		{

			if (mainTread == nullptr)
			{
				mainTread = thread;
				smallestFtCreate = ftCreate;
				continue;
			}

			if (CompareFileTime(&ftCreate, &smallestFtCreate) == -1)
			{
				mainTread = thread;
				smallestFtCreate = ftCreate;
			}

		}
	}

	return mainTread;
}

List<MyMemory::Threads::RemoteThread^>^ MyMemory::Threads::ThreadsManager::Threads::get()
{

	List<MyMemory::Threads::RemoteThread^>^ results = gcnew List<MyMemory::Threads::RemoteThread^>();

	HANDLE hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	if (hThreadSnap == INVALID_HANDLE_VALUE)
		return results;

	THREADENTRY32 te32;
	te32.dwSize = sizeof(THREADENTRY32);

	if (!Thread32First(hThreadSnap, &te32))
		return results;

	do
	{
		if (te32.th32OwnerProcessID == m_remoteProcess->ProcessId)
		{
			results->Add(gcnew MyMemory::Threads::RemoteThread(m_remoteProcess, te32.th32ThreadID));
		}
	} while (Thread32Next(hThreadSnap, &te32));

	return results;

}

#include <Pch/StdAfx.h>

using namespace System::Runtime::InteropServices;

MyMemory::RemoteProcess::RemoteProcess()
{
#if _WIN64 || __amd64__
	m_yasm = gcnew MyMemory::Assembly::Yasm(this, 20480, 64);
#else
	m_yasm = gcnew MyMemory::Assembly::Yasm(this, 20480, 32);
#endif
}

MyMemory::RemoteProcess::~RemoteProcess()
{
	Close();
}


bool MyMemory::RemoteProcess::Open(unsigned int processId)
{

	CLIENT_ID clientId;
	clientId.UniqueProcess = (PVOID)processId;
	clientId.UniqueThread = NULL;

	OBJECT_ATTRIBUTES oa;
	InitializeObjectAttributes(&oa, NULL, 0, NULL, NULL);

	HANDLE hProcess;
	if (!NT_SUCCESS(NtOpenProcess(&hProcess, PROCESS_ALL_ACCESS, &oa, &clientId)))
		return false;

	bool remoteIs64Bits = false;

	if (Environment::Is64BitOperatingSystem)
	{
		BOOL Wow64Process;
		IsWow64Process(hProcess, &Wow64Process);
		remoteIs64Bits = !Wow64Process;
	}

	if (remoteIs64Bits != Environment::Is64BitProcess)
		throw gcnew NotSupportedException("Attempt to attach on different architecture process !");

	m_processHandle = hProcess;
	m_processId = processId;
	m_memoryManager = gcnew MyMemory::Memory::MemoryManager(this);
	m_modulesManager = gcnew MyMemory::Modules::ModulesManager(this);
	m_threadsManager = gcnew MyMemory::Threads::ThreadsManager(this);

	return true;

}

void MyMemory::RemoteProcess::Close()
{

	if (m_processHandle)
	{
		NtClose(m_processHandle);
		m_processHandle = nullptr;
	}

	m_processId = 0;

}


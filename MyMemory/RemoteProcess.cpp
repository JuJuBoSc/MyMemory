#include <Pch/StdAfx.h>

using namespace System::Runtime::InteropServices;

MyMemory::RemoteProcess::RemoteProcess(unsigned int processId)
{
#if _WIN64 || __amd64__
	m_yasm = gcnew MyMemory::Assembly::Yasm(this, 20480, 64);
#else
	m_yasm = gcnew MyMemory::Assembly::Yasm(this, 20480, 32);
#endif

	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, processId);

	if (!hProcess)
		throw gcnew Exception("Unable to open process !");

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
	m_hooksManager = gcnew MyMemory::Hooks::HooksManager(this);

}

MyMemory::RemoteProcess::~RemoteProcess()
{

	delete m_hooksManager;
	delete m_modulesManager;
	delete m_threadsManager;
	delete m_memoryManager;

	if (m_processHandle)
		CloseHandle(m_processHandle);

}

#include <Pch/StdAfx.h>

using namespace System::Runtime::InteropServices;

MyMemory::RemoteProcess::RemoteProcess()
{
	m_yasm = gcnew MyMemory::Assembly::Yasm(this, 20480, Environment::Is64BitProcess ? 64 : 32);
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

	if (Environment::Is64BitOperatingSystem)
	{
		BOOL Wow64Process;
		IsWow64Process(hProcess, &Wow64Process);
		m_is64BitsProcess = !Wow64Process;
	}
	else
	{
		m_is64BitsProcess = false;
	}

	if (m_is64BitsProcess != Environment::Is64BitProcess)
		throw gcnew NotSupportedException("Attempt to attach on different architecture process !");

	m_processHandle = hProcess;
	m_processId = processId;
	m_mainThread = GetMainThread();

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

generic <typename T> T MyMemory::RemoteProcess::Read(IntPtr lpAddress)
{
	if (Utils::MarshalCache<T>::TypeRequireMarshal)
	{
		int size = Utils::MarshalCache<T>::Size;
		array<byte>^ buffer = gcnew array<byte>(size);
		pin_ptr<Byte> pBuffer = &buffer[0];
		NtReadVirtualMemory(m_processHandle, (void*)lpAddress, pBuffer, size, NULL);
		return (T)Marshal::PtrToStructure(IntPtr(pBuffer), Utils::MarshalCache<T>::RealType);
	}
	else
	{
		T result = T();
		NtReadVirtualMemory(m_processHandle, (void*)lpAddress, &result, Utils::MarshalCache<T>::Size, NULL);
		return result;
	}
}

array<byte>^ MyMemory::RemoteProcess::ReadBytes(IntPtr lpAddress, int count)
{
	array<byte>^ buffer = gcnew array<byte>(count);
	pin_ptr<Byte> pBuffer = &buffer[0];
	NtReadVirtualMemory(m_processHandle, (void*)lpAddress, pBuffer, count, NULL);
	return buffer;
}

String^ MyMemory::RemoteProcess::ReadString(IntPtr lpAddress, Encoding^ encoding, int maxLength)
{

	array<byte>^ buffer = ReadBytes(lpAddress, maxLength * encoding->GetByteCount("a"));
	String^ ret = encoding->GetString(buffer);

	int nullCharIdx = ret->IndexOf('\0');
	if (nullCharIdx != -1)
		ret = ret->Remove(nullCharIdx);

	return ret;

}

generic <typename T> bool MyMemory::RemoteProcess::Write(IntPtr lpAddress, T value)
{
	int size = Utils::MarshalCache<T>::Size;
	unsigned long sizeU = Utils::MarshalCache<T>::SizeU;
	unsigned long oldProtection;
	void* ptr = lpAddress.ToPointer();
	NtProtectVirtualMemory(m_processHandle, &ptr, &sizeU, PAGE_EXECUTE_READWRITE, &oldProtection);
	try
	{
		if (Utils::MarshalCache<T>::TypeRequireMarshal)
		{
			array<byte> ^ buffer = gcnew array<byte>(size);
			pin_ptr<Byte> pBuffer = &buffer[0];
			Marshal::StructureToPtr(value, IntPtr(pBuffer), false);
			return NT_SUCCESS(NtWriteVirtualMemory(m_processHandle, (void*)lpAddress, pBuffer, size, NULL));
		}
		else
		{
			return NT_SUCCESS(NtWriteVirtualMemory(m_processHandle, (void*)lpAddress, &value, size, NULL));
		}
	}
	finally
	{
		NtProtectVirtualMemory(m_processHandle, &ptr, &sizeU, oldProtection, &oldProtection);
	}
}

bool MyMemory::RemoteProcess::WriteBytes(IntPtr lpAddress, array<byte>^ bBuffer)
{
	Memory::RemoteMemoryProtection^ protect = ProtectMemory(lpAddress, bBuffer->Length, MyMemory::Enumerations::MemoryProtectionFlags::ExecuteReadWrite);
	unsigned long oldProtection;
	void* ptr = lpAddress.ToPointer();
	unsigned long size = bBuffer->Length;
	NtProtectVirtualMemory(m_processHandle, &ptr, &size, PAGE_EXECUTE_READWRITE, &oldProtection);
	try
	{
		pin_ptr<Byte> pBuffer = &bBuffer[0];
		return NT_SUCCESS(NtWriteVirtualMemory(m_processHandle, (void*)lpAddress, pBuffer, bBuffer->Length, NULL));
	}
	finally
	{
		NtProtectVirtualMemory(m_processHandle, &ptr, &size, oldProtection, &oldProtection);
	}
}

bool MyMemory::RemoteProcess::WriteString(IntPtr lpAddress, Encoding^ encoding, String^ value)
{
	if (value->Length == 0)
		value = "\0";

	if (value[value->Length - 1] != '\0')
		value += '\0';

	return WriteBytes(lpAddress, encoding->GetBytes(value));
}

MyMemory::Modules::RemoteModule^ MyMemory::RemoteProcess::GetModule(String^ name)
{

	HMODULE hMods[1024];
	WCHAR buffer[MAX_PATH];
	DWORD cbNeeded;
	pin_ptr<const wchar_t> wName = PtrToStringChars(name);

	if (EnumProcessModules(m_processHandle, hMods, sizeof(hMods), &cbNeeded))
	{
		for (unsigned int i = 0; i < (cbNeeded / sizeof(HMODULE)); i++)
		{
			HMODULE hModule = hMods[i];
			if (hModule && GetModuleBaseNameW(m_processHandle, hModule, buffer, sizeof(buffer)) && _wcsicmp(buffer, wName) == 0)
			{
				return gcnew MyMemory::Modules::RemoteModule(this, IntPtr(hModule));
			}
		}
	}

	return nullptr;

}

MyMemory::Memory::RemoteMemoryProtection^ MyMemory::RemoteProcess::ProtectMemory(IntPtr pointer, unsigned long size, Enumerations::MemoryProtectionFlags newProtection)
{
	return gcnew MyMemory::Memory::RemoteMemoryProtection(this, pointer, size, newProtection);
}

MyMemory::Threads::RemoteThread^ MyMemory::RemoteProcess::GetMainThread()
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

MyMemory::Modules::RemoteModule^ MyMemory::RemoteProcess::default::get(String^ s)
{
	return GetModule(s);
}

List<MyMemory::Modules::RemoteModule^>^ MyMemory::RemoteProcess::Modules::get()
{

	List<MyMemory::Modules::RemoteModule^>^ results = gcnew List<MyMemory::Modules::RemoteModule^>();
	
	HMODULE hMods[1024];
	DWORD cbNeeded;

	if (EnumProcessModules(m_processHandle, hMods, sizeof(hMods), &cbNeeded))
	{
		for (unsigned int i = 0; i < (cbNeeded / sizeof(HMODULE)); i++)
		{
			HMODULE hModule = hMods[i];
			if (hModule)
			{
				results->Add(gcnew MyMemory::Modules::RemoteModule(this, IntPtr(hModule)));
			}
		}
	}

	return results;

}

List<MyMemory::Threads::RemoteThread^>^ MyMemory::RemoteProcess::Threads::get()
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
		if (te32.th32OwnerProcessID == m_processId)
		{
			results->Add(gcnew MyMemory::Threads::RemoteThread(this, te32.th32ThreadID));
		}
	} while (Thread32Next(hThreadSnap, &te32));

	return results;

}

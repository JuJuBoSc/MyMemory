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
	if (Utils::MarshalCache<T>::TypeRequireMarshal)
	{
		int size = Utils::MarshalCache<T>::Size;
		array<byte> ^ buffer = gcnew array<byte>(size);
		pin_ptr<Byte> pBuffer = &buffer[0];
		Marshal::StructureToPtr(value, IntPtr(pBuffer), false);
		return NT_SUCCESS(NtWriteVirtualMemory(m_processHandle, (void*)lpAddress, pBuffer, size, NULL));
	}
	else
	{
		return NT_SUCCESS(NtWriteVirtualMemory(m_processHandle, (void*)lpAddress, &value, Utils::MarshalCache<T>::Size, NULL));
	}
}

bool MyMemory::RemoteProcess::WriteBytes(IntPtr lpAddress, array<byte>^ bBuffer)
{
	pin_ptr<Byte> pBuffer = &bBuffer[0];
	return NT_SUCCESS(NtWriteVirtualMemory(m_processHandle, (void*)lpAddress, pBuffer, bBuffer->Length, NULL));
}

bool MyMemory::RemoteProcess::WriteString(IntPtr lpAddress, Encoding^ encoding, String^ value)
{
	if (value->Length == 0)
		value = "\0";

	if (value[value->Length - 1] != '\0')
		value += '\0';

	return WriteBytes(lpAddress, encoding->GetBytes(value));
}

List<MyMemory::Modules::RemoteModule^>^ MyMemory::RemoteProcess::Modules::get()
{

	List<MyMemory::Modules::RemoteModule^>^ results = gcnew List<MyMemory::Modules::RemoteModule^>();

	HMODULE hMods[1024];
	DWORD cbNeeded;

	if (EnumProcessModules(m_processHandle, hMods, sizeof(hMods), &cbNeeded))
	{
		for (int i = 0; i < (cbNeeded / sizeof(HMODULE)); i++)
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

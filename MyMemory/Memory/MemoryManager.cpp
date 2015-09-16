#include <Pch/StdAfx.h>

MyMemory::Memory::MemoryManager::MemoryManager(RemoteProcess^ remoteProcess)
{
	m_remoteProcess = remoteProcess;
}

generic <typename T> T MyMemory::Memory::MemoryManager::Read(IntPtr lpAddress)
{
	if (Utils::MarshalCache<T>::TypeRequireMarshal)
	{
		int size = Utils::MarshalCache<T>::Size;
		array<byte>^ buffer = gcnew array<byte>(size);
		pin_ptr<Byte> pBuffer = &buffer[0];
		NtReadVirtualMemory(this->m_remoteProcess->ProcessHandle.ToPointer(), (void*)lpAddress, pBuffer, size, NULL);
		return (T)Marshal::PtrToStructure(IntPtr(pBuffer), Utils::MarshalCache<T>::RealType);
	}
	else
	{
		T result = T();
		NtReadVirtualMemory(this->m_remoteProcess->ProcessHandle.ToPointer(), (void*)lpAddress, &result, Utils::MarshalCache<T>::Size, NULL);
		return result;
	}
}

array<byte>^ MyMemory::Memory::MemoryManager::ReadBytes(IntPtr lpAddress, int count)
{
	array<byte>^ buffer = gcnew array<byte>(count);
	pin_ptr<Byte> pBuffer = &buffer[0];
	NtReadVirtualMemory(this->m_remoteProcess->ProcessHandle.ToPointer(), (void*)lpAddress, pBuffer, count, NULL);
	return buffer;
}

String^ MyMemory::Memory::MemoryManager::ReadString(IntPtr lpAddress, Encoding^ encoding, int maxLength)
{

	array<byte>^ buffer = ReadBytes(lpAddress, maxLength * encoding->GetByteCount("a"));
	String^ ret = encoding->GetString(buffer);

	int nullCharIdx = ret->IndexOf('\0');
	if (nullCharIdx != -1)
		ret = ret->Remove(nullCharIdx);

	return ret;

}

generic <typename T> bool MyMemory::Memory::MemoryManager::Write(IntPtr lpAddress, T value)
{
	int size = Utils::MarshalCache<T>::Size;
	unsigned long sizeU = Utils::MarshalCache<T>::SizeU;
	unsigned long oldProtection;
	void* ptr = lpAddress.ToPointer();
	NtProtectVirtualMemory(this->m_remoteProcess->ProcessHandle.ToPointer(), &ptr, &sizeU, PAGE_EXECUTE_READWRITE, &oldProtection);
	try
	{
		if (Utils::MarshalCache<T>::TypeRequireMarshal)
		{
			array<byte> ^ buffer = gcnew array<byte>(size);
			pin_ptr<Byte> pBuffer = &buffer[0];
			Marshal::StructureToPtr(value, IntPtr(pBuffer), false);
			return NT_SUCCESS(NtWriteVirtualMemory(this->m_remoteProcess->ProcessHandle.ToPointer(), (void*)lpAddress, pBuffer, size, NULL));
		}
		else
		{
			return NT_SUCCESS(NtWriteVirtualMemory(this->m_remoteProcess->ProcessHandle.ToPointer(), (void*)lpAddress, &value, size, NULL));
		}
	}
	finally
	{
		NtProtectVirtualMemory(this->m_remoteProcess->ProcessHandle.ToPointer(), &ptr, &sizeU, oldProtection, &oldProtection);
	}
}

bool MyMemory::Memory::MemoryManager::WriteBytes(IntPtr lpAddress, array<byte>^ bBuffer)
{
	Memory::RemoteMemoryProtection^ protect = ProtectMemory(lpAddress, bBuffer->Length, MyMemory::Enumerations::MemoryProtectionFlags::ExecuteReadWrite);
	unsigned long oldProtection;
	void* ptr = lpAddress.ToPointer();
	unsigned long size = bBuffer->Length;
	NtProtectVirtualMemory(this->m_remoteProcess->ProcessHandle.ToPointer(), &ptr, &size, PAGE_EXECUTE_READWRITE, &oldProtection);
	try
	{
		pin_ptr<Byte> pBuffer = &bBuffer[0];
		return NT_SUCCESS(NtWriteVirtualMemory(this->m_remoteProcess->ProcessHandle.ToPointer(), (void*)lpAddress, pBuffer, bBuffer->Length, NULL));
	}
	finally
	{
		NtProtectVirtualMemory(this->m_remoteProcess->ProcessHandle.ToPointer(), &ptr, &size, oldProtection, &oldProtection);
	}
}

bool MyMemory::Memory::MemoryManager::WriteString(IntPtr lpAddress, Encoding^ encoding, String^ value)
{
	if (value->Length == 0)
		value = "\0";

	if (value[value->Length - 1] != '\0')
		value += '\0';

	return WriteBytes(lpAddress, encoding->GetBytes(value));
}

MyMemory::Memory::RemoteMemoryProtection^ MyMemory::Memory::MemoryManager::ProtectMemory(IntPtr pointer, unsigned long size, Enumerations::MemoryProtectionFlags newProtection)
{
	return gcnew MyMemory::Memory::RemoteMemoryProtection(this->m_remoteProcess, pointer, size, newProtection);
}

MyMemory::Memory::RemoteAllocatedMemory^ MyMemory::Memory::MemoryManager::AllocateMemory(unsigned long size, Enumerations::MemoryProtectionFlags protection)
{
	void* ptr = nullptr;
	NtAllocateVirtualMemory(this->m_remoteProcess->ProcessHandle.ToPointer(), &ptr, 0, &size, MEM_COMMIT | MEM_RESERVE, (ULONG)protection);
	return gcnew MyMemory::Memory::RemoteAllocatedMemory(this->m_remoteProcess, IntPtr(ptr), size, protection);
}
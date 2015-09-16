#include <Pch/StdAfx.h>

MyMemory::Memory::RemoteAllocatedMemory::RemoteAllocatedMemory(RemoteProcess^ process, IntPtr pointer, unsigned long size, MyMemory::Enumerations::MemoryProtectionFlags protection)
	: MyMemory::Memory::RemoteRegion(process, pointer, size)
{
	m_protection = protection;
	m_allocated = gcnew Dictionary<String^, int>();
	m_currentOffset = 0;
}

MyMemory::Memory::RemoteAllocatedMemory::~RemoteAllocatedMemory()
{
	if (Pointer.ToPointer())
	{
		PVOID ptr = this->Pointer.ToPointer();
		ULONG regionSize = this->Size;
		NtFreeVirtualMemory(this->Process->ProcessHandle.ToPointer(), &ptr, &regionSize, MEM_RELEASE);
		this->Pointer = IntPtr(nullptr);
	}
}

IntPtr MyMemory::Memory::RemoteAllocatedMemory::AllocateOfChunk(String^ name, int size)
{
	unsigned long value = (unsigned long)m_currentOffset;
	m_allocated->Add(name, value);
	m_currentOffset = size + m_currentOffset;
	int aligned = m_currentOffset % 4;
	if (aligned != 0)
	{
		m_currentOffset = m_currentOffset - aligned + 4;
	}
	return Pointer + value;
}

generic <typename T> IntPtr MyMemory::Memory::RemoteAllocatedMemory::AllocateOfChunk(String^ name)
{
	return AllocateOfChunk(name, Utils::MarshalCache<T>::Size);
}

System::IntPtr MyMemory::Memory::RemoteAllocatedMemory::GetAllocatedChunk(String^ name)
{
	int offset = m_allocated[name];
	return Pointer + offset;
}

generic <typename T> bool MyMemory::Memory::RemoteAllocatedMemory::Write(String^ name, int offset, T value)
{
	IntPtr ptr = GetAllocatedChunk(name) + offset;
	return Process->Write<T>(ptr, value);
}

bool MyMemory::Memory::RemoteAllocatedMemory::WriteBytes(String^ name, int offset, array<byte>^ bBuffer)
{
	IntPtr ptr = GetAllocatedChunk(name) + offset;
	return Process->WriteBytes(ptr, bBuffer);
}

bool MyMemory::Memory::RemoteAllocatedMemory::WriteString(String^ name, int offset, Encoding^ encoding, String^ value)
{
	IntPtr ptr = GetAllocatedChunk(name) + offset;
	return Process->WriteString(ptr, encoding, value);
}

generic <typename T> T MyMemory::Memory::RemoteAllocatedMemory::Read(String^ name, int offset)
{
	IntPtr ptr = GetAllocatedChunk(name) + offset;
	return Process->Read<T>(ptr);
}

array<byte>^ MyMemory::Memory::RemoteAllocatedMemory::ReadBytes(String^ name, int offset, int size)
{
	IntPtr ptr = GetAllocatedChunk(name) + offset;
	return Process->ReadBytes(ptr, size);
}

String^ MyMemory::Memory::RemoteAllocatedMemory::ReadString(String^ name, int offset, Encoding^ encoding, int maxLength)
{
	IntPtr ptr = GetAllocatedChunk(name) + offset;
	return Process->ReadString(ptr, encoding, maxLength);
}

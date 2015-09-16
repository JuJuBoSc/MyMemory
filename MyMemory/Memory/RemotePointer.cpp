#include <Pch/StdAfx.h>

MyMemory::Memory::RemotePointer::RemotePointer(RemoteProcess^ remoteProcess, IntPtr pointer)
{
	m_remoteProcess = remoteProcess;
	m_pointer = pointer;
	m_nativePointer = pointer.ToPointer();
}

generic <typename T> T MyMemory::Memory::RemotePointer::Read(int offset)
{
	return Process->MemoryManager->Read<T>(m_pointer + offset);
}

array<byte>^ MyMemory::Memory::RemotePointer::ReadBytes(int offset, int count)
{
	return Process->MemoryManager->ReadBytes(m_pointer + offset, count);
}

String^ MyMemory::Memory::RemotePointer::ReadString(int offset, Encoding^ encoding, int maxLength)
{
	return Process->MemoryManager->ReadString(m_pointer + offset, encoding, maxLength);
}

generic <typename T> bool MyMemory::Memory::RemotePointer::Write(int offset, T value)
{
	return Process->MemoryManager->Write<T>(m_pointer + offset, value);
}

bool MyMemory::Memory::RemotePointer::WriteBytes(int offset, array<byte>^ bBuffer)
{
	return Process->MemoryManager->WriteBytes(m_pointer + offset, bBuffer);
}

bool MyMemory::Memory::RemotePointer::WriteString(int offset, Encoding^ encoding, String^ value)
{
	return Process->MemoryManager->WriteString(m_pointer + offset, encoding, value);
}

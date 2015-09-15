#include <Pch/StdAfx.h>

MyMemory::Memory::RemotePointer::RemotePointer(RemoteProcess^ remoteProcess, IntPtr pointer)
{
	m_remoteProcess = remoteProcess;
	m_pointer = pointer;
	m_nativePointer = pointer.ToPointer();
}

generic <typename T> T MyMemory::Memory::RemotePointer::Read(int offset)
{
	return m_remoteProcess->Read<T>(m_pointer + offset);
}

array<byte>^ MyMemory::Memory::RemotePointer::ReadBytes(int offset, int count)
{
	return m_remoteProcess->ReadBytes(m_pointer + offset, count);
}

String^ MyMemory::Memory::RemotePointer::ReadString(int offset, Encoding^ encoding, int maxLength)
{
	return m_remoteProcess->ReadString(m_pointer + offset, encoding, maxLength);
}

generic <typename T> bool MyMemory::Memory::RemotePointer::Write(int offset, T value)
{
	return m_remoteProcess->Write<T>(m_pointer + offset, value);
}

bool MyMemory::Memory::RemotePointer::WriteBytes(int offset, array<byte>^ bBuffer)
{
	return m_remoteProcess->WriteBytes(m_pointer + offset, bBuffer);
}

bool MyMemory::Memory::RemotePointer::WriteString(int offset, Encoding^ encoding, String^ value)
{
	return m_remoteProcess->WriteString(m_pointer + offset, encoding, value);
}

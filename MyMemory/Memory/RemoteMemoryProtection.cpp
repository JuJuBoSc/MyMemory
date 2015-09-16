#include <Pch/StdAfx.h>

MyMemory::Memory::RemoteMemoryProtection::RemoteMemoryProtection(RemoteProcess^ process, IntPtr pointer, unsigned long size, MyMemory::Enumerations::MemoryProtectionFlags newProtection)
	: MyMemory::Memory::RemoteRegion(process, pointer, size)
{
	ULONG old;
	PVOID ptr = pointer.ToPointer();
	NtProtectVirtualMemory(process->ProcessHandle.ToPointer(), &ptr, &size, (ULONG)newProtection, &old);
	m_oldProtection = (MyMemory::Enumerations::MemoryProtectionFlags)old;
}

MyMemory::Memory::RemoteMemoryProtection::~RemoteMemoryProtection()
{
	ULONG old;
	ULONG size = this->Size;
	PVOID ptr = Pointer.ToPointer();
	NtProtectVirtualMemory(this->Process->ProcessHandle.ToPointer(), &ptr, &size, (ULONG)m_oldProtection, &old);
}

#include <Pch/StdAfx.h>

MyMemory::Memory::RemoteMemoryProtection::RemoteMemoryProtection(RemoteProcess^ process, IntPtr pointer, unsigned long size, MyMemory::Enumerations::MemoryProtectionFlags newProtection)
	: MyMemory::Memory::RemoteRegion(process, pointer, size)
{
	DWORD old;
	PVOID ptr = pointer.ToPointer();
	VirtualProtectEx(Process->ProcessHandle.ToPointer(), pointer.ToPointer(), size, (DWORD)newProtection, &old);
	m_oldProtection = (MyMemory::Enumerations::MemoryProtectionFlags)old;
}

MyMemory::Memory::RemoteMemoryProtection::~RemoteMemoryProtection()
{
	ULONG old;
	VirtualProtectEx(Process->ProcessHandle.ToPointer(), Pointer.ToPointer(), Size, (DWORD)OldProtection, &old);
}

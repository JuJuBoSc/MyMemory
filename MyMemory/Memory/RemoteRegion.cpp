#include <Pch/StdAfx.h>

MyMemory::Memory::RemoteRegion::RemoteRegion(RemoteProcess^ process, IntPtr pointer, unsigned long size)
	: MyMemory::Memory::RemotePointer(process, pointer)
{
	m_size = size;
}

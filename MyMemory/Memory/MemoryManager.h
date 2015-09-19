#pragma once

using namespace System;

namespace MyMemory {
	namespace Memory {

		public ref class MemoryManager
		{

			// Members
		private:
			RemoteProcess^ m_remoteProcess;

			// Methods
		public:
			MemoryManager(RemoteProcess^ remoteProcess);
			generic <typename T> T Read(IntPtr lpAddress);
			array<byte>^ ReadBytes(IntPtr lpAddress, int count);
			String^ ReadString(IntPtr lpAddress, Encoding^ encoding, int maxLength);
			String^ ReadString(IntPtr lpAddress, Encoding^ encoding) { return ReadString(lpAddress, encoding, 128); }
			String^ ReadString(IntPtr lpAddress) { return ReadString(lpAddress, Encoding::UTF8); }
			generic <typename T> bool Write(IntPtr lpAddress, T value);
			bool WriteBytes(IntPtr lpAddress, array<byte>^ bBuffer);
			bool WriteString(IntPtr lpAddress, Encoding^ encoding, String^ value);
			bool WriteString(IntPtr lpAddress, String^ value) { return WriteString(lpAddress, Encoding::UTF8, value); }
			MyMemory::Memory::RemoteMemoryProtection^ ProtectMemory(IntPtr pointer, unsigned long size, Enumerations::MemoryProtectionFlags newProtection);
			MyMemory::Memory::RemoteAllocatedMemory^ AllocateMemory(unsigned long size, Enumerations::MemoryProtectionFlags protection);
			MyMemory::Memory::RemoteAllocatedMemory^ AllocateMemory(unsigned long size) { return AllocateMemory(size, MyMemory::Enumerations::MemoryProtectionFlags::ExecuteReadWrite); }
			IntPtr AllocateRawMemory(unsigned long size, Enumerations::MemoryProtectionFlags protection);
			IntPtr AllocateRawMemory(unsigned long size) { return AllocateRawMemory(size, MyMemory::Enumerations::MemoryProtectionFlags::ExecuteReadWrite); }
			bool FreeRawMemory(IntPtr lpAddress);


			// Properties
		public:
			property RemoteProcess^ Process {
				RemoteProcess^ get() { return m_remoteProcess; }
			}


		};

	}
}
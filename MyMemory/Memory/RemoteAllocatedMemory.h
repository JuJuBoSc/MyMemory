#pragma once

using namespace System;

namespace MyMemory {
	namespace Memory {

		public ref class RemoteAllocatedMemory : public MyMemory::Memory::RemoteRegion
		{

			// Members
		private:
			MyMemory::Enumerations::MemoryProtectionFlags m_protection;
			Dictionary<String^, int>^ m_allocated;
			int m_currentOffset;

			// Methods
		public:
			RemoteAllocatedMemory(RemoteProcess^ process, IntPtr pointer, unsigned long size, MyMemory::Enumerations::MemoryProtectionFlags protection);
			~RemoteAllocatedMemory();
			IntPtr AllocateOfChunk(String^ name, int size);
			generic <typename T> IntPtr AllocateOfChunk(String^ name);
			IntPtr GetAllocatedChunk(String^ name);
			generic <typename T> bool Write(String^ name, int offset, T value);
			generic <typename T> bool Write(String^ name, T value) { return Write<T>(name, 0, value); }
			generic <typename T> T Read(String^ name, int offset);
			generic <typename T> T Read(String^ name) { return Read<T>(name, 0); }
			bool WriteBytes(String^ name, int offset, array<byte>^ bBuffer);
			bool WriteBytes(String^ name, array<byte>^ bBuffer) { return WriteBytes(name, 0, bBuffer); }
			array<byte>^ ReadBytes(String^ name, int offset, int size);
			array<byte>^ ReadBytes(String^ name, int size) { return ReadBytes(name, 0, size); }
			bool WriteString(String^ name, int offset, Encoding^ encoding, String^ value);
			bool WriteString(String^ name, int offset, String^ value) { return WriteString(name, offset, Encoding::UTF8, value); }
			bool WriteString(String^ name, Encoding^ encoding, String^ value) { return WriteString(name, 0, encoding, value); }
			bool WriteString(String^ name, String^ value) { return WriteString(name, Encoding::UTF8, value); }
			String^ ReadString(String^ name, int offset, Encoding^ encoding, int maxLength);
			String^ ReadString(String^ name, int offset, int maxLength) { return ReadString(name, offset, Encoding::UTF8, maxLength); }
			String^ ReadString(String^ name, int offset, Encoding^ encoding) { return ReadString(name, offset, encoding, 128); }
			String^ ReadString(String^ name, int offset) { return ReadString(name, offset, Encoding::UTF8, 128); }
			String^ ReadString(String^ name) { return ReadString(name, 0); }


			// Properties
		public:
			property IntPtr default[String^]{
				IntPtr get(String^ name) { return GetAllocatedChunk(name); }
			}
			property MyMemory::Enumerations::MemoryProtectionFlags Protection {
				MyMemory::Enumerations::MemoryProtectionFlags get() { return m_protection; }
			}

		};

	}
}
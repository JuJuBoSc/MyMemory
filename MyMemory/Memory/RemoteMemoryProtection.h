#pragma once

using namespace System;

namespace MyMemory {
	namespace Memory {

		public ref class RemoteMemoryProtection : public MyMemory::Memory::RemoteRegion
		{

			// Members
		private:
			MyMemory::Enumerations::MemoryProtectionFlags m_newProtection;
			MyMemory::Enumerations::MemoryProtectionFlags m_oldProtection;

			// Methods
		public:
			RemoteMemoryProtection(RemoteProcess^ process, IntPtr pointer, unsigned long size, MyMemory::Enumerations::MemoryProtectionFlags newProtection);
			~RemoteMemoryProtection();

			// Properties
		public:
			property MyMemory::Enumerations::MemoryProtectionFlags NewProtection {
				MyMemory::Enumerations::MemoryProtectionFlags get() { return m_newProtection; }
			}
			property MyMemory::Enumerations::MemoryProtectionFlags OldProtection {
				MyMemory::Enumerations::MemoryProtectionFlags get() { return m_oldProtection; }
			}

		};

	}
}
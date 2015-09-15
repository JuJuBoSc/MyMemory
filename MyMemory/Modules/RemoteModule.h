#pragma once

using namespace System;

namespace MyMemory {
	namespace Modules {

		public ref class RemoteModule : public MyMemory::Memory::RemotePointer
		{

			// Members
		private:
			HMODULE m_hModule;
			String^ m_baseName;
			String^ m_fullName;
			unsigned long m_sizeOfImage;
			IntPtr m_baseAddress;
			IntPtr m_entryPointAddress;

			// Methods
		public:
			RemoteModule(RemoteProcess^ process, IntPtr hModule);

			// Properties
		public:
			property IntPtr BaseAddress {
				IntPtr get() { return m_baseAddress; }
			}
			property IntPtr EntryPointAddress {
				IntPtr get() { return m_entryPointAddress; }
			}
			property String^ BaseName {
				String^ get() { return m_baseName; }
			}
			property String^ FileName {
				String^ get() { return m_fullName; }
			}
			property unsigned long Size {
				unsigned long get() { return m_sizeOfImage; }
			}

		};

	}
}
#pragma once

using namespace System;

namespace MyMemory {
	namespace Modules {

		public ref class RemoteModule : public MyMemory::Memory::RemotePointer
		{

			// Members
		private:
			HMODULE m_hModule;

			// Methods
		public:
			RemoteModule(RemoteProcess^ process, IntPtr hModule);

			// Properties
		public:
			property IntPtr BaseAddress {
				IntPtr get() { return this->Pointer; }
			}

		};

	}
}
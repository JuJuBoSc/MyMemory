#pragma once

using namespace System;

namespace MyMemory {
	namespace Memory {

		public ref class RemoteRegion : public RemotePointer
		{

			// Members
		private:
			unsigned long m_size;

			// Methods
		public:
			RemoteRegion(RemoteProcess^ process, IntPtr pointer, unsigned long size);

			// Properties
		public:
			property unsigned long BaseAddress {
				unsigned long get() { return m_size; }
			}

		};

	}
}
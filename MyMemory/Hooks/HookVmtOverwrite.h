#pragma once

using namespace System;

namespace MyMemory {
	namespace Hooks {

		public ref class HookVmtOverwrite : public HookBase
		{

			// Members
		private:
			bool m_isApplied;
			IntPtr m_vtableAddress;
			int m_functionIndex;
			IntPtr m_originalAddress;
			MyMemory::Memory::RemoteAllocatedMemory^ m_hookVmtAllocatedMemory;
			IntPtr m_detourPointer;

			// Methods
		public:
			HookVmtOverwrite(RemoteProcess^ remoteProcess, IntPtr vTable, int functionIndex);
			~HookVmtOverwrite();

			// Methods
		private:
			void GenerateDetour();

			// Properties
		public:

			// Override methods
		public:
			virtual void Apply() override;
			virtual void Remove() override;

			// Abstract properties
		public:
			property bool IsApplied {
				virtual bool get() override { return m_isApplied; }
			}

		};

	}
}
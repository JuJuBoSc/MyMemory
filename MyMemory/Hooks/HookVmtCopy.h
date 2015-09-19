#pragma once

using namespace System;

namespace MyMemory {
	namespace Hooks {

		public ref class HookVmtCopy : public HookBase
		{

			// Members
		private:
			bool m_isApplied;
			IntPtr m_objectAddress;
			int m_functionIndex;
			IntPtr m_originalAddress;
			IntPtr m_originalVtableAddress;
			MyMemory::Memory::RemoteAllocatedMemory^ m_hookVmtAllocatedMemory;
			IntPtr m_fakeVtablePointer;
			IntPtr m_detourPointer;
			bool m_isOriginalVtable;

			// Methods
		public:
			HookVmtCopy(RemoteProcess^ remoteProcess, IntPtr pObject, int functionIndex);
			~HookVmtCopy();

			// Methods
		private:
			unsigned long CountVmt(IntPtr pVtable);
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
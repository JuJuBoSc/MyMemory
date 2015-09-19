#pragma once

using namespace System;

namespace MyMemory {
	namespace Hooks {
		ref class HookBase;
		ref class HookJmp;
		ref class HookVmtOverwrite;
		ref class HookVmtCopy;
	}
}

namespace MyMemory {
	namespace Hooks {

		public ref class HooksManager
		{

			// Members
		private:
			RemoteProcess^ m_remoteProcess;

			// Members
		internal:
			List<MyMemory::Hooks::HookBase^>^ m_hooks;
			Dictionary<IntPtr, IntPtr> m_fakeVtables; // fake, original

			// Methods
		public:
			HooksManager(RemoteProcess^ remoteProcess);
			~HooksManager();
			MyMemory::Hooks::HookJmp^ CreateJmpHook(IntPtr lpAddress, int length);
			MyMemory::Hooks::HookJmp^ CreateAndApplyJmpHook(IntPtr lpAddress, int length);
			MyMemory::Hooks::HookVmtOverwrite^ CreateVmtOverwriteHook(IntPtr vTable, int functionIndex);
			MyMemory::Hooks::HookVmtOverwrite^ CreateAndApplyVmtOverwriteHook(IntPtr vTable, int functionIndex);
			MyMemory::Hooks::HookVmtCopy^ CreateVmtCopyHook(IntPtr pObject, int functionIndex);
			MyMemory::Hooks::HookVmtCopy^ CreateAndApplyVmtCopyHook(IntPtr pObject, int functionIndex);

			// Properties
		public:
			property RemoteProcess^ Process {
				RemoteProcess^ get() { return m_remoteProcess; }
			}
			property array<MyMemory::Hooks::HookBase^>^ Hooks {
				array<MyMemory::Hooks::HookBase^>^ get() { return m_hooks->ToArray(); }
			}

		};

	}
}
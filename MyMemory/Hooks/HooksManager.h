#pragma once

using namespace System;

namespace MyMemory {
	namespace Hooks {
		ref class HookBase;
		ref class HookJmp;
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

			// Methods
		public:
			HooksManager(RemoteProcess^ remoteProcess);
			~HooksManager();
			MyMemory::Hooks::HookJmp^ CreateJmpHook(IntPtr lpAddress, int length);
			MyMemory::Hooks::HookJmp^ CreateAndApplyJmpHook(IntPtr lpAddress, int length);

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
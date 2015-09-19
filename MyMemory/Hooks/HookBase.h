#pragma once

using namespace System;

namespace MyMemory {
	namespace Hooks {
		ref class CallbackBase;
		ref class CallbackExecutor;
		ref class CallbackNotifier;
	}
}

namespace MyMemory {
	namespace Hooks {

		public ref class HookBase abstract
		{

			// Members
		private:
			RemoteProcess^ m_remoteProcess;
			List<MyMemory::Hooks::CallbackBase^>^ m_callbacks;
			MyMemory::Memory::RemoteAllocatedMemory^ m_hookBaseAllocatedMemory;
			IntPtr m_registersPointer;
			IntPtr m_callbacksPointer;
			MyMemory::Hooks::CallbackExecutor^ m_executor;
			MyMemory::Hooks::CallbackNotifier^ m_notifier;

			// Members
		protected:
			IntPtr m_hookMutexHandle;


			// Methods
		public:
			HookBase(RemoteProcess^ remoteProcess);
			~HookBase();
			void AddCallback(MyMemory::Hooks::CallbackBase^ callback);

			// Methods
		internal:
			void RemoveCallback(MyMemory::Hooks::CallbackBase^ callback);

			// Methods
		protected:
			array<String^>^ GenerateSaveRegisters();
			array<String^>^ GenerateInvokeCallbacks();

			// Methods
		private:
			void UpdateCallbacks();

			// Properties
		public:
			property RemoteProcess^ Process {
				RemoteProcess^ get() { return m_remoteProcess; }
			}
			property Structures::BaseRegisters BaseRegisters {
				Structures::BaseRegisters get() { return m_remoteProcess->MemoryManager->Read<Structures::BaseRegisters>(m_registersPointer); }
			}
			property MyMemory::Hooks::CallbackExecutor^ Executor {
				MyMemory::Hooks::CallbackExecutor^ get() { return m_executor; }
			}
			property MyMemory::Hooks::CallbackNotifier^ Notifier {
				MyMemory::Hooks::CallbackNotifier^ get() { return m_notifier; }
			}

			// Abstract methods
		public:
			virtual void Apply() = 0;
			virtual void Remove() = 0;

			// Abstract properties
		public:
			virtual property bool IsApplied {
				bool get() = 0;
			}

		};

	}
}
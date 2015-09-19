#pragma once

using namespace System;

namespace MyMemory {
	namespace Hooks {

		public ref class CallbackNotifier : public CallbackBase
		{

		public:
			ref class NotifyArgs
			{

			internal:
				NotifyArgs(MyMemory::Structures::BaseRegisters registers, unsigned long threadId);

				// Members
			private:
				MyMemory::Structures::BaseRegisters m_registers;
				unsigned long m_threadId;

				// Properties
			public:
				property MyMemory::Structures::BaseRegisters Registers {
					MyMemory::Structures::BaseRegisters get() { return m_registers; }
				}
				property unsigned long ThreadId {
					unsigned long get() { return m_threadId; }
				}

			};

			// Delegates
		public:
			delegate void NotifyCallback(NotifyArgs^ notifyArgs);


			// Members
		private:
			MyMemory::Memory::RemoteAllocatedMemory^ m_notifierAllocatedMemory;
			IntPtr m_entryPointPointer;
			IntPtr m_notifyEnabledPointer;
			IntPtr m_threadIdPointer;
			String^ m_notifierReachedEventName;
			String^ m_notifierHandledEventName;
			IntPtr m_notifierReachedRemoteHandle;
			IntPtr m_notifierHandledRemoteHandle;
			IntPtr m_notifierReachedLocalHandle;
			IntPtr m_notifierHandledLocalHandle;
			bool m_stopRequired;
			Thread^ m_threadWaitForEvents;
			List<NotifyCallback^>^ m_callbacks;

			// Methods
		public:
			CallbackNotifier(HookBase^ hook);
			~CallbackNotifier();
			bool AddCallback(NotifyCallback^ callback);
			bool RemoveCallback(NotifyCallback^ callback);

			// Methods
		private:
			void GenerateNotifier();
			void WaitForEvents();


			// Properties
		public:

			// Abstract properties
		public:
			property IntPtr EntryPointPointer {
				virtual IntPtr get() override { return m_entryPointPointer; }
			}

		};

	}
}
#pragma once

using namespace System;

namespace MyMemory {
	namespace Hooks {

		public ref class CallbackExecutor : public CallbackBase
		{

		public:
			ref class FrameLock
			{

			public:
				FrameLock(CallbackExecutor^ executor);
				~FrameLock();

			private:
				CallbackExecutor^ m_executor;

			};

			// Members
		protected:
			Mutex^ m_frameLockMutex;

			// Members
		private:
			MyMemory::Memory::RemoteAllocatedMemory^ m_executorAllocatedMemory;
			IntPtr m_entryPointPointer;
			IntPtr m_executeCodePointer;
			IntPtr m_executeReturnValuePointer;
			IntPtr m_executeFrameLockRequiredPointer;
			IntPtr m_executeFrameLockedPointer;
			String^ m_executeRequiredEventName;
			String^ m_executeFinishedEventName;
			IntPtr m_executeRequiredRemoteHandle;
			IntPtr m_executeFinishedRemoteHandle;
			IntPtr m_executeRequiredLocalHandle;
			IntPtr m_executeFinishedLocalHandle;

			// Methods
		protected:
			void SetFrameLock(bool value);

			// Methods
		public:
			CallbackExecutor(HookBase^ hook);
			~CallbackExecutor();
			generic <typename T> T Execute(array<String^>^ mnemonics);
			MyMemory::Hooks::CallbackExecutor::FrameLock^ LockFrame();

			// Methods
		private:
			void GenerateExecutor();


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
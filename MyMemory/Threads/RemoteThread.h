#pragma once

using namespace System;

namespace MyMemory {
	namespace Threads {

		public ref class RemoteThread
		{

			// Members
		private:
			RemoteProcess^ m_remoteProcess;
			unsigned long m_threadId;
			void* m_threadHandle;

			// Methods
		public:
			RemoteThread(RemoteProcess^ process, unsigned long threadId);
			RemoteThread(RemoteProcess^ process, IntPtr threadHandle);
			~RemoteThread();
			bool SuspendThread();
			bool ResumeThread();
			bool GetThreadContext([Out] MyMemory::Structures::ThreadContext% context);
			bool SetThreadContext(MyMemory::Structures::ThreadContext% context);
			bool TerminateThread(unsigned long exitCode);
			void Join();

			// Properties
		public:
			property RemoteProcess^ Process {
				RemoteProcess^ get() { return m_remoteProcess; }
			}
			property unsigned long ThreadId {
				unsigned long get() { return m_threadId; }
			}
			property IntPtr ThreadHandle {
				IntPtr get() { return IntPtr(m_threadHandle); }
			}
			property unsigned long ExitCodeThread {
				unsigned long get();
			}

		};

	}
}
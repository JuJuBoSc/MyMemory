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
			~RemoteThread();
			bool SuspendThread();
			bool ResumeThread();

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

		};

	}
}
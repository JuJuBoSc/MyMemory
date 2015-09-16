#pragma once

using namespace System;

namespace MyMemory {
	namespace Threads {

		public ref class ThreadsManager
		{

			// Members
		private:
			RemoteProcess^ m_remoteProcess;
			MyMemory::Threads::RemoteThread^ m_mainThread;

			// Methods
		public:
			ThreadsManager(RemoteProcess^ remoteProcess);
			MyMemory::Threads::RemoteThread^ CreateRemoteThread(IntPtr lpStartAddress, IntPtr parameter, Enumerations::ThreadCreationFlags threadCreationFlags);
			MyMemory::Threads::RemoteThread^ CreateRemoteThread(IntPtr lpStartAddress, IntPtr parameter) { return CreateRemoteThread(lpStartAddress, parameter, Enumerations::ThreadCreationFlags::Run); }
			MyMemory::Threads::RemoteThread^ CreateRemoteThread(IntPtr lpStartAddress) { return CreateRemoteThread(lpStartAddress, IntPtr(nullptr)); }
			
			// Methods
		private:
			MyMemory::Threads::RemoteThread^ GetMainThread();

			// Properties
		public:
			property RemoteProcess^ Process {
				RemoteProcess^ get() { return m_remoteProcess; }
			}
			property List<MyMemory::Threads::RemoteThread^>^ Threads {
				List<MyMemory::Threads::RemoteThread^>^ get();
			}
			property MyMemory::Threads::RemoteThread^ MainThread {
				MyMemory::Threads::RemoteThread^ get() { return m_mainThread; }
			}


		};

	}
}
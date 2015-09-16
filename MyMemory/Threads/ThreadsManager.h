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
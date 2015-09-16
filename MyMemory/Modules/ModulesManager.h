#pragma once

using namespace System;

namespace MyMemory {
	namespace Modules {

		public ref class ModulesManager
		{

			// Members
		private:
			RemoteProcess^ m_remoteProcess;
			RemoteModule^ m_mainModule;

			// Methods
		public:
			ModulesManager(RemoteProcess^ remoteProcess);
			MyMemory::Modules::RemoteModule^ GetModule(String^ name);

			// Methods
		private:
			MyMemory::Modules::RemoteModule^ GetMainModule();

			// Properties
		public:
			property RemoteProcess^ Process {
				RemoteProcess^ get() { return m_remoteProcess; }
			}
			property RemoteModule^ MainModule {
				RemoteModule^ get() { return m_mainModule; }
			}
			property List<MyMemory::Modules::RemoteModule^>^ Modules {
				List<MyMemory::Modules::RemoteModule^>^ get();
			}
			property MyMemory::Modules::RemoteModule^ default[String^]{
				MyMemory::Modules::RemoteModule^ get(String^);
			}


		};

	}
}
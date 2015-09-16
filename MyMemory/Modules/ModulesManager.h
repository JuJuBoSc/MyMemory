#pragma once

using namespace System;

namespace MyMemory {
	namespace Modules {

		public ref class ModulesManager
		{

			// Members
		private:
			RemoteProcess^ m_remoteProcess;

			// Methods
		public:
			ModulesManager(RemoteProcess^ remoteProcess);
			MyMemory::Modules::RemoteModule^ GetModule(String^ name);


			// Properties
		public:
			property RemoteProcess^ Process {
				RemoteProcess^ get() { return m_remoteProcess; }
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
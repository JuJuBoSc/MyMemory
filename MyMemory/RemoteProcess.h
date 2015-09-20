#pragma once

using namespace System;
using namespace System::Text;
using namespace System::Collections::Generic;

namespace MyMemory {
	namespace Assembly {
		ref class Yasm;
	}
	namespace Modules {
		ref class ModulesManager;
		ref class RemoteModule;
	}
	namespace Threads {
		ref class ThreadsManager;
		ref class RemoteThread;
	}
	namespace Memory {
		ref class MemoryManager;;
		ref class RemoteMemoryProtection;
		ref class RemoteAllocatedMemory;
	}
	namespace Hooks {
		ref class HooksManager;
	}
	namespace Patterns {
		ref class PatternsManager;
	}
}

namespace MyMemory {
	
	public ref class RemoteProcess
	{

	private:
		unsigned int m_processId;
		void* m_processHandle;
		MyMemory::Assembly::Yasm^ m_yasm;
		MyMemory::Memory::MemoryManager^ m_memoryManager;
		MyMemory::Modules::ModulesManager^ m_modulesManager;
		MyMemory::Threads::ThreadsManager^ m_threadsManager;
		MyMemory::Hooks::HooksManager^ m_hooksManager;
		MyMemory::Patterns::PatternsManager^ m_patternsManager;
		
		// Methods
	public:
		RemoteProcess(unsigned int processId);
		~RemoteProcess();

		// Properties
	public:
		property MyMemory::Assembly::Yasm^ Yasm {
			MyMemory::Assembly::Yasm^ get() { return m_yasm; }
		}
		property MyMemory::Memory::MemoryManager^ MemoryManager {
			MyMemory::Memory::MemoryManager^ get() { return m_memoryManager; }
		}
		property MyMemory::Modules::ModulesManager^ ModulesManager {
			MyMemory::Modules::ModulesManager^ get() { return m_modulesManager; }
		}
		property MyMemory::Threads::ThreadsManager^ ThreadsManager {
			MyMemory::Threads::ThreadsManager^ get() { return m_threadsManager; }
		}
		property MyMemory::Hooks::HooksManager^ HooksManager {
			MyMemory::Hooks::HooksManager^ get() { return m_hooksManager; }
		}
		property MyMemory::Patterns::PatternsManager^ PatternsManager {
			MyMemory::Patterns::PatternsManager^ get() { return m_patternsManager; }
		}
		property unsigned int ProcessId {
			unsigned int get() { return m_processId; }
		}
		property IntPtr ProcessHandle {
			IntPtr get() { return (IntPtr)m_processHandle; }
		}

	};
}

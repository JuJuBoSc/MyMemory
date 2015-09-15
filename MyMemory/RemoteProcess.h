#pragma once

using namespace System;
using namespace System::Text;
using namespace System::Collections::Generic;

namespace MyMemory {
	namespace Assembly {
		ref class Yasm;
	}
	namespace Modules {
		ref class RemoteModule;
	}
}

namespace MyMemory {
	
	public ref class RemoteProcess
	{

	private:
		unsigned int m_processId;
		void* m_processHandle;
		bool m_is64BitsProcess;
		MyMemory::Assembly::Yasm^ m_yasm;
		
		// Methods
	public:
		RemoteProcess();
		~RemoteProcess();
		bool Open(unsigned int processId);
		void Close();
		generic <typename T> T Read(IntPtr lpAddress);
		array<byte>^ ReadBytes(IntPtr lpAddress, int count);
		String^ ReadString(IntPtr lpAddress, Encoding^ encoding, int maxLength);
		String^ ReadString(IntPtr lpAddress, Encoding^ encoding) { return ReadString(lpAddress, encoding, 128); }
		String^ ReadString(IntPtr lpAddress) { return ReadString(lpAddress, Encoding::UTF8); }
		generic <typename T> bool Write(IntPtr lpAddress, T value);
		bool WriteBytes(IntPtr lpAddress, array<byte>^ bBuffer);
		bool WriteString(IntPtr lpAddress, Encoding^ encoding, String^ value);
		bool WriteString(IntPtr lpAddress, String^ value) { return WriteString(lpAddress, Encoding::UTF8, value); }
		MyMemory::Modules::RemoteModule^ GetModule(String^ name);


		// Properties
	public:
		property MyMemory::Assembly::Yasm^ Yasm {
			MyMemory::Assembly::Yasm^ get() { return m_yasm; }
		}
		property unsigned int ProcessId {
			unsigned int get() { return m_processId; }
		}
		property IntPtr ProcessHandle {
			IntPtr get() { return (IntPtr)m_processHandle; }
		}
		property bool Is64BitsProcess {
			bool get() { return m_is64BitsProcess; }
		}
		property List<MyMemory::Modules::RemoteModule^>^ Modules {
			List<MyMemory::Modules::RemoteModule^>^ get();
		}

	};
}

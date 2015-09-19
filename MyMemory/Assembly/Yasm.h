#pragma once

extern "C" int __cdecl _yasm_Assemble(const char *, void *, int);

using namespace System;
using namespace System::Threading;

namespace MyMemory {
	ref class RemoteProcess;
	namespace Assembly {


		public ref class Yasm
		{

			// Members
		private:
			RemoteProcess^ m_remoteProcess;
			int m_bufferSize;
			IntPtr m_pBuffer;
			int m_bits;

			// Methods
		public:
			Yasm(RemoteProcess^ process, int bufferSize, int bits);
			~Yasm();
			void SetBufferSize(int size);
			array<byte>^ Assemble(array<String^>^ mnemonics, IntPtr org);
			array<byte>^ Assemble(array<String^>^ mnemonics) { return Assemble(mnemonics, IntPtr(0)); }
			bool Inject(array<String^>^ mnemonics, IntPtr lpAddress);
			IntPtr InjectAndExecute(array<String^>^ mnemonics, IntPtr lpAddress);
			IntPtr InjectAndExecute(array<String^>^ mnemonics);

			// Properties
		public:
			property int Bits {
				int get() { return m_bits; }
			}
			property RemoteProcess^ Process {
				RemoteProcess^ get() { return m_remoteProcess; }
			}

		};

	}
}
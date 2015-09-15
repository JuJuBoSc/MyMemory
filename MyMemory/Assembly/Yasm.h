#pragma once

using namespace System;
using namespace System::Threading;

namespace MyMemory {
	ref class RemoteProcess;
	namespace Assembly {


		public ref class Yasm
		{

		private:
			RemoteProcess^ m_remoteProcess;
			int m_bufferSize;
			IntPtr m_pBuffer;
			int m_bits;

		public:
			Yasm(RemoteProcess^ process, int bufferSize, int bits);
			~Yasm();
			void SetBufferSize(int size);
			array<byte>^ Assemble(array<String^>^ mnemonics, IntPtr org);
			array<byte>^ Assemble(array<String^>^ mnemonics) { return Assemble(mnemonics, IntPtr(0)); }
			property int Bits {
				int get() { return m_bits; }
			}
			property RemoteProcess^ Process {
				RemoteProcess^ get() { return m_remoteProcess; }
			}

		};

	}
}
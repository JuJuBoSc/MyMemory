#pragma once

using namespace System;

namespace MyMemory {
	namespace Patterns {

		public ref class PatternsManager
		{

			// Statics
		private:
			static int FintPatternInBuffer(array<byte>^ bBuffer, String^ pattern);

			// Members
		private:
			RemoteProcess^ m_remoteProcess;

			// Methods
		public:
			PatternsManager(RemoteProcess^ remoteProcess);
			IntPtr FindPattern(IntPtr lpStartAddress, unsigned int lenth, String^ pattern, String^ operationTokens);
			IntPtr FindPattern(String^ pattern, String^ operationTokens);
			IntPtr FindPattern(IntPtr lpStartAddress, unsigned int lenth, String^ pattern);
			IntPtr FindPattern(String^ pattern);

			// Methods
		private:
			bool ProcessTokens(IntPtr% pattern, String^% operationTokens);

			// Properties
		public:
			property RemoteProcess^ Process {
				RemoteProcess^ get() { return m_remoteProcess; }
			}


		};

	}
}
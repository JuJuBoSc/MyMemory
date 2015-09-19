#pragma once

using namespace System;

namespace MyMemory {
	namespace Hooks {

		public ref class HookJmp : public HookBase
		{

			// Members
		private:
			bool m_isApplied;
			IntPtr m_address;
			unsigned long m_length;
			array<byte>^ m_originalBytes;
			array<byte>^ m_jmpBytes;
			MyMemory::Memory::RemoteAllocatedMemory^ m_hookJmpAllocatedMemory;
			IntPtr m_trampolinePointer;
			IntPtr m_detourPointer;

			// Methods
		public:
			HookJmp(RemoteProcess^ remoteProcess, IntPtr lpAddress, unsigned long length);
			~HookJmp();

			// Methods
		private:
			void GenerateTrampoline();
			void GenerateDetour();
			void GenerateJmp();

			// Properties
		public:

			// Override methods
		public:
			virtual void Apply() override;
			virtual void Remove() override;

			// Abstract properties
		public:
			property bool IsApplied {
				virtual bool get() override { return m_isApplied; }
			}

		};

	}
}
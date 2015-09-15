#pragma once

using namespace System;

namespace MyMemory {
	namespace Memory {

		public ref class RemotePointer
		{

		// Members
		private:
			RemoteProcess^ m_remoteProcess;
			IntPtr m_pointer;
			void* m_nativePointer;

		// Methods
		public:
			RemotePointer(RemoteProcess^ remoteProcess, IntPtr pointer);
			generic <typename T> T Read(int offset);
			generic <typename T> T Read() { return Read<T>(0); }
			array<byte>^ ReadBytes(int offset, int count);
			array<byte>^ ReadBytes(int count) { return ReadBytes(0, count); }
			String^ ReadString(int offset, Encoding^ encoding, int maxLength);
			String^ ReadString(int offset, Encoding^ encoding) { return ReadString(offset, encoding, 128); }
			String^ ReadString(int offset) { return ReadString(offset, Encoding::UTF8); }
			generic <typename T> bool Write(int offset, T value);
			generic <typename T> bool Write(T value) { return Write<T>(0, value); }
			bool WriteBytes(int offset, array<byte>^ bBuffer);
			bool WriteBytes(array<byte>^ bBuffer) { return WriteBytes(0, bBuffer); }
			bool WriteString(int offset, Encoding^ encoding, String^ value);
			bool WriteString(int offset, String^ value) { return WriteString(offset, Encoding::UTF8, value); }
			bool WriteString(String^ value) { return WriteString(0, value); }


		// Properties
		public:
			property RemoteProcess^ Process {
				RemoteProcess^ get() { return m_remoteProcess; }
			}
			property IntPtr Pointer {
				IntPtr get() { return m_pointer; }
				void set(IntPtr value) { 
					m_pointer = value;
					m_nativePointer = value.ToPointer();
				}
			}
			property bool IsValid {
				bool get() { return m_nativePointer != nullptr; }
			}


		};

	}
}
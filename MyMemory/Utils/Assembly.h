#pragma once

using namespace System;
using namespace System::Linq;
using namespace System::Runtime::InteropServices;
using namespace System::Reflection;
using namespace System::Reflection::Emit;

namespace MyMemory {
	namespace Utils {

			public ref class Assembly
			{

			internal:
				static array<String^>^ Generate_SaveRegisters(IntPtr pBaseRegisters);
				static array<String^>^ Generate_PushRegistersAndFlags();
				static array<String^>^ Generate_PopRegistersAndFlags();
				static array<String^>^ Generate_RtlEnterCriticalSection(IntPtr lpCriticalSection);
				static array<String^>^ Generate_RtlLeaveCriticalSection(IntPtr lpCriticalSection);
				static array<String^>^ Generate_WaitForSingleObject(IntPtr hHandle, unsigned long dwMilliseconds);
				static array<String^>^ Generate_ReleaseMutex(IntPtr hMutex);
				static array<String^>^ Generate_SetEvent(IntPtr hEvent);
				static array<String^>^ Generate_GetCurrentThreadId();
				static bool Remote_RtlInitializeCriticalSection(MyMemory::RemoteProcess^ process, IntPtr lpCriticalSection);
				static IntPtr Remote_CreateEvent(MyMemory::RemoteProcess^ process, String^ eventName, bool manualReset, bool initialState);
				static IntPtr Remote_CreateMutex(MyMemory::RemoteProcess^ process, String^ mutexName, bool initialOwner);
			};

	}
}
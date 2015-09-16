#pragma once

using namespace System;
using namespace System::Runtime::InteropServices;

namespace MyMemory {

	public ref class Enumerations
	{

	public:
		[Flags]
		enum class MemoryProtectionFlags : unsigned int
		{
			Execute = 0x10,
			ExecuteRead = 0x20,
			ExecuteReadWrite = 0x40,
			ExecuteWriteCopy = 0x80,
			NoAccess = 0x01,
			ReadOnly = 0x02,
			ReadWrite = 0x04,
			WriteCopy = 0x08,
			GuardModifierflag = 0x100,
			NoCacheModifierflag = 0x200,
			WriteCombineModifierflag = 0x400
		};

	};
}

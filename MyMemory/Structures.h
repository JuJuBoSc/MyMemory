#pragma once

using namespace System;
using namespace System::Runtime::InteropServices;

namespace MyMemory {

	public ref class Structures
	{
		
#if _WIN64 || __amd64__
	public:
		[StructLayout(LayoutKind::Sequential)]
		value struct BaseRegisters
		{
		public:
			IntPtr R15;
			IntPtr R14;
			IntPtr R13;
			IntPtr R12;
			IntPtr R11;
			IntPtr R10;
			IntPtr R9;
			IntPtr R8;
			IntPtr Rdi;
			IntPtr Rsi;
			IntPtr Rdx;
			IntPtr Rcx;
			IntPtr Rbx;
			IntPtr Rax;
			IntPtr Rbp;
			IntPtr Rsp;
		};
		[StructLayout(LayoutKind::Sequential)]
		value struct ThreadContext
		{
		public:
			unsigned long long P1Home;
			unsigned long long P2Home;
			unsigned long long P3Home;
			unsigned long long P4Home;
			unsigned long long P5Home;
			unsigned long long P6Home;
			unsigned long ContextFlags;
			unsigned long MxCsr;
			unsigned short SegCs;
			unsigned short SegDs;
			unsigned short SegEs;
			unsigned short SegFs;
			unsigned short SegGs;
			unsigned short SegSs;
			unsigned long EFlags;
			unsigned long long Dr0;
			unsigned long long Dr1;
			unsigned long long Dr2;
			unsigned long long Dr3;
			unsigned long long Dr6;
			unsigned long long Dr7;
			unsigned long long Rax;
			unsigned long long Rcx;
			unsigned long long Rdx;
			unsigned long long Rbx;
			unsigned long long Rsp;
			unsigned long long Rbp;
			unsigned long long Rsi;
			unsigned long long Rdi;
			unsigned long long R8;
			unsigned long long R9;
			unsigned long long R10;
			unsigned long long R11;
			unsigned long long R12;
			unsigned long long R13;
			unsigned long long R14;
			unsigned long long R15;
			unsigned long long Rip;
			[MarshalAs(UnmanagedType::ByValArray, SizeConst = 512)]
			array<unsigned char>^  FltSave;
			[MarshalAs(UnmanagedType::ByValArray, SizeConst = 416)]
			array<unsigned char>^  VectorRegister;
			unsigned long long VectorControl;
			unsigned long long DebugControl;
			unsigned long long LastBranchToRip;
			unsigned long long LastBranchFromRip;
			unsigned long long LastExceptionToRip;
			unsigned long long LastExceptionFromRip;
		};
#else
	public:
		[StructLayout(LayoutKind::Sequential)]
		value struct BaseRegisters
		{
		public:
			IntPtr Eax;
			IntPtr Ebx;
			IntPtr Ecx;
			IntPtr Edx;
			IntPtr Edi;
			IntPtr Esi;
			IntPtr Ebp;
			IntPtr Esp;
		};
		[StructLayout(LayoutKind::Sequential)]
		value struct ThreadContext
		{
		public:
			unsigned long ContextFlags;
			unsigned long Dr0;
			unsigned long Dr1;
			unsigned long Dr2;
			unsigned long Dr3;
			unsigned long Dr6;
			unsigned long Dr7;
			[MarshalAs(UnmanagedType::ByValArray, SizeConst = 112)]
			array<unsigned char>^  FltSave;
			unsigned long SegGs;
			unsigned long SegFs;
			unsigned long SegEs;
			unsigned long SegDs;
			unsigned long Edi;
			unsigned long Esi;
			unsigned long Ebx;
			unsigned long Edx;
			unsigned long Ecx;
			unsigned long Eax;
			unsigned long Ebp;
			unsigned long Eip;
			unsigned long SegCs;
			unsigned long EFlags;
			unsigned long Esp;
			unsigned long SegSs;
			[MarshalAs(UnmanagedType::ByValArray, SizeConst = 512)]
			array<unsigned char>^  ExtendedRegisters;
	};
#endif

	};
}

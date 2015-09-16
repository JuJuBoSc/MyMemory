#pragma once

#define InitializeObjectAttributes( i, o, a, r, s ) {    \
      (i)->Length = sizeof( OBJECT_ATTRIBUTES );         \
      (i)->RootDirectory = r;                            \
      (i)->Attributes = a;                               \
      (i)->ObjectName = o;                               \
      (i)->SecurityDescriptor = s;                       \
      (i)->SecurityQualityOfService = NULL;              \
   }

#define NT_SUCCESS(Status) (((NTSTATUS)(Status)) >= 0) 

typedef struct _CLIENT_ID
{
	PVOID UniqueProcess;
	PVOID UniqueThread;
} CLIENT_ID, *PCLIENT_ID;

typedef struct _UNICODE_STRING {
	USHORT Length;
	USHORT MaximumLength;
	PWSTR  Buffer;
} UNICODE_STRING, *PUNICODE_STRING;

typedef struct _OBJECT_ATTRIBUTES {
	ULONG           Length;
	HANDLE          RootDirectory;
	PUNICODE_STRING ObjectName;
	ULONG           Attributes;
	PVOID           SecurityDescriptor;
	PVOID           SecurityQualityOfService;
}  OBJECT_ATTRIBUTES, *POBJECT_ATTRIBUTES;

// ntdll
EXTERN_C NTSTATUS NTAPI NtOpenProcess(PHANDLE, ACCESS_MASK, POBJECT_ATTRIBUTES, PCLIENT_ID);
EXTERN_C NTSTATUS NTAPI NtOpenThread(PHANDLE, ACCESS_MASK, POBJECT_ATTRIBUTES, PCLIENT_ID);
EXTERN_C NTSTATUS NTAPI NtClose(HANDLE);
EXTERN_C NTSTATUS NTAPI NtReadVirtualMemory(HANDLE, PVOID, PVOID, ULONG, ULONG);
EXTERN_C NTSTATUS NTAPI NtWriteVirtualMemory(HANDLE, PVOID, PVOID, ULONG, ULONG);
EXTERN_C NTSTATUS NTAPI NtSuspendThread(HANDLE, PULONG);
EXTERN_C NTSTATUS NTAPI NtResumeThread(HANDLE, PULONG);
EXTERN_C NTSTATUS NTAPI NtProtectVirtualMemory(HANDLE, PVOID, PULONG, ULONG, PULONG);
EXTERN_C NTSTATUS NTAPI NtGetContextThread(HANDLE, PCONTEXT);
EXTERN_C NTSTATUS NTAPI NtSetContextThread(HANDLE, PCONTEXT);
EXTERN_C NTSTATUS NTAPI NtAllocateVirtualMemory(HANDLE, PVOID, ULONG, PULONG, ULONG, ULONG);
EXTERN_C NTSTATUS NTAPI NtFreeVirtualMemory(HANDLE, PVOID, PULONG, ULONG);
EXTERN_C NTSTATUS NTAPI NtWaitForSingleObject(HANDLE, BOOLEAN, PLARGE_INTEGER);

// yasm
extern "C" int __cdecl _yasm_Assemble(const char *, void *, int);
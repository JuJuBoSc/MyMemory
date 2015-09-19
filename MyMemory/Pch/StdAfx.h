#pragma once

#include <vcclr.h>

#include <stdio.h>
#include <Windows.h>
#include <psapi.h>
#include <TlHelp32.h>

#include <Structures.h>
#include <Enumerations.h>
#include <RemoteProcess.h>
#include <Utils/MarshalCache.h>
#include <Utils/Assembly.h>
#include <Utils/Randoms.h>
#include <Assembly/Yasm.h>
#include <Memory/MemoryManager.h>
#include <Memory/RemotePointer.h>
#include <Memory/RemoteRegion.h>
#include <Memory/RemoteMemoryProtection.h>
#include <Memory/RemoteAllocatedMemory.h>
#include <Modules/ModulesManager.h>
#include <Modules/RemoteModule.h>
#include <Threads/ThreadsManager.h>
#include <Threads/RemoteThread.h>
#include <Hooks/HooksManager.h>
#include <Hooks/HookBase.h>
#include <Hooks/CallbackBase.h>
#include <Hooks/CallbackExecutor.h>
#include <Hooks/CallbackNotifier.h>
#include <Hooks/HookJmp.h>
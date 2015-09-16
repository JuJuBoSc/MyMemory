# MyMemory
.NET remote process manipulation library for x86 / x64 written in C++/CLI.


> **Note :** 
> Your program architecture must match the remote process architecture.
> If you want to manipulate x64 process, compile MyMemory as x64 !
> 
> This also apply to WOW64 process, x86 process on x64 OS, MyMemory need to be compiled as x86.


# Credits
- Mercury because that's mainly because of you I fall in this world.
- Apoc for it's MarshalCache class and many clean win32 implementation.
- DrakeFish as you did the first version of VEH in assembly for me.
- Cypher for being awesome and helped me with many undocumented stuff (and because he like yelling at everyone).
- Shynd because he is a wizard doing black magic.
- Master674 you helped me in many differents area, unrelated to this project, but had to put your name.
- ZenLulz for good implementation in it's MemorySharp project that gave me many ideas.
- Nesox's cat.
- Cromon because I had some fun time reading you on Skype group while coding.

#Simple documentation :

 - 1 - **Basic memory maniputation**
  - 1.1 - Initialize a RemoteProcess
  - 1.2 - Reading memory
  - 1.3 - Writing memory
  - 1.4 - Protect memory
  - 1.5 - Allocate memory
 - 2 - **Modules informations**
  - 2.1 - Enumerate modules 
 - 3 - **Threads informations**
  - 3.1 - Enumerate threads
  - 3.2 - Manipulate thread context
 - 4 - **Assembly**
  - 4.1 - Use Yasm
  - 4.2 - Assemble mnemonics


# 1 - Basic memory manipulation

##1.1 Initialize a RemoteProcess

Simply make a new instance of **MyMemory.RemoteProcess** and call **RemoteProcess.Open(uint processId)** :

```csharp
var process = new MyMemory.RemoteProcess();
bool processOpened = process.Open(1234); // adjust process id
```

## 1.2 Reading memory

```csharp
var result = process.MemoryManager.Read<int>(new IntPtr(0xDEADBEEF));
```

## 1.3 Writing memory

```csharp
var writeSuccess = process.MemoryManager.Write<int>(new IntPtr(0xDEADBEEF), 1234);
```

## 1.4 Protect memory

Change the memory protection of a specific region, the RemoteMemoryProtection class implement IDisposable so it can be used in an using block :

```csharp
using (var protection = process.MemoryManager.ProtectMemory(new IntPtr(0x9A0000), 0x1000, Enumerations.MemoryProtectionFlags.ExecuteReadWrite))
{
    // Protect the memory at address 0x9A0000 with a length 0x1000
    // ProtectMemory return an RemoteMemoryProtection instance
    // The memory protection is restored when .Dispose() is called
}
```

## 1.5 Allocate memory

Allocate memory in the process, the RemoteAllocatedMemory class implement IDisposable so it can be used in an using block :

```csharp
using (var allocatedMemory = process.MemoryManager.AllocateMemory(1000))
{
    Console.WriteLine("BaseAddress : 0x{0}", allocatedMemory.Pointer.ToInt64());
    // You can also get chunk from this allocated memory
    var chunk1 = allocatedMemory.AllocateOfChunk("chunkName1", 4);
    var chunk2 = allocatedMemory.AllocateOfChunk("chunkName2", 4);
    // And write in them
    allocatedMemory.Write<int>("chunkName1", 123);
    // Or read them
    int value = allocatedMemory.Read<int>("chunkName1");
}
```

# 2 - Modules informations

## 2.1 Enumerate modules

```csharp
foreach (var remoteModule in process.ModulesManager.Modules)
{
    Console.WriteLine("Name : {0}", remoteModule.BaseName);
    Console.WriteLine("BaseAddress : 0x{0:X}", remoteModule.BaseAddress.ToInt64());
    Console.WriteLine("Size : 0x{0:X}", remoteModule.Size);
}
```

# 3 - Threads informations

## 3.1 Enumerate  threads

```csharp
foreach (var remoteThread in process.ThreadsManager.Threads)
{
    Console.WriteLine("ThreadId : {0}", remoteThread.ThreadId);
}
```

## 3.2 Manipulate thread context

```csharp
var mainThread = process.ThreadsManager.MainThread;
if (mainThread.SuspendThread())
{
    MyMemory.Structures.ThreadContext ctx;
    if (mainThread.GetThreadContext(out ctx))
    {
        ctx.Eip = 0; // Don't do that for real, just proof of concept !
        mainThread.SetThreadContext(ref ctx);
    }
    mainThread.ResumeThread();
}
```

# 4 - Assembly

## 4.1 Use Yasm

> MyMemory embedd Yasm assembler to assemble mnemonics on the fly.
> It support 32 and 64 bits.

First, you need to create a new instance of Yasm :


```csharp
var yasm32 = new MyMemory.Assembly.Yasm(process, 1024, 32); 
// Create a new Yasm instance with a 1024 bytes buffer, for 32 bits

var yasm64 = new MyMemory.Assembly.Yasm(process, 1024, 64); 
// Create a new Yasm instance with a 1024 bytes buffer, for 64 bits
```

You can also use the already instanced Yasm property in RemoteProcess class :

```csharp
var yasm = process.Yasm;
```

> **Note : the RemoteProcess.Yasm by default match the current process architecture.**

## 4.2 Assemble mnemonics

That's pretty simple, the Yasm.Assemble function take the mnemonics as a string array, and return the result as byte array :

```csharp
string[] mnemonics = { "mov rax, 1", "retn" };
byte[] opcodes = process.Yasm.Assemble(mnemonics);

// Output : 48 C7 C0 01 00 00 00 C3
```
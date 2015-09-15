# MyMemory
.NET remote process manipulation library for x86 / x64 written in C++/CLI.


> **Note :** 
> Your program architecture must match the remote process architecture.
> If you want to manipulate x64 process, compile MyMemory as x64 !
> 
> This also apply to WOW64 process, x86 process on x64 OS, MyMemory need to be compiled as x86.

#Simple documentation :

 1. **Basic memory maniputation**
 1.1 Initialize a RemoteProcess
 1.2 Reading memory
 1.3 Writing memory
 2. **Modules informations**
 2.1 Enumerate modules 
 3. **Threads informations**
 3.1 Enumerate threads
 3.2 Manipulate thread context
 4. **Assembly**
 4.1 Use Yasm
 4.2 Assemble mnemonics


# 1 - Basic memory manipulation

##1.1 Initialize a RemoteProcess

Simply make a new instance of **MyMemory.RemoteProcess** and call **RemoteProcess.Open(uint processId)** :

    var process = new MyMemory.RemoteProcess();
    bool processOpened = process.Open(1234); // adjust process id

## 1.2 Reading memory

    var result = process.Read<int>(new IntPtr(0xDEADBEEF));

## 1.3 Writing memory

    var writeSuccess = process.Write<int>(new IntPtr(0xDEADBEEF), 1234);

# 2 - Modules informations

## 2.1 Enumerate modules
    foreach (var remoteModule in process.Modules)
    {
	    Console.WriteLine("Name : {0}", remoteModule.BaseName);
        Console.WriteLine("BaseAddress : 0x{0:X}", remoteModule.BaseAddress.ToInt64());
        Console.WriteLine("Size : 0x{0:X}", remoteModule.Size);
    }

# 3 - Threads informations

## 3.1 Enumerate  threads
    foreach (var remoteThread in process.Threads)
    {
	    Console.WriteLine("ThreadId : {0}", remoteThread.ThreadId);
    }

## 3.2 Manipulate thread context
    if (remoteThread.SuspendThread())
    {
        MyMemory.Structures.ThreadContext ctx;
        if (remoteThread.GetThreadContext(out ctx))
        {
            ctx.Rip = 0; // Don't do that for real, just proof of concept !
            remoteThread.SetThreadContext(ref ctx);
        }
        remoteThread.ResumeThread();
    }

# 4 - Assembly

## 4.1 Use Yasm

> MyMemory embedd Yasm assembler to assemble mnemonics on the fly.
> It support 32 and 64 bits.

First, you need to create a new instance of Yasm :

    var yasm32 = new MyMemory.Assembly.Yasm(process, 1024, 32); 
    // Create a new Yasm instance with a 1024 bytes buffer, for 32 bits
          
    var yasm64 = new MyMemory.Assembly.Yasm(process, 1024, 64); 
    // Create a new Yasm instance with a 1024 bytes buffer, for 64 bits

You can also use the already instanced Yasm property in RemoteProcess class :

    var yasm = process.Yasm;

> **Note : the RemoteProcess.Yasm by default match the current process architecture.**

## 4.2 Assemble mnemonics

That's pretty simple, the Yasm.Assemble function take the mnemonics as a string array, and return the result as byte array :

    string[] mnemonics = { "mov rax, 1", "retn" };
    byte[] opcodes = process.Yasm.Assemble(mnemonics);

    // Output : 48 C7 C0 01 00 00 00 C3


# Credits
- Apoc for it's MarshalCache class and many clean win32 implementation.
- Cypher for being awesome and helped me with many undocumented stuff.
- ZenLulz for good implementation in it's MemorySharp project that gave me many ideas.
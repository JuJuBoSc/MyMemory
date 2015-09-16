#include <Pch/StdAfx.h>


MyMemory::Assembly::Yasm::Yasm(RemoteProcess^ process, int bufferSize, int bits)
{
	if (bits != 32 && bits != 64) throw gcnew ArgumentOutOfRangeException("bits", "Yasm support only 32 or 64 bits !");
	m_bits = bits;
	m_remoteProcess = process;
	SetBufferSize(bufferSize);
}

MyMemory::Assembly::Yasm::~Yasm()
{
	if (m_pBuffer.ToPointer() != nullptr)
	{
		Marshal::FreeHGlobal(m_pBuffer);
		m_pBuffer = IntPtr(nullptr);
	}
}

void MyMemory::Assembly::Yasm::SetBufferSize(int size)
{
	if (m_pBuffer.ToPointer() != nullptr) Marshal::FreeHGlobal(m_pBuffer);
	m_pBuffer = Marshal::AllocHGlobal(size);
	m_bufferSize = size;
}

array<byte>^ MyMemory::Assembly::Yasm::Assemble(array<String^>^ mnemonics, IntPtr org)
{
	Monitor::Enter(this);
	try
	{

		StringBuilder sb;
		sb.AppendLine("[BITS " + m_bits + "]");

		if (org.ToPointer() != nullptr) sb.AppendLine("[ORG 0x" + org.ToInt64().ToString("X") + "]");
		for each (String^ s in mnemonics) sb.AppendLine(s);

		IntPtr pBuffer = Marshal::StringToHGlobalAnsi(sb.ToString());
		int len = _yasm_Assemble((const char*)pBuffer.ToPointer(), m_pBuffer.ToPointer(), m_bufferSize);

		if (len == -1)
			throw gcnew Exception("Yasm error : Invalid mnemonics !");

		if (len == -2)
			throw gcnew OutOfMemoryException("Yasm error : The buffer was too small !");

		if (len == -3)
			throw gcnew Exception("Yasm error : Internal yasm error !");

		array<byte>^ result = gcnew array<byte>(len);
		pin_ptr<Byte> pResult = &result[0];
		memcpy(pResult, m_pBuffer.ToPointer(), len);
		Marshal::FreeHGlobal(pBuffer);

		return result;

	}
	finally
	{
		Monitor::Exit(this);
	}
}

bool MyMemory::Assembly::Yasm::Inject(array<String^>^ mnemonics, IntPtr lpAddress)
{
	return m_remoteProcess->MemoryManager->WriteBytes(lpAddress, Assemble(mnemonics, lpAddress));
}

System::IntPtr MyMemory::Assembly::Yasm::InjectAndExecute(array<String^>^ mnemonics, IntPtr lpAddress)
{
	Memory::RemoteAllocatedMemory^ allocatedMemory = m_remoteProcess->MemoryManager->AllocateMemory(500);
	try
	{

		IntPtr pResult = allocatedMemory->AllocateOfChunk("Result", Utils::MarshalCache<IntPtr>::Size);
		IntPtr pCallGate = allocatedMemory->AllocateOfChunk("CallGate", 256);
		Inject(mnemonics, lpAddress);

#if _WIN64 || __amd64__
		array<String^>^ asmCallGate = gcnew array<String^>
		{	
				"call " + lpAddress,
				"mov [" + pResult + "], rax",
				"retn"
		};
#else
		array<String^>^ asmCallGate = gcnew array<String^>
		{
				"call " + lpAddress,
				"mov [" + pResult + "], eax",
				"retn"
		};

#endif

		Inject(asmCallGate, pCallGate);

		Threads::RemoteThread^ remoteThread = m_remoteProcess->ThreadsManager->CreateRemoteThread(pCallGate);
		remoteThread->Join();

		return m_remoteProcess->MemoryManager->Read<IntPtr>(pResult);

	}
	finally
	{
		delete allocatedMemory;
	}
}

System::IntPtr MyMemory::Assembly::Yasm::InjectAndExecute(array<String^>^ mnemonics)
{
	MyMemory::Memory::RemoteAllocatedMemory^ allocatedMemory = m_remoteProcess->MemoryManager->AllocateMemory(m_bufferSize);
	try
	{
		return InjectAndExecute(mnemonics, allocatedMemory->Pointer);
	}
	finally
	{
		delete allocatedMemory;
	}
}

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

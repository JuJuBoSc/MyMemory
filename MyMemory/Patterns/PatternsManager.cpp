#include <Pch/StdAfx.h>

MyMemory::Patterns::PatternsManager::PatternsManager(RemoteProcess^ remoteProcess)
{
	m_remoteProcess = remoteProcess;
}

System::IntPtr MyMemory::Patterns::PatternsManager::FindPattern(IntPtr lpStartAddress, unsigned int lenth, String^ pattern, String^ operationTokens)
{

	array<byte>^ bBuffer = Process->MemoryManager->ReadBytes(lpStartAddress, lenth);

	int patternOffset = FintPatternInBuffer(bBuffer, pattern);

	if (patternOffset == -1)
		return IntPtr(0);

	IntPtr patternAbsolute = lpStartAddress + patternOffset;

	while (operationTokens->Length > 0 && ProcessTokens(patternAbsolute, operationTokens)) { }

	return patternAbsolute;

}

System::IntPtr MyMemory::Patterns::PatternsManager::FindPattern(String^ pattern)
{
	return FindPattern(Process->ModulesManager->MainModule->BaseAddress, Process->ModulesManager->MainModule->Size, pattern);
}

System::IntPtr MyMemory::Patterns::PatternsManager::FindPattern(String^ pattern, String^ operationTokens)
{
	return FindPattern(Process->ModulesManager->MainModule->BaseAddress, Process->ModulesManager->MainModule->Size, pattern, operationTokens);
}

bool MyMemory::Patterns::PatternsManager::ProcessTokens(IntPtr% pattern, String^% operationTokens)
{

	if (operationTokens->ToLower()->StartsWith("read8"))
	{
		pattern = (IntPtr)Process->MemoryManager->Read<byte>(pattern);
		operationTokens = operationTokens->Remove(0, 5)->Trim();
		return true;
	}

	if (operationTokens->ToLower()->StartsWith("read16"))
	{
		pattern = (IntPtr)Process->MemoryManager->Read<short>(pattern);
		operationTokens = operationTokens->Remove(0, 6)->Trim();
		return true;
	}

	if (operationTokens->ToLower()->StartsWith("read32"))
	{
		pattern = (IntPtr)Process->MemoryManager->Read<int>(pattern);
		operationTokens = operationTokens->Remove(0, 6)->Trim();
		return true;
	}

	if (operationTokens->ToLower()->StartsWith("read64"))
	{
		pattern = (IntPtr)Process->MemoryManager->Read<long long>(pattern);
		operationTokens = operationTokens->Remove(0, 6)->Trim();
		return true;
	}

	if (operationTokens->ToLower()->StartsWith("tracecall"))
	{
		switch (Process->MemoryManager->Read<byte>(pattern))
		{
		case 0xE8:
			pattern = pattern + 5 + Process->MemoryManager->Read<int>(pattern + 1);
			break;
		default:
			break;
		}
		operationTokens = operationTokens->Remove(0, 9)->Trim();
		return true;
	}

	if (operationTokens->ToLower()->StartsWith("tracerelative"))
	{
#if _WIN64 || __amd64__
		pattern = pattern + 7 + Process->MemoryManager->Read<int>(pattern + 3);
#else
		pattern = Process->MemoryManager->Read<IntPtr>(pattern + 1);
#endif
		operationTokens = operationTokens->Remove(0, 13)->Trim();
		return true;
	}

	if (operationTokens->ToLower()->StartsWith("add"))
	{
		array<System::String^>^ splitSpaces = operationTokens->Split(' ');
		int offset = 0;
		if (splitSpaces->Length > 0 && int::TryParse(splitSpaces[1], offset))
		{
			pattern = pattern + offset;
			operationTokens = operationTokens->Remove(0, 4 + splitSpaces[1]->Length)->Trim();
			return true;
		}
		return false;
	}

	if (operationTokens->ToLower()->StartsWith("sub"))
	{
		array<System::String^>^ splitSpaces = operationTokens->Split(' ');
		int offset = 0;
		if (splitSpaces->Length > 0 && int::TryParse(splitSpaces[1], offset))
		{
			pattern = pattern - offset;
			operationTokens = operationTokens->Remove(0, 4 + splitSpaces[1]->Length)->Trim();
			return true;
		}
		return false;
	}

	return false;

}

System::IntPtr MyMemory::Patterns::PatternsManager::FindPattern(IntPtr lpStartAddress, unsigned int lenth, String^ pattern)
{

	array<String^>^ tokensPrefix = gcnew array<String^>
	{
		"Read8",
			"Read16",
			"Read32",
			"Read64",
			"Add",
			"Sub",
			"TraceCall",
			"TraceRelative",
	};

	int nearestPrefix = -1;
	for each (String^ prefix in tokensPrefix)
	{
		int prefixIndex = pattern->IndexOf(prefix);
		if (prefixIndex >= 0 && (nearestPrefix == -1 || prefixIndex < nearestPrefix))
		{
			nearestPrefix = prefixIndex;
		}
	}

	if (nearestPrefix >= 0)
		return FindPattern(lpStartAddress, lenth, pattern->Substring(0, nearestPrefix), pattern->Substring(nearestPrefix, pattern->Length - nearestPrefix));

	return FindPattern(lpStartAddress, lenth, pattern, String::Empty);

}

int MyMemory::Patterns::PatternsManager::FintPatternInBuffer(array<byte>^ bBuffer, String^ pattern)
{

	List<byte> bPattern;
	List<bool> Mask;

	for each (String^ s in pattern->Split(' '))
	{
		if (String::IsNullOrEmpty(s)) continue;
		if (s == "?" || s == "??")
		{
			bPattern.Add(0);
			Mask.Add(false);
		}
		else
		{
			byte b;
			if (byte::TryParse(s, Globalization::NumberStyles::HexNumber, Globalization::CultureInfo::CurrentCulture, b))
			{
				bPattern.Add(Convert::ToByte(s, 16));
				Mask.Add(true);
			}
			else
			{
				break;
			}
		}
	}

	int ix, iy;
	bool bFound = false;
	int patternLength = bPattern.Count;
	int dataLength = bBuffer->Length - patternLength;

	for (ix = 0; ix < dataLength; ix++)
	{

		bFound = true;

		for (iy = 0; iy < patternLength; iy++)
		{
			if (Mask[iy] && bPattern[iy] != bBuffer[ix + iy])
			{
				bFound = false;
				break;
			}
		}

		if (bFound)
			return ix;

	}

	return -1;

}

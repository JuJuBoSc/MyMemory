#include <Pch/StdAfx.h>

String^ MyMemory::Utils::Randoms::GetRandomString(int minLength, int maxLength)
{
	StringBuilder^ stringBuilder;
	array<wchar_t>^ array = String("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789").ToCharArray();
	if (minLength < maxLength)
		stringBuilder = gcnew StringBuilder(s_randomString.Next(minLength, maxLength));
	else
		stringBuilder = gcnew StringBuilder(minLength);
	int num = 0;
	if (0 < stringBuilder->Capacity)
	{
		do
		{
			stringBuilder->Append(array[s_randomString.Next(array->Length)]);
			num++;
		} while (num < stringBuilder->Capacity);
	}
	return stringBuilder->ToString();
}

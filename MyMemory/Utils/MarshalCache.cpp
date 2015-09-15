#include <Pch/StdAfx.h>

generic<typename T>
bool MyMemory::Utils::MarshalCache<T>::IsTypeRequireMarshal(Type^ t)
{
	for each (FieldInfo^ fieldInfo in t->GetFields(BindingFlags::Instance | BindingFlags::Public | BindingFlags::NonPublic))
		if (fieldInfo->GetCustomAttributes(MarshalAsAttribute::typeid, true)->Length > 0)
			return true;
	return false;
}


#include <Pch/StdAfx.h>

MyMemory::Hooks::CallbackBase::CallbackBase(HookBase^ hook)
{
	m_hook = hook;
}

MyMemory::Hooks::CallbackBase::~CallbackBase()
{
	m_hook->RemoveCallback(this);
}

#pragma once
#include "address.h"
#include "structure.h"

namespace pkodev
{
	namespace pointer
	{
		// void CItemCommand::AddHint(int x, int y)
		typedef void* (__thiscall* CItemCommand__AddHint__Ptr)(void*, int, int);
		CItemCommand__AddHint__Ptr CItemCommand__AddHint = (CItemCommand__AddHint__Ptr)(void*)(address::MOD_EXE_VERSION::CItemCommand__AddHint);

		// void PushHint(const char* str, DWORD color = COLOR_WHITE, int height = 5, int font = 0, int index = -1)
		typedef void* (__thiscall* CCommandObj__PushHint__Ptr)(void*, const char*, unsigned int, int, int);
		CCommandObj__PushHint__Ptr CCommandObj__PushHint = (CCommandObj__PushHint__Ptr)(void*)(address::MOD_EXE_VERSION::CCommandObj__PushHint);

		// inline CItemRecord* GetItemRecordInfo( int nTypeID )
		typedef void* (__cdecl* GetItemRecordInfo__Ptr)(int);
		GetItemRecordInfo__Ptr GetItemRecordInfo = (GetItemRecordInfo__Ptr)(void*)(address::MOD_EXE_VERSION::GetItemRecordInfo);
	}
}
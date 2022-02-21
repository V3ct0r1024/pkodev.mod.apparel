#pragma once
#include "structure.h"

namespace pkodev
{
	namespace hook
	{
		// void CItemCommand::AddHint(int x, int y)
		void __fastcall CItemCommand__AddHint(void* This, void* NotUsed, int x, int y);

		// void PushHint(const char* str, DWORD color = COLOR_WHITE, int height = 5, int font = 0, int index = -1)
		void __fastcall CCommandObj__PushHint(void* This, void* NotUsed, const char* str, unsigned int color, int height, int font);

	}
}
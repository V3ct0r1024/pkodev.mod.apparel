#include <Windows.h>
#include <detours.h>

#include <cstdio>

#include "loader.h"

#include "address.h"
#include "pointer.h"
#include "hook.h"
#include "structure.h"

namespace pkodev
{
    // Is the program executing the void CItemCommand::AddHint(int x, int y) method at the moment?
    bool CItemCommand__AddHint__call{ false };

    // The first hint flag
    bool first_hint{ false };

    // The item name in the apparel
    char* name{ nullptr };
}


// Entry point
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    // Nothing to do . . .
    return TRUE;
}

// Get mod information
void GetModInformation(mod_info& info)
{
    strcpy_s(info.name, TOSTRING(MOD_NAME));
    strcpy_s(info.version, TOSTRING(MOD_VERSION));
    strcpy_s(info.author, TOSTRING(MOD_AUTHOR));
    info.exe_version = MOD_EXE_VERSION;
}

// Start the mod
void Start(const char* path)
{
    // Enable hooks
    DetourRestoreAfterWith();
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&(PVOID&)pkodev::pointer::CItemCommand__AddHint, pkodev::hook::CItemCommand__AddHint);
    DetourAttach(&(PVOID&)pkodev::pointer::CCommandObj__PushHint, pkodev::hook::CCommandObj__PushHint);
    DetourTransactionCommit();
}

// Stop the mod
void Stop()
{
    // Disable hooks
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourDetach(&(PVOID&)pkodev::pointer::CItemCommand__AddHint, pkodev::hook::CItemCommand__AddHint);
    DetourDetach(&(PVOID&)pkodev::pointer::CCommandObj__PushHint, pkodev::hook::CCommandObj__PushHint);
    DetourTransactionCommit();
}

// void CItemCommand::AddHint(int x, int y)
void __fastcall pkodev::hook::CItemCommand__AddHint(void* This, void* NotUsed,
    int x, int y)
{
    // Reset the first hint flag
    pkodev::first_hint = false;

    // Reset the pointer to the item name in the apparel
    pkodev::name = nullptr;

    // Get pointer to the data of the item rendering
    void *data = reinterpret_cast<void*>(
        *reinterpret_cast<unsigned int*>(
            reinterpret_cast<unsigned int>(This) + 0x38
        )
    );

    // Get the item ID
    unsigned int id = *reinterpret_cast<unsigned int*>(
        reinterpret_cast<unsigned int>(data) + 0x6C
    );

    // Check that the item is the apparel one
    if (id >= 5000)
    {
        // Get the item type
        unsigned short int type = *reinterpret_cast<unsigned short int*>(
            reinterpret_cast<unsigned int>(data) + 0x0134
        );

        // Check the item type
        bool isWeapon = (type >= 1 && type <= 10);
        bool isDefenceType = (type == 11 || type == 22 || type == 27);
        bool isJewelery = (type == 25 || type == 26 || type == 81 || type == 82 || type == 83);
        bool isEquip = (type == 20 || type == 23 || type == 24 || type == 88 || type == 84);

        if ( (isWeapon == true) || (isDefenceType == true) 
            || (isJewelery == true) || (isEquip == true) )
        {
            // Get the item id in the apparel
            unsigned short int app_id = *reinterpret_cast<unsigned short int*>(
                reinterpret_cast<unsigned int>(This) + 0x50
            );

            // Check that the apparel has the item
            if (app_id != 0)
            {
                // Get the item data
                void* record = pkodev::pointer::GetItemRecordInfo(app_id);

                // Check that item is found
                if (record != nullptr)
                {
                    // Get the item name
                    pkodev::name = reinterpret_cast<char*>(
                        reinterpret_cast<unsigned int>(record) + 0x70
                    );
                }
            }
        }
    }

    // Call the original method void CItemCommand::AddHint(int x, int y)
    pkodev::CItemCommand__AddHint__call = true;
    pkodev::pointer::CItemCommand__AddHint(This, x, y);
    pkodev::CItemCommand__AddHint__call = false;
}

// void CCommandObj::PushHint( const char* str, DWORD color, int height, int font, int index, bool shadow, DWORD scolor )
void __fastcall pkodev::hook::CCommandObj__PushHint(void* This, void* NotUsed, 
    const char* str, unsigned int color, int height, int font)
{
    // Call the original method void PushHint(const char* str, DWORD color = COLOR_WHITE, int height = 5, int font = 0, int index = -1)
    pkodev::pointer::CCommandObj__PushHint(This, str, color, height, font);

    // Check that the void CItemCommand::AddHint(int x, int y) is executing
    if (pkodev::CItemCommand__AddHint__call == true)
    {
        // Check that the item name is rendered
        if (pkodev::first_hint == false)
        {
            // The item name was rendered
            pkodev::first_hint = true;

            // Render the item name in the apparel
            if (pkodev::name != nullptr)
            {
                // Build the item name string
                static char buf[128]{ 0x00 };
                sprintf_s(buf, sizeof(buf), "(%s)", pkodev::name);

                // Render the item name in the apparel
                pkodev::pointer::CCommandObj__PushHint(This, buf, 0xFFFFA500, height, font);
            }
        }
    }
}
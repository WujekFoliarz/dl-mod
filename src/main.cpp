#include <pch.hpp>
#include "function_loader.hpp"
#include "game/engine_x64_rwdi/structs.hpp"
#include "game/engine_x64_rwdi/structs.hpp"
#include "game/gamedll_x64_rwdi/symbols.hpp"
#include "game/gamedll_x64_rwdi/structs.hpp"
#include "component_loader.hpp"

// 180091C00
// 180F30F90 <- check these out later

void hook_game_ctor()
{
    HMODULE hGame = nullptr;
    while (!hGame)
    {
        hGame = GetModuleHandleA("gamedll_x64_rwdi.dll");
        if (!hGame)
        {
            Sleep(500);
        }
    }

    void* ctorAddr = reinterpret_cast<void*>(
        reinterpret_cast<uintptr_t>(hGame) + 
        std::get<uint32_t>(game::gamedll_x64_rwdi::symbols.at("IGame::ctor"))
    );
    auto status = MH_CreateHook(ctorAddr, reinterpret_cast<void*>(&game::gamedll_x64_rwdi::IGame::hooked_game_ctor), reinterpret_cast<void**>(&game::gamedll_x64_rwdi::IGame::original_game_ctor));
    status = MH_EnableHook(ctorAddr);

    MH_CreateHook(function_loader::get_function_address("Sessions::StatusDL::ctor"), reinterpret_cast<void*>(&game::gamedll_x64_rwdi::Sessions::StatusDL::hooked_status_ctor), (void**)&game::gamedll_x64_rwdi::Sessions::StatusDL::original_status_ctor);
    MH_EnableHook(function_loader::get_function_address("Sessions::StatusDL::ctor"));
}

std::atomic<bool> running = true;
void start()
{
    // console FIRST
    AllocConsole();
    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);
    freopen("CONIN$", "r", stdin);

    std::cout << "start() called" << std::endl;

    MH_Initialize();

    function_loader::load_functions();
    hook_game_ctor();

    std::this_thread::sleep_for(std::chrono::seconds(1));

    if (!component_loader::pre_load() || !component_loader::post_load())
	{
		throw std::runtime_error("premature shutdown");
	}

	component_loader::start();
	component_loader::post_start();
}

BOOL WINAPI DllMain(
    HINSTANCE hinstDLL,  // handle to DLL module
    DWORD fdwReason,     // reason for calling function
    LPVOID lpvReserved )  // reserved
{
    switch( fdwReason ) 
    { 
        case DLL_PROCESS_ATTACH:
            CreateThread(nullptr, 0, [](LPVOID) -> DWORD {
                start();
                return 0;
            }, nullptr, 0, nullptr);
            break;

        case DLL_PROCESS_DETACH:
            running = false;
            if (lpvReserved != nullptr)
            {
                break; 
            }
            
            break;
    }
    return TRUE;  
}
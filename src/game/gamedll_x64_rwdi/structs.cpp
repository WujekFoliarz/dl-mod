#include <pch.hpp>
#include "symbols.hpp"
#include "structs.hpp"

void* g_pGame = nullptr;

void *game::gamedll_x64_rwdi::IGame::hooked_game_ctor(void *a1)
{
    void* result = original_game_ctor(a1);
    g_pGame = result;
    std::cout << "Game constructed: " << g_pGame << std::endl;
    return result;
}

game::engine_x64_rwdi::IGame::impl::IGame *game::gamedll_x64_rwdi::IGame::get_game()
{
    return static_cast<game::engine_x64_rwdi::IGame::impl::IGame*>(g_pGame);
}

game::engine_x64_rwdi::Sessions::StatusDL::impl::StatusDL* g_pStatus = nullptr;
void *game::gamedll_x64_rwdi::Sessions::StatusDL::hooked_status_ctor(game::engine_x64_rwdi::Sessions::StatusDL::impl::StatusDL* this_)
{
    auto result = original_status_ctor(this_);
    g_pStatus = this_;
    std::cout << "StatusDL constructed: " << g_pStatus << std::endl;
    return result;
}

game::engine_x64_rwdi::Sessions::StatusDL::impl::StatusDL *game::gamedll_x64_rwdi::Sessions::StatusDL::get_dl_status()
{
    return g_pStatus;
}


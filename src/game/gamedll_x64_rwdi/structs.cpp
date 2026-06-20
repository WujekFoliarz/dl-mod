#include <pch.hpp>
#include "symbols.hpp"
#include "structs.hpp"

void* g_pGame = nullptr;
game::gamedll_x64_rwdi::Sessions::StatusDL::status_dl_list sessions;
std::mutex sessions_lock;

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

void *game::gamedll_x64_rwdi::Sessions::StatusDL::hooked_status_ctor(game::engine_x64_rwdi::Sessions::StatusDL::impl::StatusDL* this_)
{
    auto result = original_status_ctor(this_);
    std::lock_guard<std::mutex> lock(sessions_lock);
    sessions.push_back(this_);
    std::cout << "StatusDL constructed: " << this_ << std::endl;
    return result;
}

game::gamedll_x64_rwdi::Sessions::StatusDL::status_dl_list game::gamedll_x64_rwdi::Sessions::StatusDL::get_status_dl()
{
    std::lock_guard<std::mutex> lock(sessions_lock);
    std::erase_if(sessions, [](game::engine_x64_rwdi::Sessions::StatusDL::impl::StatusDL* s)
    {
        return !s || 
        (s->m_Team != game::engine_x64_rwdi::Sessions::StatusDL::ETeam::Default && 
        s->m_Team != game::engine_x64_rwdi::Sessions::StatusDL::ETeam::Zombie);
    });
    std::erase(sessions, nullptr);

    return sessions;
}


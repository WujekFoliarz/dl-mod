#pragma once

namespace session
{
    const std::string& get_current_map();
    bool is_being_invaded_while_human();
    bool is_playing_as_zombie();
    int32_t get_player_count();
}
#pragma once

namespace session
{
    const std::string& get_current_map();
    bool is_being_invaded_while_human();
    uint32_t get_quest_uid();
    int32_t get_player_count();
    bool is_night_hunter();
    int get_game_visibility();
}
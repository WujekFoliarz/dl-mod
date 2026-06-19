#include <pch.hpp>
#include "structs.hpp"

std::string game::engine_x64_rwdi::Sessions::StatusDL::ETeam_to_string(ETeam Type)
{
    switch(Type)
    {
        case ETeam::None: return "None";
        case ETeam::Default: return "Default";
        case ETeam::Zombie: return "Zombie";
        default: return "Unknown";
    }
    return std::string();
}

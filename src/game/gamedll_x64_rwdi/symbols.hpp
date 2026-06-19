#pragma once

#include "symbol.hpp"

namespace game
{
    namespace gamedll_x64_rwdi
    {
        inline const utils::symbol_map symbols =
        {
            { "IGame::ctor",                static_cast<uint32_t>(0x1811120F0u - IDA_BASE_IMAGE) },
            { "Sessions::StatusDL::ctor",   static_cast<uint32_t>(0x180F30E50u - IDA_BASE_IMAGE) },
            { "LobbyState",                 static_cast<uint32_t>(0x180F30F90u - IDA_BASE_IMAGE) },
        };
    }
}
#pragma once

#include "symbol.hpp"

namespace game
{
    namespace gamedll_x64_rwdi
    {
        inline const utils::symbol_map symbols =
        {
            { "IGame::ctor",                static_cast<uint32_t>(0x1811120F0u - IDA_BASE_IMAGE) },
            { "GameDI::OnFrame",            static_cast<uint32_t>(0x1804408D0u - IDA_BASE_IMAGE) },
            { "GSesssion",                  static_cast<uint32_t>(0x181C15318u - IDA_BASE_IMAGE) },
            
            //{ "LogicalPlayer::ctor",        static_cast<uint32_t>(0x180490BB0u - IDA_BASE_IMAGE) },
            //{ "LogicalPlayer::dtor",        static_cast<uint32_t>(0x1804951A8u - IDA_BASE_IMAGE) },
            //{ "PlayerDI::ctor",             static_cast<uint32_t>(0x180B8AEC0u - IDA_BASE_IMAGE) },
            //{ "GetCampaignInstance",        static_cast<uint32_t>(0x180357260u - IDA_BASE_IMAGE) },
            //{ "Sessions::StatusDL::ctor",   static_cast<uint32_t>(0x180F30E50u - IDA_BASE_IMAGE) },
        };
    }
}
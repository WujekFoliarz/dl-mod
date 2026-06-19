#pragma once

#include "symbol.hpp"

namespace game
{
    namespace engine_x64_rwdi
    {
        inline const utils::symbol_map symbols =
        {
            { "IGame::GetActiveLevel", "?GetActiveLevel@IGame@@QEAAPEAVILevel@@XZ"},
            { "ILevel::IsLoading", "?IsLoading@ILevel@@QEBA_NXZ"},
            { "ILevel::GetLevelName", "?GetLevelName@ILevel@@QEAAPEBDXZ" },
            { "ILevel::GetLevelPath", "?GetLevelPath@ILevel@@QEBA?AV?$string_base@D@ttl@@XZ" },
        };
    }
}
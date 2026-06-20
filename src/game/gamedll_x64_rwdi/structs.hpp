#pragma once

#include "game/engine_x64_rwdi/structs.hpp"

namespace game
{
    namespace gamedll_x64_rwdi
    {
        namespace Sessions
        {
            namespace StatusDL
            {
                typedef void*(__fastcall* t_status_ctor)(game::engine_x64_rwdi::Sessions::StatusDL::impl::StatusDL* this_);
                inline t_status_ctor original_status_ctor = nullptr;

                void* hooked_status_ctor(game::engine_x64_rwdi::Sessions::StatusDL::impl::StatusDL* this_);
                
                using status_dl_list = std::vector<game::engine_x64_rwdi::Sessions::StatusDL::impl::StatusDL*>;
                game::gamedll_x64_rwdi::Sessions::StatusDL::status_dl_list get_status_dl();
            }
        }

        namespace IGame
        {
            using GameCtor_t = void*(*)(void*);
            inline GameCtor_t original_game_ctor = nullptr;
            void* hooked_game_ctor(void* a1);

            typedef void (__fastcall* t_OnLobbyRemotePeerJoined)(game::engine_x64_rwdi::IGame::impl::IGame* pThis, unsigned int peer_id);
            inline t_OnLobbyRemotePeerJoined original_OnLobbyRemotePeerJoined = nullptr;
        
            typedef void(__fastcall* t_OnLobbyRemotePeerStartedJoining)(game::engine_x64_rwdi::IGame::impl::IGame* this_, unsigned int peerid, game::engine_x64_rwdi::Sessions::StatusDL::ETeam team);
            inline t_OnLobbyRemotePeerStartedJoining original_OnLobbyRemotePeerStartedJoining = nullptr;

            typedef void (__fastcall* t_OnLobbyRemotePeerLeft)(game::engine_x64_rwdi::IGame::impl::IGame* pThis, unsigned int peerId, game::engine_x64_rwdi::Sessions::StatusDL::EDisconnectReason disconnect_reason);
            inline t_OnLobbyRemotePeerLeft original_OnLobbyRemotePeerLeft = nullptr;
        
            typedef void(__fastcall* t_OnLobbyRemotePeerFailedJoining)(game::engine_x64_rwdi::IGame::impl::IGame* this_, unsigned int peer_id, game::engine_x64_rwdi::Sessions::StatusDL::EDisconnectReason disconnect_reason);
            inline t_OnLobbyRemotePeerFailedJoining original_OnLobbyRemotePeerFailedJoining = nullptr;

            typedef void*(__fastcall* t_LobbyState)(void* a1, void* a2);
            inline t_LobbyState original_LobbyState = nullptr;

            game::engine_x64_rwdi::IGame::impl::IGame* get_game();
        }

        namespace GameDI
        {
            typedef void(__fastcall* t_OnFrame)(void* a1);
            inline t_OnFrame org_OnFrame = nullptr;
        }
    }
}
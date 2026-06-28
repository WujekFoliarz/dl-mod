#pragma once

#include "game/engine_x64_rwdi/structs.hpp"

namespace game
{
    namespace gamedll_x64_rwdi
    {
        namespace CampaignManager
        {
            typedef game::engine_x64_rwdi::CampaignManager::impl::CampaignManager*(__fastcall* t_get_campaign_manager_instance)();
            inline t_get_campaign_manager_instance org_get_campaign_manager_instance = nullptr;
        }

        namespace PlayerDI
        {
            typedef void*(__fastcall* t_playerdi_ctor)(game::engine_x64_rwdi::PlayerDI::impl::PlayerDI* playerdi);
            inline t_playerdi_ctor org_playerdi_ctor = nullptr;
            void* hooked_playerdi_ctor(game::engine_x64_rwdi::PlayerDI::impl::PlayerDI* playerdi);

            game::engine_x64_rwdi::PlayerDI::impl::PlayerDI* get_playerdi();
        }
    
        namespace LogicalPlayer
        {
            namespace impl
            {
                struct LogicalPlayer_vftable
                {

                };

                #pragma pack(push, 1)
                struct LogicalPlayer
                {
                    LogicalPlayer_vftable* vtable;
                    char _pad1[1752];                
                    uint32_t m_ReplTeam;            
                    char _pad2[448];                 
                    uint32_t m_ReplTrackedQuestUid;  
                    char _pad3[46];
                    uint32_t m_IsLocalPlayer;
                    char _pad4[62];
                };
                #pragma pack(pop)

                static_assert(sizeof(LogicalPlayer) == 2328, "LogicalPlayer size mismatch! Expected 2328 bytes.");
            }

            typedef void*(__fastcall* t_logical_player_ctor)(impl::LogicalPlayer* player);
            inline t_logical_player_ctor org_logical_player_ctor = nullptr;
            void* hooked_logical_player_ctor(impl::LogicalPlayer* player);

            typedef void*(__fastcall* t_logical_player_dtor)(impl::LogicalPlayer* player);
            inline t_logical_player_dtor org_logical_player_dtor = nullptr;
            void* hooked_logical_player_dtor(impl::LogicalPlayer* player);

            game::gamedll_x64_rwdi::LogicalPlayer::impl::LogicalPlayer* get_logical_player();
        }

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

            typedef int(__fastcall* t_ReplGetLocalId)(game::engine_x64_rwdi::IGame::impl::IGame* pThis);
            inline t_ReplGetLocalId original_ReplGetLocalId = nullptr;

            game::engine_x64_rwdi::IGame::impl::IGame* get_game();
        }

        namespace GameDI
        {
            typedef void(__fastcall* t_OnFrame)(void* a1);
            inline t_OnFrame org_OnFrame = nullptr;
        }
    }
}
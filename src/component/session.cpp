#include <pch.hpp>
#include "component_loader.hpp"
#include "session.hpp"

#include "game/engine_x64_rwdi/structs.hpp"
#include "game/gamedll_x64_rwdi/structs.hpp"

#include "function_loader.hpp"
#include "scheduler.hpp"

namespace session
{

	namespace 
	{
        std::string current_map = "";
        std::mutex current_map_lock;
        std::unordered_map<uint32_t, game::engine_x64_rwdi::Sessions::StatusDL::ETeam> connected_peers;
        std::mutex connected_peers_lock;
        std::atomic<game::engine_x64_rwdi::Sessions::StatusDL::ETeam> current_team = game::engine_x64_rwdi::Sessions::StatusDL::ETeam::None;
        std::atomic<unsigned short> zombie_slot_status = 0;
        static constexpr int ZOMBIE_SLOT_OCCUPIED = 2;

        void OnLobbyRemotePeerStartedJoining_stub(game::engine_x64_rwdi::IGame::impl::IGame* this_, unsigned int peer_id, game::engine_x64_rwdi::Sessions::StatusDL::ETeam team)
        {
            game::gamedll_x64_rwdi::IGame::original_OnLobbyRemotePeerStartedJoining(this_, peer_id, team);
            std::cout << "[discord][OnLobbyRemotePeerStartedJoining] peer_id: " << peer_id << " team: " << game::engine_x64_rwdi::Sessions::StatusDL::ETeam_to_string(team).c_str() << std::endl;
            {
                std::lock_guard lock(connected_peers_lock); 
                connected_peers.insert({peer_id, team});
            }
        }

        void OnLobbyRemotePeerJoined_stub(game::engine_x64_rwdi::IGame::impl::IGame* this_, unsigned int peer_id)
        {
            game::gamedll_x64_rwdi::IGame::original_OnLobbyRemotePeerJoined(this_, peer_id);
            std::cout << "[discord][OnLobbyRemotePeerJoined_stub] peer_id: " << peer_id << std::endl;
        }

        void OnLobbyRemotePeerLeft_stub(game::engine_x64_rwdi::IGame::impl::IGame* this_, unsigned int peer_id, game::engine_x64_rwdi::Sessions::StatusDL::EDisconnectReason disconnect_reason)
        {
            game::gamedll_x64_rwdi::IGame::original_OnLobbyRemotePeerLeft(this_, peer_id, disconnect_reason);
            std::cout << "[discord][OnLobbyRemotePeerLeft_stub] peer_id: " << peer_id << " disconnect_reason: " << (int)disconnect_reason << std::endl;
            {
                std::lock_guard lock(connected_peers_lock); 
                connected_peers.erase(peer_id);
            }
        }

        void OnLobbyRemotePeerFailedJoining_stub(game::engine_x64_rwdi::IGame::impl::IGame* this_, unsigned int peer_id, game::engine_x64_rwdi::Sessions::StatusDL::EDisconnectReason disconnect_reason)
        {
            game::gamedll_x64_rwdi::IGame::original_OnLobbyRemotePeerFailedJoining(this_, peer_id, disconnect_reason);
            std::cout << "[discord][OnLobbyRemotePeerFailedJoining_stub] peer_id: " << peer_id << " disconnect_reason: " << (int)disconnect_reason << std::endl;
            {
                std::lock_guard lock(connected_peers_lock); 
                connected_peers.erase(peer_id);
            }
        }

        const char* GetLevelName_stub(game::engine_x64_rwdi::ILevel::impl::ILevel* level)
        {
            auto result = game::engine_x64_rwdi::ILevel::org_GetLevelName(level);
            {
                std::lock_guard<std::mutex> lock(current_map_lock);
                current_map = result;
            }
            return result;
        }

        //game::engine_x64_rwdi::CampaignManager::impl::CampaignManager* get_campaign_manager_instance_stub()
        //{
        //
        //}
	}

	class component final : public component_interface
	{
	public:
		void pre_load() override
		{

		}

		void start() override
		{
            std::cout << "[session] starting" << std::endl;

            auto game = game::gamedll_x64_rwdi::IGame::get_game();

            MH_CreateHook((void*)game->vtable->OnLobbyRemotePeerStartedJoining, reinterpret_cast<void*>(&OnLobbyRemotePeerStartedJoining_stub), (void**)&game::gamedll_x64_rwdi::IGame::original_OnLobbyRemotePeerStartedJoining);
            MH_EnableHook((void*)game->vtable->OnLobbyRemotePeerStartedJoining);

            MH_CreateHook((void*)game->vtable->OnLobbyRemotePeerJoined, reinterpret_cast<void*>(&OnLobbyRemotePeerJoined_stub), (void**)&game::gamedll_x64_rwdi::IGame::original_OnLobbyRemotePeerJoined);
            MH_EnableHook((void*)game->vtable->OnLobbyRemotePeerJoined);

            MH_CreateHook((void*)game->vtable->OnLobbyRemotePeerLeft, reinterpret_cast<void*>(&OnLobbyRemotePeerLeft_stub), (void**)&game::gamedll_x64_rwdi::IGame::original_OnLobbyRemotePeerLeft);
            MH_EnableHook((void*)game->vtable->OnLobbyRemotePeerLeft);

            MH_CreateHook((void*)game->vtable->OnLobbyRemotePeerFailedJoining, reinterpret_cast<void*>(&OnLobbyRemotePeerFailedJoining_stub), (void**)&game::gamedll_x64_rwdi::IGame::original_OnLobbyRemotePeerFailedJoining);
            MH_EnableHook((void*)game->vtable->OnLobbyRemotePeerFailedJoining);

            MH_CreateHook((void*)function_loader::get_function_address("ILevel::GetLevelName"), reinterpret_cast<void*>(&GetLevelName_stub), (void**)&game::engine_x64_rwdi::ILevel::org_GetLevelName);
            MH_EnableHook((void*)function_loader::get_function_address("ILevel::GetLevelName"));

            std::cout << "[session] started" << std::endl;
		}
	};

    const std::string &get_current_map()
    {
        std::lock_guard<std::mutex> lock(current_map_lock);
        return current_map;
    }

    bool is_being_invaded_while_human()
    {          
        bool zombie_peer_present = false;
        {   
            std::lock_guard lock(connected_peers_lock);
            for(auto& [peer_id, team] : connected_peers)
            {
                if (team == game::engine_x64_rwdi::Sessions::StatusDL::ETeam::Zombie)
                {
                    zombie_peer_present = true;
                }
            }
        }

        if (zombie_peer_present)
        {
            return true;
        }

        return false;
    }

    int32_t get_player_count()
    {
        return connected_peers.size() + 1;
    }

    bool is_night_hunter()
    {
        __int64 base    = *(__int64*)(function_loader::get_function_address("GSesssion"));
        __int64 session = *(__int64*)(base + 1344);
        if (!session) return false;

        __int64 unk = *(__int64*)(session + 72);
        if (!unk) return false;

        __int64 playerObj = *(__int64*)(unk + 2520);
        if (!playerObj) return false;

        return *(int*)(playerObj + 1760) == 2;
    }

    int get_game_visibility()
    {
        __int64 base = *(__int64*)(function_loader::get_function_address("GSesssion"));
        if (!base) return -1;

        using Fn = int(__fastcall*)(__int64);
        auto vtable = *(__int64*)(base);
        auto fn = *(Fn*)(vtable + 2240);
        return fn(base);
    }
}

REGISTER_COMPONENT(session::component)
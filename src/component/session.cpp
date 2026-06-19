#include <pch.hpp>
#include "component_loader.hpp"
#include "session.hpp"

#include "game/engine_x64_rwdi/structs.hpp"
#include "game/gamedll_x64_rwdi/structs.hpp"

#include "function_loader.hpp"

namespace session
{

	namespace 
	{
        std::string current_map = "";
        std::unordered_map<uint32_t, game::engine_x64_rwdi::Sessions::StatusDL::ETeam> connected_peers;
        std::mutex connected_peers_lock;
        std::atomic<game::engine_x64_rwdi::Sessions::StatusDL::ETeam> current_team = game::engine_x64_rwdi::Sessions::StatusDL::ETeam::None;
        std::atomic<unsigned short> zombie_slot_status = 0;
        static constexpr int ZOMBIE_SLOT_OCCUPIED = 2;

        void OnLobbyRemotePeerStartedJoining_stub(game::engine_x64_rwdi::IGame::impl::IGame* this_, unsigned int peer_id, game::engine_x64_rwdi::Sessions::StatusDL::ETeam team)
        {
            std::cout << "[discord][OnLobbyRemotePeerStartedJoining] peer_id: " << peer_id << " team: " << game::engine_x64_rwdi::Sessions::StatusDL::ETeam_to_string(team).c_str() << std::endl;
            {
                std::lock_guard lock(connected_peers_lock); 
                connected_peers.insert({peer_id, team});
            }
            game::gamedll_x64_rwdi::IGame::original_OnLobbyRemotePeerStartedJoining(this_, peer_id, team);
        }

        void OnLobbyRemotePeerJoined_stub(game::engine_x64_rwdi::IGame::impl::IGame* this_, unsigned int peer_id)
        {
            std::cout << "[discord][OnLobbyRemotePeerJoined_stub] peer_id: " << peer_id << std::endl;
            game::gamedll_x64_rwdi::IGame::original_OnLobbyRemotePeerJoined(this_, peer_id);
        }

        void OnLobbyRemotePeerLeft_stub(game::engine_x64_rwdi::IGame::impl::IGame* this_, unsigned int peer_id, game::engine_x64_rwdi::Sessions::StatusDL::EDisconnectReason disconnect_reason)
        {
            std::cout << "[discord][OnLobbyRemotePeerLeft_stub] peer_id: " << peer_id << " disconnect_reason: " << (int)disconnect_reason << std::endl;
            {
                std::lock_guard lock(connected_peers_lock); 
                connected_peers.erase(peer_id);
            }
            game::gamedll_x64_rwdi::IGame::original_OnLobbyRemotePeerLeft(this_, peer_id, disconnect_reason);
        }

        void OnLobbyRemotePeerFailedJoining_stub(game::engine_x64_rwdi::IGame::impl::IGame* this_, unsigned int peer_id, game::engine_x64_rwdi::Sessions::StatusDL::EDisconnectReason disconnect_reason)
        {
            std::cout << "[discord][OnLobbyRemotePeerFailedJoining_stub] peer_id: " << peer_id << " disconnect_reason: " << (int)disconnect_reason << std::endl;
            {
                std::lock_guard lock(connected_peers_lock); 
                connected_peers.erase(peer_id);
            }
            game::gamedll_x64_rwdi::IGame::original_OnLobbyRemotePeerFailedJoining(this_, peer_id, disconnect_reason);
        }
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

            std::cout << "[session] started" << std::endl;
		}

        void update() override
        {
            auto game = game::gamedll_x64_rwdi::IGame::get_game();

            if (!game)
            {
                return;
            }

            auto level = function_loader::call<game::engine_x64_rwdi::ILevel::impl::ILevel*>("IGame::GetActiveLevel", game);
        
            if (!level)
            {
                return;
            }

            current_map = function_loader::call<const char*>("ILevel::GetLevelName", level);

            // So this actually takes StatusDL of the latest player that joined
            // This isn't yours if you're playing in coop!
            auto session = game::gamedll_x64_rwdi::Sessions::StatusDL::get_dl_status();
            if (session)
            {
                current_team = session->m_Team;
                zombie_slot_status = session->m_ZombieTeamSlot;
            }
            else
            {
                current_team = game::engine_x64_rwdi::Sessions::StatusDL::ETeam::None;
                zombie_slot_status = 65535;
            }
        }
	};

    const std::string &get_current_map()
    {
        return current_map;
    }

    bool is_being_invaded_while_human()
    {          
        std::lock_guard lock(connected_peers_lock); 
        for(auto& [peer_id, team] : connected_peers)
        {
            if (team == game::engine_x64_rwdi::Sessions::StatusDL::ETeam::Zombie)
            {
                return true;
            }
        }

        return false;
    }

    bool is_playing_as_zombie()
    {  
        if (current_team == game::engine_x64_rwdi::Sessions::StatusDL::ETeam::Zombie)
        {
            return true;
        }

        bool zombie_peer_present = false;
        std::lock_guard lock(connected_peers_lock); 
        for(auto& [peer_id, team] : connected_peers)
        {
            if (team == game::engine_x64_rwdi::Sessions::StatusDL::ETeam::Zombie)
            {
                zombie_peer_present = true;
                break;
            }
        }

        if (!zombie_peer_present && zombie_slot_status <= 1000)
        {
            return true;
        }

        return false;
    }

    int32_t get_player_count()
    {
        return connected_peers.size() + 1;
    }
}

REGISTER_COMPONENT(session::component)
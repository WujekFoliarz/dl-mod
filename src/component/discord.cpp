#include <pch.hpp>
#include "component_loader.hpp"
#include <discord-rpc.hpp>
#include "session.hpp"

namespace discord
{

	namespace 
	{
        constexpr auto APPLICATION_ID = "1013441050225950852";
        static int64_t start_time;
        static bool send_presence = true;
        static bool reinit = false;

        std::string current_map = "none";
        std::string last_valid_current_map = "none";
        std::string details = "";
        std::string small_image = "";

        static inline const std::unordered_map<std::string, std::string> pretty_string_map = 
        {
            {"none", ""},
            {"slums", "Slums"},
            {"countryside", "Countryside"},
            {"hellraid", "Hellraid"},
            {"human", "Human"},
            {"hunter", "Hunter"},
            {"old_town", "Old Town"},
            {"prison", "Prison"},
            {"slums_btz_tutorial", "Be The Zombie Lobby"},
            {"stadium", "Stadium"},
            {"menu_lvl_new", "Main Menu"}
        };

        void discord_setup()
        {
            discord::RPCManager::get().setClientID(APPLICATION_ID)
                .onReady([](discord::User const& user) {
                    std::cout << std::format("[discord] connected to user {}#{} - {}", user.username, user.discriminator, user.id) << std::endl;
                })
                .onDisconnected([](int errcode, std::string_view message) {
                    std::cout << std::format("[discord] disconnected with error code {} - {}", errcode, message) << std::endl;
                    reinit = true;
                })
                .onErrored([](int errcode, std::string_view message) {
                    std::cout << std::format("[discord] error with code {} - {}", errcode, message) << std::endl;
                });
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
            std::cout << "[discord] starting" << std::endl;
            discord_setup();
            discord::RPCManager::get().initialize();
            std::cout << "[discord] started" << std::endl;
            start_time = time(nullptr);
		}

        void update() override
        {
            auto& rpc = discord::RPCManager::get();
            if (!send_presence) 
            {
                rpc.clearPresence();
                return;
            }

            if(reinit)
            {
                reinit = false;
                rpc.initialize();
            }

            current_map = session::get_current_map();

            if (current_map != "")
            {
                last_valid_current_map = current_map;
            }

            if (last_valid_current_map == "menu_lvl_new")
            {
                small_image = "";
            }
            else if (session::is_playing_as_zombie())
            {
                small_image = "hunter";
            }
            else
            {
                small_image = "human";
            }

            static std::string pretty_map_name = "";
            auto it = pretty_string_map.find(last_valid_current_map);
            if (it != pretty_string_map.end())
            {
                pretty_map_name = it->second;
            }
            else
            {
                pretty_map_name = "";
            }

            bool is_playing_as_zombie = session::is_playing_as_zombie();
            bool is_being_invaded_while_human = session::is_being_invaded_while_human();

            if (last_valid_current_map == "menu_lvl_new")
            {
                details = "";
            }
            else if (session::get_player_count() == 1 && !is_playing_as_zombie)
            {
                details = "Playing in Singleplayer";
            }
            else if (session::get_player_count() > 1 && !is_playing_as_zombie && !is_being_invaded_while_human)
            {
                details = "Playing in CO-OP";
            }
            else if (session::get_player_count() > 1 && !is_playing_as_zombie && is_being_invaded_while_human)
            {
                details = "Fighting the Night Hunter";
            }
            else if (session::get_player_count() == 1 && is_playing_as_zombie && !is_being_invaded_while_human)
            {
                details = "Looking for pray...";
            }
            else if (session::get_player_count() > 1 && is_playing_as_zombie && !is_being_invaded_while_human)
            {
                details = "Invading players";
            }
            else
            {
                details = "Playing the game";
            }

            rpc.getPresence()
                .setState(pretty_map_name)
                .setActivityType(discord::ActivityType::Game)
                .setStatusDisplayType(discord::StatusDisplayType::Name)
                .setDetails(details)
                .setStartTimestamp(start_time)
                .setEndTimestamp(0)
                .setLargeImageKey(last_valid_current_map)
                .setSmallImageKey(small_image)
                .setPartyID("party1234")
                .setPartySize(session::get_player_count())
                .setPartyMax(5)
                .setPartyPrivacy(discord::PartyPrivacy::Private)
                .setInstance(false)
                .refresh();
        }
	};
}

REGISTER_COMPONENT(discord::component)
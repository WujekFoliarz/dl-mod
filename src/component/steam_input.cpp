#include <pch.hpp>
#include "component_loader.hpp"
#include "steam_input.hpp"

namespace steam_input
{
	namespace 
	{

	}

	class component final : public component_interface
	{
	public:
		void pre_load() override
		{

		}

		void start() override
		{

		}
	};
}

REGISTER_COMPONENT(steam_input::component)
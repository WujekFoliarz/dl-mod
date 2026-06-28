#include <pch.hpp>
#include "function_loader.hpp"
#include "game/engine_x64_rwdi/symbols.hpp"
#include "game/gamedll_x64_rwdi/symbols.hpp"

namespace function_loader
{
    bool already_loaded = false;

    void* get_function_address(const std::string &function_name)
    {
        auto result = function_map.at(function_name);

        if (!result)
        {
            MessageBoxA(0, "Error", std::format("Failed to get function address: {}", function_name).c_str(), MB_ICONERROR);
            return nullptr;
        }

        return result;
    }

    void load_functions()
    {
        if (already_loaded)
        {
            return;
        }

        already_loaded = true;

        static std::vector<std::pair<HMODULE, utils::symbol_map>> modules = 
        {
            { GetModuleHandleA("engine_x64_rwdi.dll"), game::engine_x64_rwdi::symbols},
            { GetModuleHandleA("gamedll_x64_rwdi.dll"), game::gamedll_x64_rwdi::symbols}
        };

        for (auto& [module, symbol_map] : modules)
        {
            for (auto& [name, symbol] : symbol_map)
            {
                void* ptr = nullptr;
                if (std::holds_alternative<std::string>(symbol))
                {
                    ptr = reinterpret_cast<void*>(GetProcAddress(module, std::get<std::string>(symbol).c_str()));
                }
                else
                {
                    ptr = reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(module) + std::get<uint32_t>(symbol));
                    std::cout << std::format("[function_loader] Loaded raw address {}: {}", name, ptr) << std::endl;
                    function_map.try_emplace(name, ptr);
                    continue;
                }
                
                if (!ptr)
                {
                    auto msg = std::format("Failed to load {}", name);
                    MessageBoxA(nullptr, msg.c_str(), "Error", MB_ICONWARNING);
                    continue;
                }
                else
                {
                    std::cout << std::format("[function_loader] Loaded {}: {}", name, ptr) << std::endl;
                }

                function_map.try_emplace(name, ptr);
            }
        }

        std::cout << "[function_loader] Success" << std::endl;
    }
}

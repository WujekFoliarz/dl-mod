#pragma once

namespace function_loader
{
    using function_map_ = std::unordered_map<std::string, void*>;
    inline std::unordered_map<std::string, void*> function_map;

    void* get_function_address(const std::string& function_name);

    template <typename Ret, typename Class, typename... Args>
    Ret call(const std::string& name, Class* obj, Args&&... args)
    {
        auto it = function_map.find(name);
        if (it == function_map.end() || it->second == nullptr)
        {
            MessageBoxA(0, "Error", std::format("Failed to call function: {}", name).c_str(), MB_ICONERROR);
            return;
        }
        using Fn = Ret(__fastcall*)(Class*, Args...);
        auto fn = reinterpret_cast<Fn>(it->second);
        return fn(obj, std::forward<Args>(args)...);
    }
    
    template <typename Ret>
    Ret call_static(const std::string& name)
    {
        auto it = function_map.find(name);
        if (it == function_map.end() || it->second == nullptr)
        {
            MessageBoxA(0, "Error", std::format("Failed to call function: {}", name).c_str(), MB_ICONERROR);
            return;
        }
        using Fn = Ret(__fastcall*)();
        auto fn = reinterpret_cast<Fn>(it->second);
        return fn();
    }

    void load_functions();
}
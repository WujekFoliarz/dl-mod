#pragma once

namespace utils
{
    using symbol = std::variant<std::string, uint32_t>;
    using symbol_map = std::unordered_map<std::string, symbol>;
}
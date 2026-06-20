#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <string>
#include <cstdint>
#include <variant>
#include <unordered_map>
#include <format>
#include <tuple>
#include <iostream>
#include <thread>
#include <chrono>
#include <functional>
#include <mutex>
#include <map>
#include <iterator>
#include <cassert>

inline constexpr uint32_t IDA_BASE_IMAGE = 0x180000000u;

#include <MinHook.h>
#pragma once

#include <string>
#include <unordered_map>

namespace networkssy::error {
constexpr const int INVALID_SOCKETFD = -1; // NOLINT
extern const std::unordered_map<int, std::string> ERROR_MAP;
} // namespace networkssy::error

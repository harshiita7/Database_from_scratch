#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

namespace zeliadb {

constexpr std::size_t kPageSize = 4096;

using PageId = std::uint32_t;
using Page = std::array<std::uint8_t, kPageSize>;

Page make_empty_page();

}  // namespace zeliadb

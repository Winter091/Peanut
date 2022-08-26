#pragma once

#include <vector>
#include <cstdint>
#include <string>

namespace pn {

	std::vector<uint8_t> ReadFile(const std::string& filepath);

}
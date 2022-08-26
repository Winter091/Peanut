#include <Peanut/Core/Utils/FileUtils.hpp>

#include <Peanut/Core/Assert.hpp>

#include <fstream>

namespace pn {

    std::vector<uint8_t> ReadFile(const std::string& filepath)
    {
        std::ifstream file(filepath, std::ios::binary | std::ios::ate);
        PN_CORE_ENSURE(file, "Unable to open file for reading: {}", filepath);

        std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);

        std::vector<uint8_t> fileData(size);
        if (!file.read(reinterpret_cast<char*>(&fileData[0]), size)) {
            PN_CORE_ENSURE(false, "Unable to read data from file: {}", filepath);
        }
    
        return fileData;
    }

}

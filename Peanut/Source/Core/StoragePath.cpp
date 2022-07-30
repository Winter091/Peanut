#include "StoragePath.hpp"

#include <Peanut/Core/Assert.hpp>
#include <filesystem>

namespace pn {

std::string s_assetsPath;

void StoragePath::SetAssetsPath(const std::string& stringPath)
{
    auto absolutePath = std::filesystem::absolute(std::filesystem::path(stringPath));

    s_assetsPath = absolutePath.string();
    PN_CORE_DEBUG("Resolved assets path into {}", s_assetsPath);
    
    PN_CORE_ENSURE(std::filesystem::exists(absolutePath), "Assets path does not exist");
    PN_CORE_ENSURE(std::filesystem::is_directory(absolutePath), "Assets path is not a directory");
}

const std::string& StoragePath::GetAssetsPath()
{
    PN_CORE_ASSERT(!s_assetsPath.empty(), "Trying to get assets path that is not set");
    return s_assetsPath;
}

}

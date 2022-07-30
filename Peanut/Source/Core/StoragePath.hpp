#pragma once

#include <string>

namespace pn {

class StoragePath
{
public:
    static void SetAssetsPath(const std::string& stringPath);
    static const std::string& GetAssetsPath();
};

}

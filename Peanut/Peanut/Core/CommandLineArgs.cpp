#include "CommandLineArgs.hpp"

#include <Peanut/Core/Assert.hpp>

namespace pn {

CommandLineArgs::CommandLineArgs(int argc, const char** argv)
{
    m_args.reserve(argc);
    for (int i = 0; i < argc; i++) {
        PN_CORE_ASSERT(argv[i], "Command line argument ptr is nullptr");
        m_args.emplace_back(argv[i]);
    }
}

const std::string& CommandLineArgs::operator[](std::size_t index) const
{
    PN_CORE_ASSERT(index < m_args.size(), "Trying to access out-of-bounds element");
    return m_args[index];
}

}
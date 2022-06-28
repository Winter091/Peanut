#ifndef __COMMANDLINEARGS_H__
#define __COMMANDLINEARGS_H__

#include <vector>
#include <string>

namespace pn {

class CommandLineArgs
{
private:
    using ContainerT = std::vector<std::string>;

public:
    CommandLineArgs() = delete;
    CommandLineArgs(int argc, const char** argv);
    CommandLineArgs(CommandLineArgs&) = delete;
    CommandLineArgs& operator=(CommandLineArgs&) = delete;
    ~CommandLineArgs() = default;

    std::size_t GetSize() const { return m_args.size(); }
    const std::string& operator[](std::size_t index) const;

    ContainerT::iterator begin() { return m_args.begin(); }
    ContainerT::iterator end() { return m_args.end(); }
    ContainerT::const_iterator begin() const { return m_args.begin(); }
    ContainerT::const_iterator end() const { return m_args.end(); }

private:
    ContainerT m_args;
};

}

#endif
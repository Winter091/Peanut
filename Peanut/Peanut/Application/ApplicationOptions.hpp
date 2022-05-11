#ifndef __APPLICATIONOPTIONS_H__
#define __APPLICATIONOPTIONS_H__

#include <string>

namespace pn {

struct ApplicationOptions
{
    int width = 800;
    int height = 600;
    std::string title = "Peanut Application";
};

}

#endif
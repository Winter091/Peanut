#ifndef __WINDOWSETTINGS_H__
#define __WINDOWSETTINGS_H__

#include <string>

namespace pn {

struct WindowSettings 
{
    int width = 800;
    int height = 600;
    int swapInterval = 1;
    std::string title = "Peanut Window";
};

}

#endif
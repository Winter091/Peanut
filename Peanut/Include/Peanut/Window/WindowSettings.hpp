#pragma once

#include <Peanut/Core/Assert.hpp>
#include <string>

namespace pn {

    struct WindowSettings 
    {
    public:
        uint32_t Width = 800;
        uint32_t Height = 600;
        bool IsFullScreen = false;
        uint32_t SwapInterval = 1;
        std::string Title = "Peanut Window";

        WindowSettings& SetWidth(uint32_t width) { Width = width; return *this; }
        WindowSettings& SetHeight(uint32_t height) { Height = height; return *this; }
        WindowSettings& UseFullScreen(bool use) { IsFullScreen = use; return *this; }
        WindowSettings& SetSwapInterval(uint32_t swapInterval) { SwapInterval = swapInterval; return *this; }
        WindowSettings& SetTitle(const std::string& title) { Title = title; return *this; }
        WindowSettings& UseVsync(bool value) { SwapInterval = (value ? 1 : 0); return *this; }
    };

}
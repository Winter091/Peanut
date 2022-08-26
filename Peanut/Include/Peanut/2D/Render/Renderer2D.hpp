#pragma once

#include <Peanut/Scene/Camera/Camera.hpp>
#include <Peanut/2D/Shapes/Rectangle.hpp>

namespace pn {

    class Renderer2D
    {
    public:
        static void Init();
        static void Shutdown();

        static void BeginScene(const Camera& camera);
        static void EndScene();

        static void DrawRectangle(const Rectangle& rectangle);
    };

}

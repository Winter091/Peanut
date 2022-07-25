#ifndef __RENDERER2D_H__
#define __RENDERER2D_H__

#include <Peanut/Scene/Camera.hpp>
#include <Peanut/2D/Shapes/Rectangle.hpp>

namespace pn {

class Renderer2D
{
public:
    static void Init();
    static void Shutdown();

    static void BeginScene(const Camera& camera);
    static void EndScene();

    static void DrawShape(const Rectangle& rectangle);
};

}

#endif
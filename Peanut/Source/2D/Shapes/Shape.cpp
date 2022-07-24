#include <Peanut/2D/Shapes/Shape.hpp>

namespace pn {

Shape::Shape(const glm::vec2& pos, const glm::vec4& color)
    : m_pos(pos)
    , m_color(color)
{}

}
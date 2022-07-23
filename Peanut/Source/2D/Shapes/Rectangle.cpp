#include <Peanut/2D/Shapes/Rectangle.hpp>

namespace pn {

Rectangle::Rectangle(const glm::vec2& pos, const glm::vec2& size)
    : Shape(pos)
    , m_size(size)
{}

}
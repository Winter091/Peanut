#ifndef __SHAPE_H__
#define __SHAPE_H__

#include <glm/vec2.hpp>

namespace pn { 

class Shape
{
public:
    Shape(const glm::vec2& pos = {});
    virtual ~Shape() = default;

    virtual const glm::vec2& GetPos() const { return m_pos; }
    virtual void SetPos(const glm::vec2& pos) { m_pos = pos; }

protected:
    glm::vec2 m_pos;
};

}

#endif
#ifndef __RECTANGLE_H__
#define __RECTANGLE_H__

#include "Shape.hpp"

namespace pn {

class Rectangle : public Shape
{
public:
    Rectangle(const glm::vec2& pos);
    ~Rectangle() override = default;
};

}


#endif
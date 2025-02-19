#include "MyMath.h"

Vec2 operator+(const Vec2& op1, const Vec2& op2)
{
    return { op1.x + op2.x, op1.y + op2.y };
}

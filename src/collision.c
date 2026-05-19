#include "collision.h"

bool aabb_overlap(aabb_t a, aabb_t b)
{
    return (a.x < (uint8_t)(b.x + b.w)) && ((uint8_t)(a.x + a.w) > b.x) &&
           (a.y < (uint8_t)(b.y + b.h)) && ((uint8_t)(a.y + a.h) > b.y);
}

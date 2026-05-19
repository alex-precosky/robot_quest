#pragma once
#include <stdbool.h>
#include <stdint.h>

/**
 * Collision detection is based on checking for overlap between axis-aligned
 * bounding-boxes (AABBs)
 */
 
typedef struct {
    uint8_t x, y, w, h;
} aabb_t;

/**
 * @brief Check if two axis-aligned bounding-boxes overlap
 * @param a - One AABB
 * @param b - Another AABB
 * @return True if the two AABBs overlap
 */
bool aabb_overlap(aabb_t a, aabb_t b);

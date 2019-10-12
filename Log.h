#pragma once
#include "ray.h"
/*
Struct to contain information about last 
ray collision.
*/
class Material;

struct intersectionLog
{
    float t;
    Vec3 p;
    Vec3 normal;
    Material *material;
};

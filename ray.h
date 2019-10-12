#pragma once
#include "vec3.h"
/*
Class Definition for Ray object
*/
class Ray
{
    public:

		Vec3 origin;
		Vec3 direction;

        Ray() {}
        Ray(const Vec3& o, const Vec3& d) { 
			origin = o; 
			direction = d; 
		}
        Vec3 getOrigin() const       { 
			return origin; 
		}

        Vec3 getDirection() const    { 
			return direction; 
		}

        Vec3 getPoint(float t) const { 
			return origin + t*direction; 
		}

};
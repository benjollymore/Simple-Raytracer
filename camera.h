#pragma once
#include "ray.h"
/*
Class definition for camera object
*/
class Camera {
    public:
		Vec3 origin;
		Vec3 bottomLeft;
		Vec3 horizontal;
		Vec3 vertical;
		Vec3 u, v, w;

        Camera(Vec3 viewPoint, Vec3 viewTarget, Vec3 vup, float vfov, float aspectRatio) {
            
			//Set aspectRatio ratio based on defined field of view
            float theta = vfov*M_PI/180;
            float heightModifier = tan(theta/2);
            float widthModifier = aspectRatio * heightModifier;

			//define uv space
            origin = viewPoint;
            w = normalized(viewPoint - viewTarget);
            u = normalized(cross(vup, w));
            v = cross(w, u);

			//reference vectors
            bottomLeft = origin - widthModifier*u -heightModifier*v - w;
            horizontal = 2*widthModifier*u;
            vertical = 2*heightModifier*v;
        }
        Ray generateRay(float x, float y) {
            return Ray(origin, bottomLeft + x*horizontal + y*vertical - origin);
        }

};


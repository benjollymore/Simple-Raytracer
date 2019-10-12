#pragma once
#include "Log.h"

/*
Class Definition to define sphere object
*/
class Sphere {
    public:
		Vec3 center;
		float radius;
		Material* material;
        Sphere() {}
        Sphere(Vec3 c, float r, Material *m) {
			center = c;
			radius = r;
			material = m;
		}
        virtual bool intersects(const Ray& r, float tmin, float tmax, intersectionLog& log) const;
};

//test ray sphere intersection
bool Sphere::intersects(const Ray& r, float t_min, float t_max, intersectionLog& log) const {
    
	//test for any intersection by finding discriminant
	Vec3 originToCenter = r.getOrigin() - center;
    float a = dot(r.getDirection(), r.getDirection());
    float b = dot(originToCenter, r.getDirection());
    float c = dot(originToCenter, originToCenter) - radius*radius;
    float discriminant = b*b - a*c;

	//if discriminant exists, test both roots to see if they are the nearest point
	//of intersection with the sphere. Start with smaller root first
    if (discriminant > 0) {
        float root = (-b - sqrt(discriminant))/a;
        if (root < t_max && root > t_min) {
            log.t = root;
            log.p = r.getPoint(log.t);
            log.normal = (log.p - center) / radius;
            log.material = material;
            return true;
        }
        root = (-b + sqrt(discriminant)) / a;
        if (root < t_max && root > t_min) {
            log.t = root;
            log.p = r.getPoint(log.t);
            log.normal = (log.p - center) / radius;
            log.material = material;
            return true;
        }
    }
    return false;
}

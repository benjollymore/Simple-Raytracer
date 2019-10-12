#pragma once
#include "ray.h"
#include "Log.h"
/*
Class declaration for abstract Material class, as well
as definitions for three different Material subclasses
that all have different properties when interacting
with cast rays
*/
struct intersectionLog;

//returns pseudorandom double between 0/1
inline double getRandomDouble() {
	return rand() / (RAND_MAX + 1.0);
}

//Schlick approximation for specular reflection
float schlickApproximation(float cosine, float refractiveIndex) {
    float reflection = (1-refractiveIndex) / (1+refractiveIndex);
	reflection *= reflection;
    return reflection + (1- reflection)*pow((1 - cosine),5);
}

/*
Test to see if ray refracts through an object. Ray cannot refract while in the
object out of the two that has a refraction index higher than the other.
Thus discriminant <= 0, and no solution to snells's law.
*/
bool refract(const Vec3& rayDirection, const Vec3& normal, float refractionRatio, Vec3& refracted) {
    Vec3 unitDirection = normalized(rayDirection);
    float incidenceDot = dot(unitDirection, normal);
    float discriminant = 1.0 - refractionRatio * refractionRatio *(1- incidenceDot * incidenceDot);
    if (discriminant > 0) {
        refracted = refractionRatio *(unitDirection - normal*incidenceDot) - normal*sqrt(discriminant);
        return true;
    }
    else
        return false;
}

//helper function. returns mirror reflection
Vec3 reflect(const Vec3& rayDirection, const Vec3& normal) {
     return rayDirection - 2*dot(rayDirection,normal)*normal;
}

//returns random point in a unit sphere. used in an approximation for lambertian shading
Vec3 getRandomPointInSphere() {
    Vec3 point;
    do {
        point = 2.0*Vec3(getRandomDouble(),getRandomDouble(),getRandomDouble()) - Vec3(1,1,1);
    } while (point.squaredNorm() >= 1.0);
    return point;
}

//Abstract material class. Used so spheres can have a generic Material and inherit subclass
class Material  {
    public:
        virtual bool scatter(const Ray& ray, const intersectionLog& log, Vec3& attenuation, Ray& scattered) const = 0;
};

//Matte material that uses an approximation of lambertian shading
class Diffuse : public Material {
    public:
		Vec3 albedo;

		Diffuse(const Vec3& a) {
			albedo = a;
		}
        virtual bool scatter(const Ray& ray, const intersectionLog& log, Vec3& attenuation, Ray& scattered) const  {
             Vec3 reflected = log.p + log.normal + getRandomPointInSphere();
             scattered = Ray(log.p, reflected-log.p);
             attenuation = albedo;
			 return true;
        }

};

/*
Metal material that perfectly reflects all rays in a mirror like fashion.
Rays reflect at angle of incidence mirrored across surface norm
*/
class Metal : public Material {
    public:
		Vec3 albedo;

		Metal(const Vec3& a) {
			albedo = a;
		}
        virtual bool scatter(const Ray& ray, const intersectionLog& log, Vec3& attenuation, Ray& scattered) const  {
            Vec3 reflected = reflect(normalized(ray.getDirection()), log.normal);
            scattered = Ray(log.p, reflected);
            attenuation = albedo;
            return (dot(scattered.getDirection(), log.normal) > 0);
        }
};

/*
Glass material
Upon intersection with sphere, ray will either reflect off
surface or refract through sphere. 
If the ray is inside the sphere it will only reflect.
If the ray is intersecting at a steep angle, it may reflect.
*/
class Translucent : public Material {
    public:
		float refractiveIndex;

		Translucent(float r) {
			refractiveIndex = r;
		}
        virtual bool scatter(const Ray& ray, const intersectionLog& log, Vec3& attenuation, Ray& scattered) const  {
			 float refractionRatio;
			 Vec3 negatedNorm; //normal inside sphere
			 Vec3 refracted; //direction of refracted ray
			 float reflectionProbability; //chance of internal reflection
			 float cosine; //cos of angle of incidence

			 //reflect off surface
             Vec3 reflected = reflect(ray.getDirection(), log.normal);
             attenuation = Vec3(1.0, 1.0, 1.0); 
           
			 //depending on whether or not ray intersects sphere at less than 90 degrees from normal,
			 //calculate value for cosine of angle of refraction
             if (dot(ray.getDirection(), log.normal) > 0) {
                  negatedNorm = -1*log.normal;
                  refractionRatio = refractiveIndex;
                  cosine = refractiveIndex * dot(ray.getDirection(), log.normal) / ray.getDirection().norm();
                  cosine = sqrt(1 - refractiveIndex*refractiveIndex*(1-cosine*cosine));
             }
             else {
                  negatedNorm = log.normal;
                  refractionRatio = 1.0 / refractiveIndex;
                  cosine = -dot(ray.getDirection(), log.normal) / ray.getDirection().norm();
             }

			 //test if ray is not inside sphere, if so, use schlick approximation to test for 
			 //perfect internal reflection due to steep angle. if ray is not in sphere, 
			 //test for chance of reflection
             if (refract(ray.getDirection(), negatedNorm, refractionRatio, refracted))
                reflectionProbability = schlickApproximation(cosine, refractiveIndex);
             else //if in sphere can only reflect
                reflectionProbability = 1.0;

			 //randomly choose if ray will wind up reflecting or refracting
             if (getRandomDouble() < reflectionProbability)
                scattered = Ray(log.p, reflected);
             else
                scattered = Ray(log.p, refracted);
             return true;
        }

};

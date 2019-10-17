#include <iostream>
#include "sphere.h"
#include "float.h"
#include "camera.h"
#include "material.h"
#include <limits>
#include <vector>

//single light point. In retrospect could have easily just been a Vec3, not a sphere object.
Sphere light(Vec3(0, 5, 0), 1, new Diffuse(Vec3(1, 1, 1)));
 
/*
Iterate over each sphere in container and test for intersection.
Only keep intersection from sphere nearest to camera
*/
bool iterativeIntersectionTest(Ray& ray, double tMin, double tMax, intersectionLog& log, std::vector<Sphere> objects) {
	intersectionLog tempLog;
	bool intersected = false;
	double nearestSphere = tMax;
	for (int i = 0; i < objects.size(); i++) {
		if (objects[i].intersects(ray, tMin, nearestSphere, tempLog)) {
			intersected = true;
			nearestSphere = tempLog.t;
			log = tempLog;
		}
	}
	return intersected;
};

/*
Determine final colour for pixel based on whether or not each ray intersects
Final colour is a combination of albedo from every intersected object
with a combination of point light source getDirection dotted with surface
normal of sphere at each intersection poit
*/
Vec3 getPixelColour(Ray& ray, std::vector<Sphere> objects, int depth) {
    intersectionLog log;
    if (iterativeIntersectionTest(ray, 0.001, std::numeric_limits<float>::max(), log, objects)) {
        Ray scattered;
        Vec3 attenuation;

		//rays bounce up to 50 times, check that it is not final iteration of ray
        if (depth < 50 && log.material->scatter(ray, log, attenuation, scattered)) {
             return attenuation*getPixelColour(scattered, objects, depth+1) 
				 + Vec3(1, 1, 1) * .25 * dot(normalized(light.center - ray.getPoint(log.t)), normalized(log.normal)); //point light
        }
		//if ray abosrbed, black
        else {
            return Vec3(0,0,0);
        }
    }
	//no intersection, render background as a gradient
    else {
        Vec3 unitDirection = normalized(ray.getDirection());
        float t = 0.5*(unitDirection.y() + 1.0);
        return (1.0-t)*Vec3(1.0, 1.0, 1.0) + t*Vec3(0.5, 0.7, 1.0);
    }
}

//Helper function for debugging vector values
void printVec3(Vec3 v) {
	std::cout << v.x() << " " << v.y() << " " << v.z() << "\n";
}

//helper function to clamp values between 0 and 1
float clamp(float n) {
	return std::max(0.0f, std::min(n, 1.0f));
}

//helper function to adjust gamma and clamp color values, then cast from float [0,1) to int [0,256)
Vec3 processColour(Vec3 colour) {
	return Vec3(
		int(255 * clamp(sqrt(colour.x()))),
		int(255 * clamp(sqrt(colour.y()))),
		int(255 * clamp(sqrt(colour.z()))));
}

int main() {
	//Image defininitions
    int width = 1200;
    int height = 800;
    int samples =50;
    std::cout << "P3\n" << width << " " << height << "\n255\n";

	//Initialize some spheres
	std::vector<Sphere> sphereContainer;
	sphereContainer.push_back(Sphere(Vec3(0, -1000, -1), 1000, new Diffuse(Vec3(0.2, 0.2, 0.8))));
	sphereContainer.push_back(Sphere(Vec3(2, 1, 0), 1.0, new Translucent(1.5)));
	sphereContainer.push_back(Sphere(Vec3(0, 1, -1.5), 1.0, new Diffuse(Vec3(1, 0, 0))));
	sphereContainer.push_back(Sphere(Vec3(0, 1, 1.5), 1.0, new Metal(Vec3(0.5, 0.5, 0.5))));
	sphereContainer.push_back(Sphere(Vec3(4, .25, 2), .25, new Translucent(1.5)));
	sphereContainer.push_back(Sphere(Vec3(4, .25, 0), .25, new Metal(Vec3(0.5, 0.5, 0.5))));
	sphereContainer.push_back(Sphere(Vec3(4, .25, 1), .25, new  Diffuse(Vec3(1, 0, .8))));
	sphereContainer.push_back(Sphere(Vec3(-3, 1, 0), 2, new  Diffuse(Vec3(1, .4, .8))));

	//Camera initialzation, current viewpoint is arbitrary, can be changed
    Vec3 viewPoint(5,5,5);  //15 2 2 //5 5 5 
    Vec3 viewTarget(1,1,1); //0 0 0 //1 1 1
    Camera camera(viewPoint, viewTarget, Vec3(0,1,0), 15, float(width)/float(height));

	//Cast rays for every pixel
    for (int j = height-1; j >= 0; j--) {
        for (int i = 0; i < width; i++) {

			//For each pixel, take anti aliasing passes
            Vec3 col(0, 0, 0);
            for (int s=0; s < samples; s++) {
                float u = float(i + getRandomDouble()) / float(width);
                float v = float(j + getRandomDouble()) / float(height);
                Ray newRay = camera.generateRay(u, v);
                col += getPixelColour(newRay, sphereContainer, 0);
            }

			//process and output colour
            col /= float(samples);
            col = processColour(col);
			printVec3(col);
        }
    }
}

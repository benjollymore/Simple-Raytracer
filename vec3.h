#pragma once
#include <math.h>
#include <stdlib.h>
#include <iostream>
/*
Implementation of a Vec3 Class.
Heavily based upon Eigen library's base
functionality without additonal overhead
With elements drawn from Raytracing in one weekend
*/

class Vec3  {
    public:
        Vec3() {}
        Vec3(float x, float y, float z) { 
			contents[0] = x; 
			contents[1] = y; 
			contents[2] = z; 
		}

        inline float x() const { 
			return contents[0]; 
		}
        inline float y() const { 
			return contents[1]; 
		}
        inline float z() const { 
			return contents[2]; 
		}

        inline const Vec3& operator+() const { 
			return *this; 
		}
        inline Vec3 operator-() const { 
			return Vec3(-contents[0], -contents[1], -contents[2]); 
		}
        inline float operator[](int i) const { 
			return contents[i]; 
		}
        inline float& operator[](int i) { 
			return contents[i]; 
		}

        inline Vec3& operator+=(const Vec3 &v2);
        inline Vec3& operator-=(const Vec3 &v2);
        inline Vec3& operator*=(const Vec3 &v2);
        inline Vec3& operator/=(const Vec3 &v2);
        inline Vec3& operator*=(const float t);
        inline Vec3& operator/=(const float t);

        inline float norm() const { 
			return sqrt(contents[0]*contents[0] + contents[1]*contents[1] + contents[2]*contents[2]); 
		}
        inline float squaredNorm() const { 
			return contents[0]*contents[0] + contents[1]*contents[1] + contents[2]*contents[2]; 
		}
        inline void normalize();

        float contents[3];
};


inline std::istream& operator>>(std::istream &is, Vec3 &t) {
    is >> t.contents[0] >> t.contents[1] >> t.contents[2];
    return is;
}

inline std::ostream& operator<<(std::ostream &os, const Vec3 &t) {
    os << t.contents[0] << " " << t.contents[1] << " " << t.contents[2];
    return os;
}

inline void Vec3::normalize() {
    float k = 1.0 / sqrt(contents[0]*contents[0] + contents[1]*contents[1] + contents[2]*contents[2]);
    contents[0] *= k; contents[1] *= k; contents[2] *= k;
}

inline Vec3 operator+(const Vec3 &v1, const Vec3 &v2) {
    return Vec3(v1.contents[0] + v2.contents[0], v1.contents[1] + v2.contents[1], v1.contents[2] + v2.contents[2]);
}

inline Vec3 operator-(const Vec3 &v1, const Vec3 &v2) {
    return Vec3(v1.contents[0] - v2.contents[0], v1.contents[1] - v2.contents[1], v1.contents[2] - v2.contents[2]);
}

inline Vec3 operator*(const Vec3 &v1, const Vec3 &v2) {
    return Vec3(v1.contents[0] * v2.contents[0], v1.contents[1] * v2.contents[1], v1.contents[2] * v2.contents[2]);
}

inline Vec3 operator/(const Vec3 &v1, const Vec3 &v2) {
    return Vec3(v1.contents[0] / v2.contents[0], v1.contents[1] / v2.contents[1], v1.contents[2] / v2.contents[2]);
}

inline Vec3 operator*(float t, const Vec3 &v) {
    return Vec3(t*v.contents[0], t*v.contents[1], t*v.contents[2]);
}

inline Vec3 operator/(Vec3 v, float t) {
    return Vec3(v.contents[0]/t, v.contents[1]/t, v.contents[2]/t);
}

inline Vec3 operator*(const Vec3 &v, float t) {
    return Vec3(t*v.contents[0], t*v.contents[1], t*v.contents[2]);
}

inline float dot(const Vec3 &v1, const Vec3 &v2) {
    return v1.contents[0] * v2.contents[0]
         + v1.contents[1] * v2.contents[1]
         + v1.contents[2] * v2.contents[2];
}

inline Vec3 cross(const Vec3 &v1, const Vec3 &v2) {
    return Vec3(v1.contents[1] * v2.contents[2] - v1.contents[2] * v2.contents[1],
                v1.contents[2] * v2.contents[0] - v1.contents[0] * v2.contents[2],
                v1.contents[0] * v2.contents[1] - v1.contents[1] * v2.contents[0]);
}

inline Vec3& Vec3::operator+=(const Vec3 &v){
    contents[0] += v.contents[0];
    contents[1] += v.contents[1];
    contents[2] += v.contents[2];
    return *this;
}

inline Vec3& Vec3::operator*=(const Vec3 &v){
    contents[0] *= v.contents[0];
    contents[1] *= v.contents[1];
    contents[2] *= v.contents[2];
    return *this;
}

inline Vec3& Vec3::operator/=(const Vec3 &v){
    contents[0] /= v.contents[0];
    contents[1] /= v.contents[1];
    contents[2] /= v.contents[2];
    return *this;
}

inline Vec3& Vec3::operator-=(const Vec3& v) {
    contents[0] -= v.contents[0];
    contents[1] -= v.contents[1];
    contents[2] -= v.contents[2];
    return *this;
}

inline Vec3& Vec3::operator*=(const float t) {
    contents[0] *= t;
    contents[1] *= t;
    contents[2] *= t;
    return *this;
}

inline Vec3& Vec3::operator/=(const float t) {
    float k = 1.0f/t;

    contents[0] *= k;
    contents[1] *= k;
    contents[2] *= k;
    return *this;
}

inline Vec3 normalized(Vec3 v) {
    return v / v.norm();
}

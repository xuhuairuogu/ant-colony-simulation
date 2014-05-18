//// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
//// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
//// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
//// PARTICULAR PURPOSE.
////
//// Copyright (c) Microsoft Corporation. All rights reserved
////
//// http://code.msdn.microsoft.com/windowsapps/Direct3D-sprite-sample-97ae6262/sourcecode?fileId=50996&pathId=621366179
////
//// Modified by Aleksejs Belezjaks for Ant Colony Simulation, May 2014

#pragma once

#define _USE_MATH_DEFINES
#include <math.h>

#include <algorithm>  

// Common Constants

#define PI_F 3.1415927f

// Template Vector & Matrix Classes

template <class T> struct Vector2
{
    union
    {
        struct
        {
            T x;
            T y;
        };
        struct
        {
            T r;
            T g;
        };
        struct
        {
            T u;
            T v;
        };
    };

    T& operator[](unsigned int index)
    {
        return static_cast<T*>(this)[index];
    }

    Vector2(T _x = 0, T _y = 0) : x(_x), y(_y) { }
};

template <class T> struct Vector3
{
    union
    {
        struct
        {
            T x;
            T y;
            T z;
        };
        struct
        {
            T r;
            T g;
            T b;
        };
        struct
        {
            T u;
            T v;
            T w;
        };
    };

    T& operator[](unsigned int index)
    {
        return static_cast<T*>(this)[index];
    }

    Vector3(T _x = 0, T _y = 0, T _z = 0) : x(_x), y(_y), z(_z) { }
};

template <class T> struct Vector4
{
    union
    {
        struct
        {
            T x;
            T y;
            T z;
            T w;
        };
        struct
        {
            T r;
            T g;
            T b;
            T a;
        };
    };

    T& operator[](unsigned int index)
    {
        return static_cast<T*>(this)[index];
    }

    Vector4(T _x = 0, T _y = 0, T _z = 0, T _w = 0) : x(_x), y(_y), z(_z), w(_w) { }
};

template <class T> struct Matrix4x4
{
    union
    {
        struct
        {
            T _11; T _12; T _13; T _14;
            T _21; T _22; T _23; T _24;
            T _31; T _32; T _33; T _34;
            T _41; T _42; T _43; T _44;
        };
        struct
        {
            T _m00; T _m01; T _m02; T _m03;
            T _m10; T _m11; T _m12; T _m13;
            T _m20; T _m21; T _m22; T _m23;
            T _m30; T _m31; T _m32; T _m33;
        };
    };

    Matrix4x4(T value = 0)
    {
        _11 = _12 = _13 = _14 = value;
        _21 = _22 = _23 = _24 = value;
        _31 = _32 = _33 = _34 = value;
        _41 = _42 = _43 = _44 = value;
    }

    Matrix4x4(
        T i11, T i12, T i13, T i14,
        T i21, T i22, T i23, T i24,
        T i31, T i32, T i33, T i34,
        T i41, T i42, T i43, T i44
        )
    {
        _11 = i11; _12 = i12; _13 = i13; _14 = i14;
        _21 = i21; _22 = i22; _23 = i23; _24 = i24;
        _31 = i31; _32 = i32; _33 = i33; _34 = i34;
        _41 = i41; _42 = i42; _43 = i43; _44 = i44;
    }

    T* operator[](unsigned int index)
    {
        return &(reinterpret_cast<T*>(this)[index*4]);
    }
};

// Template Vector Operations

template <class T>
T dot(const Vector2<T>& a, const Vector2<T>& b)
{
    return a.x * b.x + a.y * b.y;
}

template <class T>
T dot(const Vector3<T>& a, const Vector3<T>& b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

template <class T>
T dot(const Vector4<T>& a, const Vector4<T>& b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w + b.w;
}

template <class T>
T length(const Vector2<T>& a)
{
    return sqrtf(a.x * a.x + a.y * a.y);
}

template <class T>
T length(const Vector3<T>& a)
{
    return sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
}

template <class T>
T length(const Vector4<T>& a)
{
    return sqrt(a.x * a.x + a.y * a.y + a.z * a.z + a.w * a.w);
}

template <class T>
Vector3<T> cross(const Vector3<T>& a, const Vector3<T>& b)
{
    return Vector3<T>((a.y*b.z)-(a.z*b.y), (a.z*b.x)-(a.x*b.z), (a.x*b.y)-(a.y*b.x));
}

template <class T>
Vector2<T> normalize(const Vector2<T>& a)
{
    T len = length(a);
    return Vector2<T>(a.x / len, a.y / len);
}

template <class T>
Vector3<T> normalize(const Vector3<T>& a)
{
    T len = length(a);
    return Vector3<T>(a.x / len, a.y / len, a.z / len);
}

template <class T>
Vector4<T> normalize(const Vector4<T>& a)
{
    T len = length(a);
    return Vector4<T>(a.x / len, a.y / len, a.z / len, a.w / len);
}

template <class T>
T dis(const Vector2<T>& a, const Vector2<T>& b)
{
	return sqrtf(powf(a.x - b.x, 2.0f) + powf(a.y - b.y, 2.0f));
}

template <class T>
T angle(const Vector2<T>& a)
{
	return atan2f(a.x, a.y);
}

template <class T>
T singned_angle(const Vector2<T>& a, const Vector2<T>& b)
{
	return atan2(b.y,b.x) - atan2(a.y,a.x);
}

template <class T>
T angle(const Vector2<T>& a, const Vector2<T>& b)
{
	return acosf(dot(normalize(a), normalize(b)));
}

template <class T>
T angle(const Vector3<T>& a, const Vector3<T>& b)
{
	return acosf(dot(normalize(a), normalize(b)));
}

template <class T>
T RadianToDegree(const T a)
{
	return a * 180.0f / PI_F; //zda
}

template <class T>
T DegreeToRadian(const T a)
{
	return a * PI_F / 180.0f;
}

template <class T>
T mod(T a, T b)
{
	T result = a - (a / b) * b;
	return result;
}


// Template Vector Operators

template <class T>
Vector2<T> operator-(const Vector2<T>& a, const Vector2<T>& b)
{
    return Vector2<T>(a.x - b.x, a.y - b.y);
}

template <class T>
Vector2<T> operator-(const Vector2<T>& a)
{
    return Vector2<T>( -a.x, -a.y);
}

template <class T>
Vector3<T> operator-(const Vector3<T>& a, const Vector3<T>& b)
{
    return Vector3<T>(a.x - b.x, a.y - b.y, a.z - b.z);
}

template <class T>
Vector3<T> operator-(const Vector3<T>& a)
{
    return Vector3<T>( -a.x, -a.y, -a.z);
}

template <class T>
Vector4<T> operator-(const Vector4<T>& a, const Vector4<T>& b)
{
    return Vector4<T>(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
}

template <class T>
Vector4<T> operator-(const Vector4<T>& a)
{
    return Vector4<T>( -a.x, -a.y, -a.z, -a.w);
}

template <class T>
Vector2<T> operator+(const Vector2<T>& a, const Vector2<T>& b)
{
    return Vector2<T>(a.x + b.x, a.y + b.y);
}

template <class T>
Vector3<T> operator+(const Vector3<T>& a, const Vector3<T>& b)
{
    return Vector3<T>(a.x + b.x, a.y + b.y, a.z + b.z);
}

template <class T>
Vector4<T> operator+(const Vector4<T>& a, const Vector4<T>& b)
{
    return Vector4<T>(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
}

template <class T>
Vector2<T> operator*(const Vector2<T>& a, T s)
{
    return Vector2<T>(a.x * s, a.y * s);
}

template <class T>
Vector2<T> operator*(T s, const Vector2<T>& a)
{
    return a * s;
}

template <class T>
Vector2<T> operator*(const Vector2<T>& a, const Vector2<T>& b)
{
    return Vector2<T>(a.x * b.x, a.y * b.y);
}

template <class T>
Vector2<T> operator/(const Vector2<T>& a, T s)
{
    return Vector2<T>(a.x / s, a.y / s);
}

template <class T>
Vector2<T> operator/(const Vector2<T>& a, const Vector2<T>& b)
{
    return Vector2<T>(a.x / b.x, a.y / b.y);
}

template <class T>
Vector3<T> operator*(const Vector3<T>& a, T s)
{
    return Vector3<T>(a.x * s, a.y * s, a.z * s);
}

template <class T>
Vector3<T> operator*(T s, const Vector3<T>& a)
{
    return a * s;
}

template <class T>
Vector3<T> operator*(const Vector3<T>& a, const Vector3<T>& b)
{
    return Vector3<T>(a.x * b.x, a.y * b.y, a.z * b.z);
}

template <class T>
Vector3<T> operator*(const Vector3<T>& a, const Matrix4x4<T>& b)//Group2 Added method
{
    return Vector3<T>((b._11*a.x) + (b._12*a.y) + (b._13*a.z),
						(b._21*a.x) + (b._22*a.y) + (b._23*a.z),
						(b._31*a.x) + (b._32*a.y) + (b._33*a.z));
}

template <class T>
Vector3<T> operator/(const Vector3<T>& a, T s)
{
    return Vector3<T>(a.x / s, a.y / s, a.z / s);
}

template <class T>
Vector4<T> operator*(const Vector4<T>& a, T s)
{
    return Vector4<T>(a.x * s, a.y * s, a.z * s, a.w * s);
}

template <class T>
Vector4<T> operator*(T s, const Vector4<T>& a)
{
    return a * s;
}

template <class T>
Vector4<T> operator*(const Vector4<T>& a, const Vector4<T>& b)
{
    return Vector4<T>(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w);
}

template <class T>
Vector4<T> operator/(const Vector4<T>& a, T s)
{
    return Vector4<T>(a.x / s, a.y / s, a.z / s, a.w / s);
}

template <class T>
Vector2<T>& operator+=(Vector2<T>& a, const Vector2<T>& b)
{
	a.x += b.x;
	a.y += b.y;
	return a;
}

template <class T>
Vector2<T>& operator-=(Vector2<T>& a, const Vector2<T>& b)
{
	a.x -= b.x;
	a.y -= b.y;
	return a;
}

template <class T>
Vector2<T>& operator*=(Vector2<T>& a, const Vector2<T>& b)
{
	a.x *= b.x;
	a.y *= b.y;
	return a;
}

template <class T>
Vector2<T>& operator/=(Vector2<T>& a, const Vector2<T>& b)
{
	a.x /= b.x;
	a.y /= b.y;
	return a;
}

template <class T>
Vector3<T>& operator+=(Vector3<T>& a, const Vector3<T>& b)
{
	a.x += b.x;
	a.y += b.y;
	return a;
}

template <class T>
Vector3<T>& operator-=(Vector3<T>& a, const Vector3<T>& b)
{
	a.x -= b.x;
	a.y -= b.y;
	return a;
}

template <class T>
Vector3<T>& operator*=(Vector3<T>& a, const Vector3<T>& b)
{
	a.x *= b.x;
	a.y *= b.y;
	return a;
}

template <class T>
Vector3<T>& operator/=(Vector3<T>& a, const Vector3<T>& b)
{
	a.x /= b.x;
	a.y /= b.y;
	return a;
}

template <class T>
bool operator==(const Vector2<T>& a, const Vector2<T>& b)
{
	return a.x == b.x && a.y == b.y;
}

template <class T>
bool operator!=(const Vector2<T>& a, const Vector2<T>& b)
{
	return a.x != b.x || a.y != b.y;
}

// Template Matrix Operations

template <class T>
Matrix4x4<T> transpose(const Matrix4x4<T>& m)
{
    return Matrix4x4<T>(
        m._11, m._21, m._31, m._41,
        m_.12, m._22, m._32, m._42,
        m._13, m._23, m._33, m._43,
        m._14, m._24, m._34, m._44
        );
}

template <class T>
Matrix4x4<T> mul(const Matrix4x4<T>& m1, const Matrix4x4<T>& m2)
{
    Matrix4x4<T> mOut;

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            for (int k = 0; k < 4; k++)
            {
                mOut[i][j] += m1[i][k] * m2[k][j];
            }
        }
    }

    return mOut;
}

// Common HLSL-compatible vector typedefs

typedef unsigned int uint;

typedef Vector2<float> float2;
typedef Vector3<float> float3;
typedef Vector4<float> float4;

typedef Vector2<int> int2;

typedef Matrix4x4<float> float4x4;

// Standard Matrix Intializers

inline float4x4 identity()
{
    float4x4 mOut;

    mOut._11 = 1.0f; mOut._12 = 0.0f; mOut._13 = 0.0f; mOut._14 = 0.0f;
    mOut._21 = 0.0f; mOut._22 = 1.0f; mOut._23 = 0.0f; mOut._24 = 0.0f;
    mOut._31 = 0.0f; mOut._32 = 0.0f; mOut._33 = 1.0f; mOut._34 = 0.0f;
    mOut._41 = 0.0f; mOut._42 = 0.0f; mOut._43 = 0.0f; mOut._44 = 1.0f;

    return mOut;
}

inline float4x4 translation(float x, float y, float z)
{
    float4x4 mOut;

    mOut._11 = 1.0f; mOut._12 = 0.0f; mOut._13 = 0.0f; mOut._14 = x;
    mOut._21 = 0.0f; mOut._22 = 1.0f; mOut._23 = 0.0f; mOut._24 = y;
    mOut._31 = 0.0f; mOut._32 = 0.0f; mOut._33 = 1.0f; mOut._34 = z;
    mOut._41 = 0.0f; mOut._42 = 0.0f; mOut._43 = 0.0f; mOut._44 = 1.0f;

    return mOut;
}

inline float4x4 scale(float x, float y, float z)
{
    float4x4 mOut;

    mOut._11 = x;    mOut._12 = 0.0f; mOut._13 = 0.0f; mOut._14 = 0.0f;
    mOut._21 = 0.0f; mOut._22 = y;    mOut._23 = 0.0f; mOut._24 = 0.0f;
    mOut._31 = 0.0f; mOut._32 = 0.0f; mOut._33 = z;    mOut._34 = 0.0f;
    mOut._41 = 0.0f; mOut._42 = 0.0f; mOut._43 = 0.0f; mOut._44 = 1.0f;

    return mOut;
}

inline float4x4 rotationX(float radX)
{
    float sinAngle = sinf(radX);
    float cosAngle = cosf(radX);

    float4x4 mOut;

    mOut._11 = 1.0f; mOut._12 = 0.0f;     mOut._13 = 0.0f;      mOut._14 = 0.0f;
    mOut._21 = 0.0f; mOut._22 = cosAngle; mOut._23 = -sinAngle; mOut._24 = 0.0f;
    mOut._31 = 0.0f; mOut._32 = sinAngle; mOut._33 = cosAngle;  mOut._34 = 0.0f;
    mOut._41 = 0.0f; mOut._42 = 0.0f;     mOut._43 = 0.0f;      mOut._44 = 1.0f;

    return mOut;
}

inline float4x4 rotationY(float radY)
{
    float sinAngle = sinf(radY);
    float cosAngle = cosf(radY);

    float4x4 mOut;

    mOut._11 = cosAngle;  mOut._12 = 0.0f; mOut._13 = sinAngle; mOut._14 = 0.0f;
    mOut._21 = 0.0f;      mOut._22 = 1.0f; mOut._23 = 0.0f;     mOut._24 = 0.0f;
    mOut._31 = -sinAngle; mOut._32 = 0.0f; mOut._33 = cosAngle; mOut._34 = 0.0f;
    mOut._41 = 0.0f;      mOut._42 = 0.0f; mOut._43 = 0.0f;     mOut._44 = 1.0f;

    return mOut;
}

inline float4x4 rotationZ(float radZ)
{
    float sinAngle = sinf(radZ);
    float cosAngle = cosf(radZ);

    float4x4 mOut;

    mOut._11 = cosAngle; mOut._12 = -sinAngle; mOut._13 = 0.0f; mOut._14 = 0.0f;
    mOut._21 = sinAngle; mOut._22 = cosAngle;  mOut._23 = 0.0f; mOut._24 = 0.0f;
    mOut._31 = 0.0f;     mOut._32 = 0.0f;      mOut._33 = 1.0f; mOut._34 = 0.0f;
    mOut._41 = 0.0f;     mOut._42 = 0.0f;      mOut._43 = 0.0f; mOut._44 = 1.0f;

    return mOut;
}

// 3D Rotation matrix for an arbitrary axis specified by x, y and z
inline float4x4 rotationArbitrary(const float3& axis_in, float degree)
{
    float3 axis = normalize(axis);

    float angleInRadians = degree * (PI_F / 180.0f);

    float sinAngle = sinf(angleInRadians);
    float cosAngle = cosf(angleInRadians);
    float oneMinusCosAngle = 1 - cosAngle;

    float4x4 mOut;

    mOut._11 = 1.0f + oneMinusCosAngle * (axis.x * axis.x - 1.0f);
    mOut._12 = axis.z * sinAngle + oneMinusCosAngle * axis.x * axis.y;
    mOut._13 = -axis.y * sinAngle + oneMinusCosAngle * axis.x * axis.z;
    mOut._41 = 0.0f;

    mOut._21 = -axis.z * sinAngle + oneMinusCosAngle * axis.y * axis.x;
    mOut._22 = 1.0f + oneMinusCosAngle * (axis.y * axis.y - 1.0f);
    mOut._23 = axis.x * sinAngle + oneMinusCosAngle * axis.y * axis.z;
    mOut._24 = 0.0f;

    mOut._31 = axis.y * sinAngle + oneMinusCosAngle * axis.z * axis.x;
    mOut._32 = -axis.x * sinAngle + oneMinusCosAngle * axis.z * axis.y;
    mOut._33 = 1.0f + oneMinusCosAngle * (axis.z * axis.z - 1.0f);
    mOut._34 = 0.0f;

    mOut._41 = 0.0f;
    mOut._42 = 0.0f;
    mOut._43 = 0.0f;
    mOut._44 = 1.0f;

    return mOut;
}

// Two line segment collision check
// Code taken from tutorial at url
// http://www.geeksforgeeks.org/check-if-two-given-line-segments-intersect/

// Given three colinear points p, q, r, the function checks if
// point q lies on line segment 'pr'
template <class T>
bool onSegment(Vector2<T>& p, Vector2<T>& q, Vector2<T>& r)
{
    if (q.x <= std::max(p.x, r.x) && q.x >= std::min(p.x, r.x) &&
       q.y <= std::max(p.y, r.y) && q.y >= std::min(p.y, r.y))
		return true;
 
    return false;
}
 
// To find orientation of ordered triplet (p, q, r).
// The function returns following values
// 0 --> p, q and r are colinear
// 1 --> Clockwise
// 2 --> Counterclockwise
template <class T>
int orientation(Vector2<T>& p, Vector2<T>& q, Vector2<T>& r)
{
    // See 10th slides from following link for derivation of the formula
    // http://www.dcs.gla.ac.uk/~pat/52233/slides/Geometry1x1.pdf
	TYPE_FLOAT32 val = (q.y - p.y) * (r.x - q.x) -
              (q.x - p.x) * (r.y - q.y);
 
    if (val == 0.0f) return 0;  // colinear
 
    return (val > 0)? 1: 2; // clock or counterclock wise
}
 
// The main function that returns true if line segment 'p1q1'
// and 'p2q2' intersect.
template <class T>
bool doIntersect(Vector2<T>& p1, Vector2<T>& q1, Vector2<T>& p2, Vector2<T>& q2)
{
    // Find the four orientations needed for general and
    // special cases
    TYPE_INT32 o1 = orientation(p1, q1, p2);
    TYPE_INT32 o2 = orientation(p1, q1, q2);
    TYPE_INT32 o3 = orientation(p2, q2, p1);
    TYPE_INT32 o4 = orientation(p2, q2, q1);
 
    // General case
    if (o1 != o2 && o3 != o4)
        return true;
 
    // Special Cases
    // p1, q1 and p2 are colinear and p2 lies on segment p1q1
    if (o1 == 0 && onSegment(p1, p2, q1)) return true;
 
    // p1, q1 and p2 are colinear and q2 lies on segment p1q1
    if (o2 == 0 && onSegment(p1, q2, q1)) return true;
 
    // p2, q2 and p1 are colinear and p1 lies on segment p2q2
    if (o3 == 0 && onSegment(p2, p1, q2)) return true;
 
     // p2, q2 and q1 are colinear and q1 lies on segment p2q2
    if (o4 == 0 && onSegment(p2, q1, q2)) return true;
 
    return false; // Doesn't fall in any of the above cases
}

template <class T>
bool lineVScircle2(Vector2<T>& A0, Vector2<T>& B0, Vector2<T>& C, T &radius)
{
	//Formula from http://mathworld.wolfram.com/Circle-LineIntersection.html 

	float2 A = (A0 - C);//set relative to (0,0)
	float2 B = (B0 - C);

	T  dx = (B.x-A.x);//change in x
	T dy = (B.y-A.y);//change in y
	T D = (A.x*B.y)-(B.x*A.y);
	T Dr2 = (dx*dx) + (dy*dy);

	//discriminat = (r^2 * (dx^2 + dy^2)) - D^2
	T discr = ((radius*radius) * Dr2) - (D*D);

	if(discr < 0)
	{
		return false;
	}
	else//find roots
	{
		//if(dy < 0) sign = -1
		//else sign = 1;

		//x = [D * dy +- sign*dx*sqrt(discr)] / [Dr2] 
		//y = -D * dx +- positive(dy)*sqrt(discr) / [Dr2]

		TYPE_INT32 sign;
		if(dy < 0) { sign = -1;} else { sign = 1; }
		T dyPosi = dy;
		if(dyPosi < 0) {dyPosi = -dyPosi;}


		float2 iPosi;//find intersect 1
		iPosi.x = (D*dy + sign*dx*sqrtf(discr)) / Dr2;
		iPosi.y = (D*dx + dyPosi*sqrtf(discr)) / Dr2;

		float2 iNeg;//find intersect 2
		iNeg.x = (D*dy - sign*dx*sqrtf(discr)) / Dr2;
		iNeg.y = (D*dx - dyPosi*sqrtf(discr)) / Dr2;


		T iPosiLength = length((iPosi) - A);//dist from start to interect
		T iNegLength = length((iNeg) - A);

		T LengthAB = length(B-A);

		if(iPosiLength < radius)
		{
			return true;
		}

		if(iNegLength < radius)
		{
			return true;
		}

		return false;
		
	}

}

// Helper methods
inline int rnd(int min, int max)
{
	return min + (rand() % (int)(max - min + 1));
}

inline float rnd(float float_max)
{
	return static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/float_max));
}


inline float2 int2float2(int2 in)
{
	return float2(static_cast<float>(in.x), static_cast<float>(in.y));
}

inline float ctof(char* c)
{
	return static_cast<float>(atof(c));
}

inline uint ctoui(char* c)
{
	int t = atoi(c);
	if (t < 0) return 0;
	return static_cast<uint>(t);
}
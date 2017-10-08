#pragma once
#include "types.h"

struct float4
{
	float x = 0;
	float y = 0;
	float z = 0;
	float w = 0;

	float4() {}
	float4(float xx, float yy, float zz, float ww) : x(xx), y(yy), z(zz), w(ww) { }

	float normal() { return sqrt(x*x + y*y + z*z + w*w); }
	float4& normalize(float l = 1) { *this = (*this)*(l / normal()); return *this; }

	float2 xy() { return float2(x, y); }
	float r() { return x; }
	float g() { return y; }
	float b() { return z; }
	float a() { return w; }

	float4 operator+(const float4& v)
	{
		return float4(x + v.x, y + v.y, z + v.z, w + v.w);
	}
	float4 operator-(const float4& v)
	{
		return float4(x - v.x, y - v.y, z - v.z, w - v.w);
	}
	float4 operator*(float v)
	{
		return float4(x*v, y*v, z*v, w*v);
	}
	float4 operator/(float  v)
	{
		return float4(x / v, y / v, z / v, w / v);
	}

	float dot(float4 b)
	{
		float4& a = *this;
		return a.x*b.x + a.y*b.y + a.z*b.z + a.w*b.w;
	}
};
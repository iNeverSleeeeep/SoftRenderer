#pragma once
struct float3
{
	float x = 0;
	float y = 0;
	float z = 0;
	float3 xy()
	{
		return *this;
	}

	float3() {}
	float3(float xx, float yy, float zz) : x(xx), y(yy), z(zz) { }

	float normal() { return sqrt(x*x + y*y + z*z); }
	float3& normalize(float l = 1) { *this = (*this)*(l / normal()); return *this; }

	float3 operator+(const float3& v)
	{
		return float3(x + v.x, y + v.y, z + v.z);
	}
	float3 operator-(const float3& v)
	{
		return float3(x - v.x, y - v.y, z - v.z);
	}
	float3 operator*(float v)
	{
		return float3(x * v, y * v, z*v);
	}
	float3 operator/(float  v)
	{
		return float3(x / v, y / v, z / v);
	}
};
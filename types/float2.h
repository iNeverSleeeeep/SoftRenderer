#pragma once
struct float2
{
	float x = 0;
	float y = 0;
	float2 xy()
	{
		return *this;
	}

	float2() {}
	float2(float xx, float yy) : x(xx), y(yy) { }

	float2 operator+(const float2& v)
	{
		return float2(x + v.x, y + v.y);
	}
	float2 operator-(const float2& v)
	{
		return float2(x - v.x, y - v.y);
	}
	float2 operator*(float v)
	{
		return float2(x * v, y * v);
	}
	float2 operator/(float  v)
	{
		return float2(x / v, y / v);
	}
};
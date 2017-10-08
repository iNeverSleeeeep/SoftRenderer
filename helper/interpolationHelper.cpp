
#include "interpolationHelper.h"

using namespace std;

void Interpolation0(float2 p1, float2 p2, function<void(float2)> handler)
{
	int x0 = p1.x;
	int y0 = p1.y;
	int x1 = p2.x;
	int y1 = p2.y;
	bool steep = false;
	// 保证高度小于宽度
	if (std::abs(x0 - x1) < std::abs(y0 - y1))
	{
		std::swap(x0, y0);
		std::swap(x1, y1);
		steep = true;
	}
	// 保证从左向右画
	if (x0 > x1)
	{ 
		std::swap(x0, x1);
		std::swap(y0, y1);
	}
	for (int x = x0; x <= x1; x++) 
	{
		float t = (x - x0) / (float)(x1 - x0);
		int y = y0*(1.0 - t) + y1*t;
		if (steep) handler(float2(y, x));
		else handler(float2(x, y));
	}
}

void Interpolation(float2 p1, float2 p2, function<void(float2)> handler)
{
	int x0 = p1.x;
	int y0 = p1.y;
	int x1 = p2.x;
	int y1 = p2.y;
	bool steep = false;
	if (std::abs(x0 - x1)<std::abs(y0 - y1)) 
	{
		std::swap(x0, y0);
		std::swap(x1, y1);
		steep = true;
	}
	if (x0>x1) 
	{
		std::swap(x0, x1);
		std::swap(y0, y1);
	}
	int dx = x1 - x0;
	int dy = y1 - y0;
	int derror2 = std::abs(dy) * 2;
	int error2 = 0;
	int y = y0;
	for (int x = x0; x <= x1; x++) 
	{
		if (steep) 
			handler(float2(y, x));
		else
			handler(float2(x, y));
		
		error2 += derror2;
		if (error2 > dx) 
		{
			y += (y1>y0 ? 1 : -1);
			error2 -= dx * 2;
		}
	}
}

float3 barycentric(float2 p0, float2 p1, float2 p2, float2 P) 
{
	float3 v1 = float3(p2.x - p0.x, p1.x - p0.x, p0.x - P.x);
	float3 v2 = float3(p2.y - p0.y, p1.y - p0.y, p0.y - P.y);
	float3 u = v1.cross(v2);
	if (std::abs(u.z)<1) return float3(-1, 1, 1); 
	return float3(1.f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z);
}

void Interpolation(float2 p0, float2 p1, float2 p2, function<void(float2)> handler)
{
	float2 bboxmin(1000, 1000);
	float2 bboxmax(0, 0);
	float2 clamp(1000, 1000);
	bboxmin.x = std::max(0.0f, std::min(bboxmin.x, p0.x));
	bboxmax.x = std::min(clamp.x, std::max(bboxmax.x, p0.x));
	bboxmin.y = std::max(0.0f, std::min(bboxmin.y, p0.y));
	bboxmax.y = std::min(clamp.y, std::max(bboxmax.y, p0.y));
	bboxmin.x = std::max(0.0f, std::min(bboxmin.x, p1.x));
	bboxmax.x = std::min(clamp.x, std::max(bboxmax.x, p1.x));
	bboxmin.y = std::max(0.0f, std::min(bboxmin.y, p1.y));
	bboxmax.y = std::min(clamp.y, std::max(bboxmax.y, p1.y));
	bboxmin.x = std::max(0.0f, std::min(bboxmin.x, p2.x));
	bboxmax.x = std::min(clamp.x, std::max(bboxmax.x, p2.x));
	bboxmin.y = std::max(0.0f, std::min(bboxmin.y, p2.y));
	bboxmax.y = std::min(clamp.y, std::max(bboxmax.y, p2.y));

	float2 P;
	for (P.x = bboxmin.x; P.x <= bboxmax.x; P.x++) 
	{
		for (P.y = bboxmin.y; P.y <= bboxmax.y; P.y++) 
		{
			float3 bc_screen = barycentric(p0, p1, p2, P);
			if (bc_screen.x<0 || bc_screen.y<0 || bc_screen.z<0) continue;
			handler(float2(P.x, P.y));
		}
	}
}

float3 barycentric(float3 A, float3 B, float3 C, float3 P) 
{
	float3 s[2];
	s[1].x = C.y - A.y;
	s[1].y = B.y - A.y;
	s[1].z = A.y - P.y;
	s[0].x = C.x - A.x;
	s[0].y = B.x - A.x;
	s[0].z = A.x - P.x;

	float3 u = s[0].cross(s[1]);
	if (std::abs(u.z)>1e-2) // dont forget that u[2] is integer. If it is zero then triangle ABC is degenerate
		return float3(1.f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z);
	return float3(-1, 1, 1); // in this case generate negative coordinates, it will be thrown away by the rasterizator
}


void Interpolation(float3 p0, float3 p1, float3 p2, function<void(float3)> handler)
{
	float2 bboxmin(1000, 1000);
	float2 bboxmax(0, 0);
	float2 clamp(1000, 1000);
	bboxmin.x = std::max(0.0f, std::min(bboxmin.x, p0.x));
	bboxmax.x = std::min(clamp.x, std::max(bboxmax.x, p0.x));
	bboxmin.y = std::max(0.0f, std::min(bboxmin.y, p0.y));
	bboxmax.y = std::min(clamp.y, std::max(bboxmax.y, p0.y));
	bboxmin.x = std::max(0.0f, std::min(bboxmin.x, p1.x));
	bboxmax.x = std::min(clamp.x, std::max(bboxmax.x, p1.x));
	bboxmin.y = std::max(0.0f, std::min(bboxmin.y, p1.y));
	bboxmax.y = std::min(clamp.y, std::max(bboxmax.y, p1.y));
	bboxmin.x = std::max(0.0f, std::min(bboxmin.x, p2.x));
	bboxmax.x = std::min(clamp.x, std::max(bboxmax.x, p2.x));
	bboxmin.y = std::max(0.0f, std::min(bboxmin.y, p2.y));
	bboxmax.y = std::min(clamp.y, std::max(bboxmax.y, p2.y));

	float3 P;
	for (P.x = bboxmin.x; P.x <= bboxmax.x; P.x++) 
	{
		for (P.y = bboxmin.y; P.y <= bboxmax.y; P.y++) 
		{
			float3 bc_screen = barycentric(p0, p1, p2, P);
			if (bc_screen.x<0 || bc_screen.y<0 || bc_screen.z<0) continue;
			P.z = p0.z * bc_screen.x + p1.z * bc_screen.y + p2.z * bc_screen.z;
			handler(P);
		}
	}
}

void Interpolation(v2f v0, v2f v1, v2f v2, function<void(v2f)> handler)
{
	float3 p0 = v0.position;
	float3 p1 = v1.position;
	float3 p2 = v2.position;
	float2 bboxmin(1000, 1000);
	float2 bboxmax(0, 0);
	float2 clamp(1000, 1000);
	bboxmin.x = std::max(0.0f, std::min(bboxmin.x, p0.x));
	bboxmax.x = std::min(clamp.x, std::max(bboxmax.x, p0.x));
	bboxmin.y = std::max(0.0f, std::min(bboxmin.y, p0.y));
	bboxmax.y = std::min(clamp.y, std::max(bboxmax.y, p0.y));
	bboxmin.x = std::max(0.0f, std::min(bboxmin.x, p1.x));
	bboxmax.x = std::min(clamp.x, std::max(bboxmax.x, p1.x));
	bboxmin.y = std::max(0.0f, std::min(bboxmin.y, p1.y));
	bboxmax.y = std::min(clamp.y, std::max(bboxmax.y, p1.y));
	bboxmin.x = std::max(0.0f, std::min(bboxmin.x, p2.x));
	bboxmax.x = std::min(clamp.x, std::max(bboxmax.x, p2.x));
	bboxmin.y = std::max(0.0f, std::min(bboxmin.y, p2.y));
	bboxmax.y = std::min(clamp.y, std::max(bboxmax.y, p2.y));

	float3 P;
	for (P.x = bboxmin.x; P.x <= bboxmax.x; P.x++) 
	{
		for (P.y = bboxmin.y; P.y <= bboxmax.y; P.y++) 
		{
			float3 bc_screen = barycentric(p0, p1, p2, P);
			if (bc_screen.x<0 || bc_screen.y<0 || bc_screen.z<0) continue;
			P.z = p0.z * bc_screen.x + p1.z * bc_screen.y + p2.z * bc_screen.z;
			v2f V;
			V.position = P;
			V.normal = v0.normal * bc_screen.x + v1.normal * bc_screen.y + v2.normal * bc_screen.z;
			V.uv = v0.uv * bc_screen.x + v1.uv * bc_screen.y + v2.uv * bc_screen.z;
			V.texcoord2 = v0.texcoord2 * bc_screen.x + v1.texcoord2 * bc_screen.y + v2.texcoord2 * bc_screen.z;
			V.texcoord3 = v0.texcoord3 * bc_screen.x + v1.texcoord3 * bc_screen.y + v2.texcoord3 * bc_screen.z;
			V.texcoord4 = v0.texcoord4 * bc_screen.x + v1.texcoord4 * bc_screen.y + v2.texcoord4 * bc_screen.z;
			V.texcoord5 = v0.texcoord5 * bc_screen.x + v1.texcoord5 * bc_screen.y + v2.texcoord5 * bc_screen.z;
			V.texcoord6 = v0.texcoord6 * bc_screen.x + v1.texcoord6 * bc_screen.y + v2.texcoord6 * bc_screen.z;
			handler(V);
		}
	}
}
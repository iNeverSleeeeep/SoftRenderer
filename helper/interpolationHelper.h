#pragma once
#include <functional>
#include <cmath>
#include <algorithm>
#include "../types/types.h"

using namespace std;

void Interpolation0(float2 p1, float2 p2, function<void(float2)> handler);

void Interpolation(float2 p1, float2 p2, function<void(float2)> handler);

float3 barycentric(float2 p0, float2 p1, float2 p2, float2 P);

void Interpolation(float2 p0, float2 p1, float2 p2, function<void(float2)> handler);

float3 barycentric(float3 A, float3 B, float3 C, float3 P);

void Interpolation(float3 p0, float3 p1, float3 p2, function<void(float3)> handler);

void Interpolation(v2f v0, v2f v1, v2f v2, function<void(v2f)> handler);
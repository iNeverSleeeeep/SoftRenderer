#pragma once
#include "types.h"
#include "../3rdParty/tgaimage.h"

struct vertex
{
	float3 position;
	TGAColor color;
	float2 uv;
	float3 normal;
	float4 tangent;
	vertex() {};
};
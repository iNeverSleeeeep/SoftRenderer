#pragma once

#include "float2.h"
#include "float3.h"
#include "float4.h"
#include "vertex.h"

#include "../3rdParty/tgaimage.h"

struct appdata
{
	float3 vertex;
	float2 uv;
	float3 normal;
};

struct v2f
{
	float3 position;
	float2 uv;
	float3 normal;

	float4 texcoord2;
	float4 texcoord3;
	float4 texcoord4;
	float4 texcoord5;
	float4 texcoord6;
};
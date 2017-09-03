#pragma once
#include <functional>
#include <cmath>
#include <algorithm>
#include "types/types.h"

using namespace std;

void Interpolation(float2 p1, float2 p2, function<void(float2)> handler)
{
	bool flip = false;
	if (p2.x == p1.x)
	{
		if (p2.y == p1.y) 
		{
			handler(float2(int(p1.x), int(p1.y)));
			return;
		}
		swap(p1.x, p1.y);
		swap(p2.x, p2.y);
		flip = true;
	}
	if (p1.x > p2.x)
	{
		swap(p1, p2);
	}

	bool directionUp = p2.y > p1.y;

	float slope = (p2.y - p1.y) / (p2.x - p1.x);
	int startX = int(p1.x);
	int endX = int(p2.x);
	int startY = round(p1.y - slope * (p1.x - startX));
	startY = max(min(startY, int(p1.y) + 1), int(p1.y));

	int lastY = startY;
	if (flip == false)
	{
		if (directionUp)
		{
			for (int i = 0; i <= endX - startX; ++i)
			{
				int tempEndY = round(startY + i*slope);
				if (endX - startX == i) tempEndY = round(p2.y);
				tempEndY = min(tempEndY, int(p2.y) + 1);
				for (int y = lastY; y <= tempEndY; ++y)
					handler(float2(startX + i, y));
				lastY = tempEndY;
			}
		}
		else
		{
			for (int i = 0; i <= endX - startX; ++i)
			{
				int tempEndY = round(startY + i*slope);
				if (endX - startX == i) tempEndY = round(p2.y);
				tempEndY = max(tempEndY, int(p2.y) - 1);
				for (int y = lastY; y >= tempEndY; --y)
					handler(float2(startX + i, y));
				lastY = tempEndY;
			}
		}
	}
	else
	{
		if (directionUp)
		{
			for (int i = 0; i <= endX - startX; ++i)
			{
				int tempEndY = round(startY + i * slope);
				if (endX - startX == i) tempEndY = round(p2.y);
				tempEndY = min(tempEndY, int(p2.y) + 1);
				for (int y = lastY; y >= tempEndY; --y)
					handler(float2(y, startX + i));
				lastY = tempEndY;
			}
		}
		else
		{
			for (int i = 0; i <= endX - startX; ++i)
			{
				int tempEndY = round(startY + i * slope);
				if (endX - startX == i) tempEndY = round(p2.y);
				tempEndY = max(tempEndY, int(p2.y) - 1);
				for (int y = lastY; y <= tempEndY; ++y)
					handler(float2(y, startX + i));
				lastY = tempEndY;
			}
		}
	}
}

void Interpolation(float2 p1, float2 p2, float2 p3, function<void(float2)> handler)
{
	if (p2.y > p3.y) swap(p2, p3);
	if (p1.y > p2.y) swap(p1, p2);
	if (p2.y > p3.y) swap(p2, p3);

	if (p1.y < p2.y)
	{
		float slope13 = (p3.y - p1.y) / (p3.x - p1.x);
		float slope13p = 1 / slope13;

		float minStartX = min(int(p1.x), int(p1.x) + 1);
		float maxStartX = max(int(p1.x), int(p1.x) + 1);
		float minEndX = min(int(p3.x), int(p3.x) + 1);
		float maxEndX = max(int(p3.x), int(p3.x) + 1);
		
		float minX = min(minStartX, minEndX);
		float maxX = max(maxStartX, maxEndX);
		
		int index = 0;
		Interpolation(p1, p2, [&](float2 p) {
			float x = round((p.y - p1.y)*slope13p + p1.x);
			Interpolation(p, { x, p.y }, handler);
		});

		Interpolation(p2, p3, [&](float2 p) {
			float x = round((p.y - p1.y)*slope13p + p1.x);
			Interpolation(p, { x, p.y }, handler);
		});
	}
}
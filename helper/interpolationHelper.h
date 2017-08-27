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
		swap(p1.x, p1.y);
		swap(p2.x, p2.y);
		flip = true;
	}
	if (p1.x > p2.x)
	{
		swap(p1.x, p2.x);
		swap(p1.y, p2.y);
	}

	bool directionUp = p2.y > p1.y;

	float slope = (p2.y - p1.y) / (p2.x - p1.x);
	int startX = int(p1.x);
	int endX = int(p2.x)+1;
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
				tempEndY = min(tempEndY, int(p2.y) + 1);
				int tempHalfY = lastY + (tempEndY - lastY) / 2;
				for (int y = lastY + 1; y <= tempHalfY; ++y)
					handler(float2(startX + i - 1, y));
				for (int y = tempHalfY; y <= tempEndY; ++y)
					handler(float2(startX + i, y));
				lastY = tempEndY;
			}
		}
		else
		{
			for (int i = 0; i <= endX - startX; ++i)
			{
				int tempEndY = round(startY + i*slope);
				tempEndY = max(tempEndY, int(p2.y) - 1);
				int tempHalfY = lastY + (tempEndY - lastY) / 2;
				for (int y = lastY - 1; y >= tempHalfY; --y)
					handler(float2(startX + i - 1, y));
				for (int y = tempHalfY; y >= tempEndY; --y)
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
				tempEndY = min(tempEndY, int(p2.y) + 1);
				int tempHalfY = lastY + (tempEndY - lastY) / 2;
				for (int y = lastY - 1; y > tempHalfY; --y)
					handler(float2(y, startX + i - 1));
				for (int y = tempHalfY; y >= tempEndY; --y)
					handler(float2(y, startX + i));
				lastY = tempEndY;
			}
		}
		else
		{
			for (int i = 0; i <= endX - startX; ++i)
			{
				int tempEndY = round(startY + i * slope);
				tempEndY = max(tempEndY, int(p2.y) - 1);
				int tempHalfY = lastY + (tempEndY - lastY) / 2;
				for (int y = lastY + 1; y < tempHalfY; ++y)
					handler(float2(y, startX + i - 1));
				for (int y = tempHalfY; y <= tempEndY; ++y)
					handler(float2(y, startX + i));
				lastY = tempEndY;
			}
		}
	}


}
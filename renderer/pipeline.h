#pragma once
#include <vector>
#include <cassert>
#include <functional>

#include "../types/types.h"
#include "../helper/interpolationHelper.h"

class Pipeline
{
private:
	std::function<v2f(appdata)> _vert;
	std::function<float4(v2f)> _frag;
	std::vector<appdata> _vertexes;
	std::vector<int> _indexes; 

	bool _ZWrite = false;
	bool _ZTest = false;

	int _width = 0;
	int _height = 0;

	float* _ZBuffer = nullptr;
	TGAImage* _frame = nullptr;

public:
	Pipeline(int width, int height)
	{
		_width = width;
		_height = height;

		_ZBuffer = new float[width*height];
	}

	~Pipeline()
	{
		delete _ZBuffer;
	}

	void compile(std::function<v2f(appdata)> vert)
	{
		_vert = vert;
	}
	void compile(std::function<float4(v2f)> frag)
	{
		_frag = frag;
	}
	void bindFrame(TGAImage* frame)
	{
		_frame = frame;
	}

	void fill(std::vector<appdata>&& vertexes, std::vector<int>&& indexes)
	{
		assert(indexes.size() % 3 == 0);
		_vertexes = vertexes;
		_indexes = indexes;
	}

	void ZWriteOn() { _ZWrite = true; }
	void ZWriteOff() { _ZWrite = false; }
	void ZTestOn() { _ZTest = true; }
	void ZTestOff() { _ZTest = false; }

	void clear()
	{
		memset(_ZBuffer, -std::numeric_limits<float>::max(), _width*_height);
	}

	void draw()
	{
		clear();
		// 顶点着色器
		std::vector<v2f> v2fs;
		v2fs.reserve(_vertexes.size());
		for (auto i = 0; i < _indexes.size(); ++i)
		{
			v2fs.push_back(_vert(_vertexes[_indexes[i]]));
		}

		// 片元着色器
		for (auto i = 0; i < v2fs.size(); i += 3)
		{
			auto v1 = v2fs[i];
			auto v2 = v2fs[i+1];
			auto v3 = v2fs[i+2];

			Interpolation(v1, v2, v3, [&](v2f v) 
			{
				auto p = v.position;
				auto color = _frag(v);
				if (_ZTest == false || p.z > _ZBuffer[int(p.x)+int(p.y)*_width])
				{
					auto tgaColor = TGAColor(int(color.x * 255), int(color.y * 255), int(color.z * 255), int(color.w * 255));
					_frame->set(p.x, p.y, tgaColor);
					if (_ZWrite)
					{
						_ZBuffer[int(p.x)+int(p.y)*_width] = p.z;
					}
				}
			});
		}
	}
};
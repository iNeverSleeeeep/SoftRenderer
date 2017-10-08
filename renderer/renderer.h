#pragma once
#include <string>
#include <vector>
#include <memory>
#include "pipeline.h"
#include"../types/types.h"

using namespace std;
class Renderer
{
private:
	vector<vertex> _vertexes;
	std::unique_ptr<Pipeline> _pipeline;
	std::unique_ptr<TGAImage> _image;
public:
	Renderer();
	~Renderer();

	void setVertexes(const vector<vertex>& vertexes);
	void setVertAndFrag(std::function<v2f(appdata)> vert, std::function<float4(v2f)> frag);

	void render(std::string path);
};
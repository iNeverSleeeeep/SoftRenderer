#pragma once
#include <string>
#include <vector>
#include"../types/types.h"

using namespace std;
class Renderer
{
private:
	vector<vertex> _vertexes;
public:
	void setVertexes(const vector<vertex> vertexes);

	void render(TGAImage& image);
};
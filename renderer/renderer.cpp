#include "renderer.h"

Renderer::Renderer()
{
	_image.reset(new TGAImage(800, 800, TGAImage::RGB));
	_pipeline.reset(new Pipeline(800, 800));
}

Renderer::~Renderer()
{
	_pipeline.reset();
}

void Renderer::setVertexes(const vector<vertex>& vertexes)
{
	_vertexes = vertexes;
}

void Renderer::setVertAndFrag(std::function<v2f(appdata)> vert, std::function<float4(v2f)> frag)
{
	_pipeline->compile(vert);
	_pipeline->compile(frag);
}

void Renderer::render(std::string path)
{
	_pipeline->bindFrame(_image.get());

	std::vector<appdata> vertexes;
	std::vector<int> indexes;
	vertexes.reserve(_vertexes.size());
	indexes.reserve(_vertexes.size());
	int index = 0;
	for (auto& v : _vertexes)
	{
		appdata data;
		data.vertex = v.position;
		data.uv = v.uv;
		data.normal = v.normal;
		vertexes.push_back(data);
		indexes.push_back(index);
		++index;
	}

	_pipeline->fill(std::move(vertexes), std::move(indexes));

	_pipeline->draw();

	_image->flip_vertically();
	_image->write_tga_file(path.c_str());
}
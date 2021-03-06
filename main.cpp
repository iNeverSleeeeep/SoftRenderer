#include <string>
#include <vector>
#include <iostream>
#include <functional>
#include "3rdParty/model.h"
#include "renderer/renderer.h"
#include "helper/interpolationHelper.h"

using namespace std;

class Lesson
{
public:
	virtual void start() = 0;
};

class Lesson1 : public Lesson
{
	const TGAColor red = TGAColor(255, 0, 0, 255);
	const TGAColor white = TGAColor(255, 255, 255, 255);
public:
	void start() override
	{
		// 红点
		{
			TGAImage image(100, 100, TGAImage::RGB);
			image.set(40, 50, red);
			image.flip_vertically();
			image.write_tga_file("output/lesson1/point.tga");
		}
		{
			TGAImage image(100, 100, TGAImage::RGB);
			Interpolation0({ 13, 20 }, { 80, 40 }, [&](float2 p) { image.set(p.x, p.y, white); });
			Interpolation0({ 20, 13 }, { 40, 80 }, [&](float2 p) { image.set(p.x, p.y, red); });
			Interpolation0({ 80, 40 }, { 13, 20 }, [&](float2 p) { image.set(p.x, p.y, red); });
			image.flip_vertically();
			image.write_tga_file("output/lesson1/temp.tga");
		}

		// 一条红线
		{
			TGAImage image(100, 100, TGAImage::RGB);
			Interpolation({10, 10}, {90, 90}, [&](float2 p) { image.set(p.x, p.y, red); });
			Interpolation({ 20, 10 }, { 20, 90 }, [&](float2 p) { image.set(p.x, p.y, red); });
			Interpolation({ 10, 10 }, { 9, 90 }, [&](float2 p) { image.set(p.x, p.y, red); });
			image.flip_vertically();
			image.write_tga_file("output/lesson1/redline.tga");
		}

		// 模型
		{
			const float width = 1000;
			const float height = 1000;
			Model model("resource/african_head/african_head.obj");
			TGAImage image(width, height, TGAImage::RGB);
			for (int i = 0; i<model.nfaces(); i++) 
			{
				std::vector<int> face = model.face(i);
				for (int j = 0; j<3; j++) 
				{
					float3 v1 = model.vert(face[j]);
					float3 v2 = model.vert(face[(j + 1) % 3]);
					float2 p1((v1.x + 1.0) * width / 2.0, (v1.y + 1.0) * height / 2.0);
					float2 p2((v2.x + 1.0) * width / 2.0, (v2.y + 1.0) * height / 2.0);
					Interpolation(p1, p2, [&](float2 p) 
					{
						image.set(p.x, p.y, white);
					});
				}
			}
			image.flip_vertically();
			image.write_tga_file("output/lesson1/model.tga");
		}
	}
};

class Lesson2 : public Lesson
{
	const TGAColor red = TGAColor(255, 0, 0, 255);
	const TGAColor white = TGAColor(255, 255, 255, 255);
	const TGAColor green = TGAColor(0, 255, 0, 255);

	void triangle(float2 p1, float2 p2, float2 p3, function<void(float2)> handler)
	{
		Interpolation(p1, p2, handler);
		Interpolation(p1, p3, handler);
		Interpolation(p2, p3, handler);
	}
public:
	void start() override
	{
		// 镂空三角形
		{
			float2 t0[3] = { float2(10, 70),   float2(50, 160),  float2(70, 80) };
			float2 t1[3] = { float2(180, 50),  float2(150, 1),   float2(70, 180) };
			float2 t2[3] = { float2(180, 150), float2(120, 160), float2(130, 180) };

			TGAImage image(200, 200, TGAImage::RGB);
			triangle(t0[0], t0[1], t0[2], [&](float2 p) 
			{
				image.set(p.x, p.y, red);
			});
			triangle(t1[0], t1[1], t1[2], [&](float2 p) 
			{
				image.set(p.x, p.y, white);
			});
			triangle(t2[0], t2[1], t2[2], [&](float2 p) 
			{
				image.set(p.x, p.y, green);
			});
			image.flip_vertically();
			image.write_tga_file("output/lesson2/simpleTriangle.tga");
		}

		// 三角形
		{
			float2 t0[3] = { float2(10, 70),   float2(50, 160),  float2(70, 80) };
			float2 t1[3] = { float2(180, 50),  float2(150, 1),   float2(70, 180) };
			float2 t2[3] = { float2(180, 150), float2(120, 160), float2(130, 180) };

			TGAImage image(200, 200, TGAImage::RGB);
			Interpolation(t0[0], t0[1], t0[2], [&](float2 p) 
			{
				image.set(p.x, p.y, red);
			});
			Interpolation(t1[0], t1[1], t1[2], [&](float2 p) 
			{
				image.set(p.x, p.y, white);
			});
			Interpolation(t2[0], t2[1], t2[2], [&](float2 p) 
			{
				image.set(p.x, p.y, green);
			});
			image.flip_vertically();
			image.write_tga_file("output/lesson2/triangle.tga");
		}

		// 随机颜色模型
		{
			const float width = 1000;
			const float height = 1000;
			Model model("resource/african_head/african_head.obj");
			TGAImage image(width, height, TGAImage::RGB);
			for (int i = 0; i<model.nfaces(); i++) 
			{
				std::vector<int> face = model.face(i);
				float2 screen_coords[3];
				for (int j = 0; j<3; j++) 
				{
					float3 v = model.vert(face[j]);
					screen_coords[j] = float2(int((v.x + 1)*width / 2.0), int((v.y + 1)*height / 2.0));
				}
				TGAColor randomColor(rand() % 255, rand() % 255, rand() % 255, 255);
				Interpolation(screen_coords[0], screen_coords[1], screen_coords[2], [&](float2 p) 
				{
					image.set(p.x, p.y, randomColor);
				});
			}
			image.flip_vertically();
			image.write_tga_file("output/lesson2/randomColorModel.tga");
		}

		// 简单光照
		{
			const float width = 1000;
			const float height = 1000;
			float3 lightDir(0, 0, -1);
			Model model("resource/african_head/african_head.obj");
			TGAImage image(width, height, TGAImage::RGB);
			for (int i = 0; i<model.nfaces(); i++) 
			{
				std::vector<int> face = model.face(i);
				float2 screen_coords[3];
				float3 world_coords[3];
				for (int j = 0; j<3; j++) 
				{
					float3 v = model.vert(face[j]);
					screen_coords[j] = float2(int((v.x + 1)*width / 2.0), int((v.y + 1)*height / 2.0));
					world_coords[j] = v;
				}
				float3 a = world_coords[2] - world_coords[0];
				float3 b = world_coords[1] - world_coords[0];
				float3 n = a.cross(b);
				float diffuse = n.normalize().dot(lightDir);
				if (diffuse > 0)
				{
					TGAColor lightColor(diffuse * 255, diffuse * 255, diffuse * 255, 255);
					Interpolation(screen_coords[0], screen_coords[1], screen_coords[2], [&](float2 p) 
					{
						image.set(p.x, p.y, lightColor);
					});
				}
			}
			image.flip_vertically();
			image.write_tga_file("output/lesson2/lightModel.tga");
		}
	}
};

class Lesson3 : public Lesson
{
	const TGAColor red = TGAColor(255, 0, 0, 255);
	const TGAColor blue = TGAColor(0, 0, 255, 255);
	const TGAColor green = TGAColor(0, 255, 0, 255);

public:
	void start() override
	{
		// z-buffer
		{
			const int width = 500;
			const int height = 500;
			float3 lightDir(0, 0, -1);
			Model model("resource/african_head/african_head.obj");
			TGAImage image(width, height, TGAImage::RGB);
			float zBuffer[width][height];
			for(int i = 0; i < width; ++i)
				for(int j = 0; j < height; ++j)
					zBuffer[i][j] = -std::numeric_limits<float>::max();

			for (int i = 0; i<model.nfaces(); i++) 
			{
				std::vector<int> face = model.face(i);
				float3 screen_coords[3];
				float3 world_coords[3];
				for (int j = 0; j<3; j++) 
				{
					float3 v = model.vert(face[j]);
					screen_coords[j] = float3(int((v.x + 1)*width / 2.0 + 0.5), int((v.y + 1)*height / 2.0 + 0.5), v.z);
					world_coords[j] = v;
				}
				float3 a = world_coords[2] - world_coords[0];
				float3 b = world_coords[1] - world_coords[0];
				float3 n = a.cross(b);
				float diffuse = n.normalize().dot(lightDir);
				if (diffuse > 0)
				{
					TGAColor lightColor(diffuse * 255, diffuse * 255, diffuse * 255, 255);
					Interpolation(screen_coords[0], screen_coords[1], screen_coords[2], [&](float3 p) 
					{
						if (p.z > zBuffer[int(p.x)][int(p.y)])
						{
							image.set(p.x, p.y, lightColor);
							zBuffer[int(p.x)][int(p.y)] = p.z;
						}
					});
				}
			}
			image.flip_vertically();
			image.write_tga_file("output/lesson3/zBufferModel.tga");

			image.clear();
			for (int i = 0; i < width; ++i)
			{
				for (int j = 0; j < height; ++j)
				{
					int gray = (zBuffer[i][j] + 1) / 2 * 255;
					image.set(i, j, TGAColor(gray, gray, gray, 255));
				}
			}
			image.flip_vertically();
			image.write_tga_file("output/lesson3/zBuffer.tga");
		}
	}
};

class Lesson4 : public Lesson
{
public:
	void start() override
	{
		auto renderer = new Renderer();

		Model model("resource/african_head/african_head.obj");
		std::vector<vertex> vertexes;
		vertexes.reserve(model.nfaces()*3);
		for (int i = 0; i < model.nfaces(); i++) 
		{
			for (int j = 0; j < 3; j++)
			{
				vertex v;
				v.position = model.vert(i, j) * 400;
				v.uv = model.uv(i, j);
				v.normal = model.normal(i, j);
				vertexes.push_back(v);
			}
		}
		renderer->setVertexes(vertexes);
		renderer->setVertAndFrag(&Lesson4::vert, &Lesson4::frag);
		renderer->render("output/lesson4/pipeline.tga");
	}

	static v2f vert(appdata i)
	{
		v2f o;
		o.position = i.vertex;
		return o;
	}

	static float4 frag(v2f i)
	{
		return float4(1, 1, 1, 1);
	}
};

int main(int argc, char** argv)
{
	vector<Lesson*> lessons = { new Lesson1(), new Lesson2(), new Lesson3(), new Lesson4() };

	lessons[3]->start();

	for (auto lesson : lessons)
	{
		delete lesson;
	}
	lessons.clear();

	cout << "Finish!" << endl;
	//cin.get();
	return 0;
}


#include <string>
#include <vector>
#include <iostream>
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
		// 一条红线
		{
			TGAImage image(100, 100, TGAImage::RGB);
			Interpolation({10, 10}, {90, 90}, [&](float2 p) {
				image.set(p.x, p.y, red);
			});
			image.flip_vertically();
			image.write_tga_file("output/lesson1/redline.tga");
		}

		// 模型
		if (true)
		{
			const float width = 1000;
			const float height = 1000;
			Model model("resource/african_head/african_head.obj");
			TGAImage image(width, height, TGAImage::RGB);
			for (int i = 0; i<model.nfaces(); i++) {
				std::vector<int> face = model.face(i);
				for (int j = 0; j<3; j++) {
					float3 v1 = model.vert(face[j]);
					float3 v2 = model.vert(face[(j + 1) % 3]);
					float2 p1((v1.x + 1.0) * width / 2.0, (v1.y + 1.0) * height / 2.0);
					float2 p2((v2.x + 1.0) * width / 2.0, (v2.y + 1.0) * height / 2.0);
					Interpolation(p1, p2, [&](float2 p) {
						image.set(p.x, p.y, white);
					});
				}
			}
			image.flip_vertically();
			image.write_tga_file("output/lesson1/model.tga");
		}
	}
};

int main(int argc, char** argv)
{
	vector<Lesson*> lessons = { new Lesson1() };

	lessons[0]->start();

	for (auto lesson : lessons)
	{
		delete lesson;
	}
	lessons.clear();

	cin.get();
	return 0;
}


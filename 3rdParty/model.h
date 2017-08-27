#ifndef __MODEL_H__
#define __MODEL_H__
#include <vector>
#include <string>
#include "../types/types.h"
#include "tgaimage.h"

class Model {
private:
    std::vector<float3> verts_;
    std::vector<std::vector<float3> > faces_;
    std::vector<float3> norms_;
    std::vector<float2> uv_;
    TGAImage diffusemap_;
    TGAImage normalmap_;
    TGAImage specularmap_;
    void load_texture(std::string filename, const char *suffix, TGAImage &img);
public:
    Model(const char *filename);
    ~Model();
    int nverts();
    int nfaces();
	float3 normal(int iface, int nthvert);
	float3 normal(float2 uv);
	float3 vert(int i);
	float3 vert(int iface, int nthvert);
	float2 uv(int iface, int nthvert);
    TGAColor diffuse(float2 uv);
    float specular(float2 uv);
    std::vector<int> face(int idx);
};
#endif //__MODEL_H__


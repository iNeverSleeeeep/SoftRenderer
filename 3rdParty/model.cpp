#include <iostream>
#include <fstream>
#include <sstream>
#include "model.h"

Model::Model(const char *filename) : verts_(), faces_(), norms_(), uv_(), diffusemap_(), normalmap_(), specularmap_() {
    std::ifstream in;
    in.open (filename, std::ifstream::in);
    if (in.fail()) return;
    std::string line;
    while (!in.eof()) {
        std::getline(in, line);
        std::istringstream iss(line.c_str());
        char trash;
        if (!line.compare(0, 2, "v ")) {
            iss >> trash;
            float3 v;
			iss >> v.x >> v.y >> v.z;
            verts_.push_back(v);
        } else if (!line.compare(0, 3, "vn ")) {
            iss >> trash >> trash;
			float3 n;
			iss >> n.x >> n.y >> n.z;
            norms_.push_back(n);
        } else if (!line.compare(0, 3, "vt ")) {
            iss >> trash >> trash;
            float2 uv;
			iss >> uv.x >> uv.y;
            uv_.push_back(uv);
        }  else if (!line.compare(0, 2, "f ")) {
            std::vector<float3> f;
            float3 tmp;
            iss >> trash;
            while (iss >> tmp.x >> trash >> tmp.y >> trash >> tmp.z) {
				tmp.x--;tmp.y--;tmp.z--;
                f.push_back(tmp);
            }
            faces_.push_back(f);
        }
    }
    std::cerr << "# v# " << verts_.size() << " f# "  << faces_.size() << " vt# " << uv_.size() << " vn# " << norms_.size() << std::endl;
    load_texture(filename, "_diffuse.tga", diffusemap_);
    load_texture(filename, "_nm_tangent.tga",      normalmap_);
    load_texture(filename, "_spec.tga",    specularmap_);
}

Model::~Model() {}

int Model::nverts() {
    return (int)verts_.size();
}

int Model::nfaces() {
    return (int)faces_.size();
}

std::vector<int> Model::face(int idx) {
    std::vector<int> face;
    for (int i=0; i<(int)faces_[idx].size(); i++) face.push_back((int)faces_[idx][i].x);
    return face;
}

float3 Model::vert(int i) {
    return verts_[i];
}

float3 Model::vert(int iface, int nthvert) {
    return verts_[(int)faces_[iface][nthvert].x];
}

void Model::load_texture(std::string filename, const char *suffix, TGAImage &img) {
    std::string texfile(filename);
    size_t dot = texfile.find_last_of(".");
    if (dot!=std::string::npos) {
        texfile = texfile.substr(0,dot) + std::string(suffix);
        std::cerr << "texture file " << texfile << " loading " << (img.read_tga_file(texfile.c_str()) ? "ok" : "failed") << std::endl;
        img.flip_vertically();
    }
}

TGAColor Model::diffuse(float2 uvf) {
    float2 uv(uvf.x*diffusemap_.get_width(), uvf.y*diffusemap_.get_height());
    return diffusemap_.get(uv.x, uv.y);
}

float3 Model::normal(float2 uvf) {
    float2 uv(uvf.x*normalmap_.get_width(), uvf.y*normalmap_.get_height());
    TGAColor c = normalmap_.get(uv.x, uv.y);
    float3 res;
	res.z = (float)c[0] / 255.f*2.f - 1.f;
	res.y = (float)c[1] / 255.f*2.f - 1.f;
	res.x = (float)c[2] / 255.f*2.f - 1.f;
    return res;
}

float2 Model::uv(int iface, int nthvert) {
    return uv_[(int)faces_[iface][nthvert].y];
}

float Model::specular(float2 uvf) {
	float2 uv(uvf.x*specularmap_.get_width(), uvf.y*specularmap_.get_height());
    return specularmap_.get((int)uv.x, (int)uv.y)[0]/1.f;
}

float3 Model::normal(int iface, int nthvert) {
    int idx = faces_[iface][nthvert].z;
    return norms_[idx].normalize();
}


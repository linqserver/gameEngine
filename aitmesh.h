#ifndef AITMESH_H
#define AITMESH_H

#include <string>
#include <fstream>
#include <vector>

#include "aitvertex.h"

struct faces;

class ait_mesh
{
public:
	ait_mesh() {};
	~ait_mesh() {};
	bool load_from_obj(std::string path);
	std::vector<ait_vertex>& get_vertices(); 
	

private:
	std::vector<ait_vertex> vertices;
	
};


inline std::vector<ait_vertex>& ait_mesh::get_vertices()
{
	return vertices;
}

#endif //AITTEXTUREDMESH_H
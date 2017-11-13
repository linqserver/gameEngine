#include "aitMesh.h"
#include <sstream>
#include <map>
#include <iostream>
#include <assert.h>
#include <cctype>
#include <locale>
#include <ctype.h>

//holds the information about faces 
//
struct faces
{
	unsigned short int f[6];
};

struct slash_is_space : std::ctype<char>
{
	slash_is_space() : ctype<char>(get_table())
	{
	}

	static mask const* get_table()
	{
		static mask rc[table_size];
		rc['/'] = space;
		rc['\n'] = space;
		rc[' '] = space;
		return &rc[0];
	}
};

bool ait_mesh::load_from_obj(std::string path)
{
	std::vector<glm::vec3> ver_and_faces;
	std::vector<faces> face_vect;

	//Attempting openig the file in read mode
	std::ifstream myfile(path);
	if (myfile.is_open())
	{
		std::string line = "";
		glm::vec3 temp = glm::vec3();
		faces face = faces();
		std::string trash = "";

		//reding the file line by line	

		while (getline(myfile, line))
		{
			std::istringstream in(line);

			if (line[0] == 'v')
			{
				in >> trash >> temp.x >> temp.y >> temp.z;
				ver_and_faces.push_back(temp);
			}
			else if (line[0] == 'f')
			{
				in.imbue(std::locale(std::cin.getloc(), new slash_is_space));
				in >> trash >> face.f[0] >> face.f[1] >> face.f[2] >> face.f[3] >> face.f[4] >> face.f[5];
				face_vect.push_back(face);
			}
		}
		myfile.close();
	}
	else
	{
		std::cout << "Unable to open file";
		return false;
	}
	int offset = ver_and_faces.size() / 2;
	for (int i = 0; i < face_vect.size(); i += 1)
	{
		vertices.push_back(ait_vertex(ver_and_faces.at(face_vect.at(i).f[0] - 1),
			ver_and_faces.at(face_vect.at(i).f[1] - 1 + offset)));
		vertices.push_back(ait_vertex(ver_and_faces.at(face_vect.at(i).f[2] - 1),
			ver_and_faces.at(face_vect.at(i).f[3] - 1 + offset)));
		vertices.push_back(ait_vertex(ver_and_faces.at(face_vect.at(i).f[4] - 1),
			ver_and_faces.at(face_vect.at(i).f[5] - 1 + offset)));
	}

	return true;
}

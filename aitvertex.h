#ifndef AITVERTEX_H
#define AITVERTEX_H

#include <glm/glm.hpp>

struct ait_vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    
    ait_vertex(const glm::vec3& pos, const glm::vec3& norm)
    {
        position = pos;
        normal   = norm;
    }
};

#endif //AITVERTEX_H
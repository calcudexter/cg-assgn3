#ifndef _RIDER_HPP_
#define _RIDER_HPP_

#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "gl_framework.hpp"
#include "shader_util.hpp"
#include "hierarchy_node.hpp"
#include "shapes.hpp"
#include <vector>
#include <cmath>

using namespace csX75;

class Human
{
public:
    HNode *left_arm[2], *right_arm[2];
    HNode *left_leg[2], *right_leg[2];
    HNode *torso, *head, *neck;
    Human();
    ~Human();
};

#endif
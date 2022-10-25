#ifndef _BIKE_HPP_
#define _BIKE_HPP_

#include <vector>
#include "gl_framework.hpp"
#include "shader_util.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "hierarchy_node.hpp"
#include "shapes.hpp"

using namespace csX75;

#define NUM_T 1000 

class Bike
{
private:
    float wheel_r, wheel_t, body_w, body_h, body_t;
    float rod_r, rod_l; 
    float rod_hinge_l, rod_body_angle;
    float rear_rot, front_rot, rod_rot;

public:
    HNode* body;
    HNode* rear_wheel, *rear_spoke[20];
    HNode* front_wheel, *front_spoke[20];
    HNode* rod, *steering;

    Bike(float* param);
    void render_bike();
    void update_bike(float* param);
    void initialize_hnode();
    void update_hnode();
    void print_rot();
    void init_arrange();
};

#endif
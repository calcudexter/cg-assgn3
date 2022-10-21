#ifndef _TRACK_HPP_
#define _TRACK_HPP_

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

#ifndef NUM_T 
#define NUM_T 1000
#endif

class Track
{
private:
    HNode* plane1, *plane2;
    HNode* curve1, *curve2;
    HNode* double_ramp1, *double_ramp2, *ramp;

    float track_t, track_l, ramp_l, ramp_h, double_ramp_w;
    float inner_r;
    float scale;

public:
    Track(float* param);
    void render_track();
    void update_track(float* param);
    void initialize_hnode();
    void update_hnode();
};

#endif
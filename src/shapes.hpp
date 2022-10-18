#ifndef _SHAPES_HPP_
#define _SHAPE_HPP_

#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <vector>
#include <cmath>
#include "gl_framework.hpp"
#include "shader_util.hpp"
#include "hierarchy_node.hpp"

class Shape
{
protected:
    int index;
public:
    glm::vec4 col;
    glm::vec4* vert_arr;
    glm::vec4* col_arr;
    int num_vertices;

    virtual void add_vertices(glm::vec4*, glm::vec4*) = 0;
    void insert_quad(glm::vec4* vert_arr, glm::vec4* col_arr, glm::vec4 a, glm::vec4 b, glm::vec4 c, glm::vec4 d);
    void insert_tri(glm::vec4* vert_arr, glm::vec4* col_arr, glm::vec4 a, glm::vec4 b, glm::vec4 c);
    int get_index();
};

class Cylinder : public Shape
{   
    float height;
    float radius;
    int num_tesselations;

public:
    Cylinder(float h, float r, int n, glm::vec4 col);
    ~Cylinder();
    void add_vertices(glm::vec4* vert_arr, glm::vec4* col_arr);
};

class Sphere : public Shape
{   
    float radius;
    int num_tesselations;

public:
    Sphere(float r, int n, glm::vec4 col);
    ~Sphere();
    void add_vertices(glm::vec4* vert_arr, glm::vec4* col_arr);
};

class Cuboid : public Shape
{   
    float a, b, c;

public:
    Cuboid(float a, float b, float c, glm::vec4 col);
    ~Cuboid();
    void add_vertices(glm::vec4* vert_arr, glm::vec4* col_arr);
};
#endif
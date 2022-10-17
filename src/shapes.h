#ifndef _SHAPES_HPP_
#define _SHAPE_HPP_

#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

class Shape
{
protected:
    int index;
public:
    virtual void add_vertices(glm::vec4*) = 0;
    void insert_quad(glm::vec4* vert_arr, glm::vec4 a, glm::vec4 b, glm::vec4 c, glm::vec4 d);
    int get_index();
};

class Cylinder : public Shape
{   
    float height;
    float radius;
    int num_tesselations;

public:
    Cylinder(float h, float r, int n);
    void add_vertices(glm::vec4* vert_arr);
};

class Sphere : public Shape
{   
    float radius;
    int num_tesselations;

public:
    Sphere(float r, int n);
    void add_vertices(glm::vec4* vert_arr);
};
class Cuboid : public Shape
{   
    float a, b, c;

public:
    Cuboid(float a, float b, int c);
    void add_vertices(glm::vec4* vert_arr);
};
#endif
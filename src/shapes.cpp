#include <vector>
#include <cmath>
#include "gl_framework.hpp"
#include "shader_util.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "hierarchy_node.hpp"

using namespace std;

const double PI = 3.141592653589793238463;

class Shape
{
protected:
    int index;
public:
    virtual void add_vertices(glm::vec4*) = 0;
    void insert_quad(glm::vec4* vert_arr, glm::vec4 a, glm::vec4 b, glm::vec4 c, glm::vec4 d)
    {
          vert_arr[this->index] = a; this->index++;
          vert_arr[this->index] = b; this->index++;
          vert_arr[this->index] = c; this->index++;
          vert_arr[this->index] = b; this->index++;
          vert_arr[this->index] = c; this->index++;
          vert_arr[this->index] = d; this->index++;
    }

    int get_index()
    {
        return this->index;
    }
}

class Cylinder : public Shape
{   
    float height;
    float radius;
    int num_tesselations;

public:
    Cylinder(float h, float r, int n)
    {
        this->height = h;
        this->radius = r;
        this->index = 0;
        this->num_tesselations = n;
    }

    void add_vertices(glm::vec4* vert_arr) override
    {   
        double theta = 2*PI/(this->num_tesselations);
        double h_d = (this->h)/(this->num_tesselations)
        for(int i=0; i<(this->num_tesselations); i++)
        {
            for(int j=0; j<(this->num_tesselations), j++)
            {
                float r = this->radius;
                double curr_theta1 = i*theta;
                double curr_theta2 = (i+1)*theta;

                double x1 = r*cos(curr_theta1);
                double x2 = r*cos(curr_theta2);

                double y1 = r*sin(curr_theta1);
                double y2 = r*sin(curr_theta2);

                double z1 = i*h_d - h/2;
                double z2 = (i+1)*h_d - h/2

                glm::vec4 a(x1, y1, z1, 1.0f);
                glm::vec4 b(x2, y2, z1, 1.0f);
                glm::vec4 c(x2, y2, z2, 1.0f);
                glm::vec4 d(x1, y1, z2, 1.0f);

                this->insert_quad(vert_arr, a, b, c, d);
            }
        }   
    }

};

class Sphere : public Shape
{
    float radius;
    int num_tesselations;
    Sphere(float r, int n)
    {
        this->radius = r;
        this->num_tesselations = n;
    }

    void add_vertices(glm::vec4* vert_arr)
    {
        double theta_z = (2*PI)/(this->num_tesselations);
        double theta_x = (2*PI)/(this->num_tesselations)
        for(int i=0; i<(this->num_tesselations); i++)
        {
            for(int j=0; j<(this->num_tesselations), j++)
            {
                float r = this->radius;
                double curr_theta_x1 = i*theta_x;
                double curr_theta_x2 = (i+1)*theta_x;

                double curr_theta_z1 = j*theta_z;
                double curr_theta_z2 = (j+1)*theta_z;

                double x1_z1 = r*sin(curr_theta_z1)*cos(curr_theta_x1);
                double x2_z1 = r*sin(curr_theta_z1)*cos(curr_theta_x2);
                double x1_z2 = r*sin(curr_theta_z2)*cos(curr_theta_x1);
                double x2_z2 = r*sin(curr_theta_z2)*cos(curr_theta_x2);

                double y1_z1 = r*sin(curr_theta_z1)*sin(curr_theta_y1);
                double y2_z1 = r*sin(curr_theta_z1)*sin(curr_theta_y2);
                double y1_z2 = r*sin(curr_theta_z2)*sin(curr_theta_y1);
                double y2_z2 = r*sin(curr_theta_z2)*sin(curr_theta_y2);

                double z1 = r*cos(curr_theta_z1);
                double z2 = r*cos(curr_theta_z2);

                glm::vec4 a(x1_z1, y1_z1, z1, 1.0f);
                glm::vec4 b(x2_z1, y2_z1, z1, 1.0f);
                glm::vec4 c(x2_z2, y2_z2, z2, 1.0f);
                glm::vec4 d(x1_z2, y1_z2, z2, 1.0f);

                this->insert_quad(vert_arr, a, b, c, d);
            }
        }   
    }
};


class Cuboid : public Shape
{
    float a, b, c;
    Sphere(float a, float b, float c)
    {
        this->a = a;
        this->b = b;
        this->c = c;
    }

    void add_vertices(glm::vec4* vert_arr)
    {
     
        glm::vec4 a_v(a/2, b/2, c/2, 1.0f);
        glm::vec4 b_v(a/2, -b/2, c/2, 1.0f);
        glm::vec4 c_v(-a/2, -b/2, c/2, 1.0f);
        glm::vec4 d_v(-a/2, b/2, c/2, 1.0f); 
        glm::vec4 e_v(a/2, b/2, -c/2, 1.0f);
        glm::vec4 f_v(a/2, -b/2, -c/2, 1.0f);  
        glm::vec4 g_v(-a/2, -b/2, -c/2, 1.0f);
        glm::vec4 h_v(-a/2, b/2, -c/2, 1.0f);  

        this->insert_quad(vert_arr, a_v, b_v, c_v, d_v);
        this->insert_quad(vert_arr, a_v, b_v, f_v, e_v);
        this->insert_quad(vert_arr, a_v, e_v, h_v, d_v);
        this->insert_quad(vert_arr, b_v, c_v, g_v, f_v);
        this->insert_quad(vert_arr, d_v, h_v, g_v, c_v);
        this->insert_quad(vert_arr, h_v, e_v, f_v, g_v);
    }
};
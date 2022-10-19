#include "shapes.hpp"
using namespace std;

// const double PI = 3.141592653589793238463;

void Shape::insert_quad(glm::vec4* vert_arr, glm::vec4* col_arr, glm::vec4 a, glm::vec4 b, glm::vec4 c, glm::vec4 d)
{
    vert_arr[this->index] = a; col_arr[this->index] = this->col; this->index++;
    vert_arr[this->index] = b; col_arr[this->index] = this->col; this->index++;
    vert_arr[this->index] = c; col_arr[this->index] = this->col; this->index++;
    vert_arr[this->index] = a; col_arr[this->index] = this->col; this->index++;
    vert_arr[this->index] = c; col_arr[this->index] = this->col; this->index++;
    vert_arr[this->index] = d; col_arr[this->index] = this->col; this->index++;
}

void Shape::insert_tri(glm::vec4* vert_arr, glm::vec4* col_arr, glm::vec4 a, glm::vec4 b, glm::vec4 c)
{
    vert_arr[this->index] = a; col_arr[this->index] = this->col; this->index++;
    vert_arr[this->index] = b; col_arr[this->index] = this->col; this->index++;
    vert_arr[this->index] = c; col_arr[this->index] = this->col; this->index++;
}

int Shape::get_index()
{
    return this->index;
}

void Cylinder::add_vertices(glm::vec4* vert_arr, glm::vec4* col_arr)
{   
    double theta = 2*M_PI/(this->num_tesselations);
    for(int i=0; i<(this->num_tesselations); i++)
    {
        float r = this->radius;
        float h = this->height;
        double curr_theta1 = i*theta;
        double curr_theta2 = (i+1)*theta;

        double x1 = r*cos(curr_theta1);
        double x2 = r*cos(curr_theta2);

        double y1 = r*sin(curr_theta1);
        double y2 = r*sin(curr_theta2);

        double z1 = -h/2;
        double z2 = h/2;

        glm::vec4 a(x1, y1, z1, 1.0f);
        glm::vec4 b(x2, y2, z1, 1.0f);
        glm::vec4 c(x2, y2, z2, 1.0f);
        glm::vec4 d(x1, y1, z2, 1.0f);
        glm::vec4 e(0.0f, 0.0f, z1, 1.0f);
        glm::vec4 f(0.0f, 0.0f, z2, 1.0f);

        this->insert_quad(vert_arr, col_arr, a, b, c, d);
        this->insert_tri(vert_arr, col_arr, a, b, e);
        this->insert_tri(vert_arr, col_arr, c, d, f);
    }   
}

Cylinder::Cylinder(float h, float r, int n, glm::vec4 col)
{
    this->height = h;
    this->radius = r;
    this->index = 0;
    this->col = col;
    this->num_tesselations = n;
    this->num_vertices = 12*n;
    this->vert_arr = new glm::vec4[12*n];
    this->col_arr = new glm::vec4[12*n];
    this->add_vertices(this->vert_arr, this->col_arr);
}
Cylinder::~Cylinder()
{
    delete this->vert_arr;
    delete this->col_arr;
}


void Sphere::add_vertices(glm::vec4* vert_arr, glm::vec4* col_arr)
{
    double theta_z = M_PI/(this->num_tesselations);
    double theta_x = (2*M_PI)/(this->num_tesselations);
    for(int i=0; i<(this->num_tesselations); i++)
    {
        for(int j=0; j<(this->num_tesselations); j++)
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

            double y1_z1 = r*sin(curr_theta_z1)*sin(curr_theta_x1);
            double y2_z1 = r*sin(curr_theta_z1)*sin(curr_theta_x2);
            double y1_z2 = r*sin(curr_theta_z2)*sin(curr_theta_x1);
            double y2_z2 = r*sin(curr_theta_z2)*sin(curr_theta_x2);

            double z1 = r*cos(curr_theta_z1);
            double z2 = r*cos(curr_theta_z2);

            glm::vec4 a(x1_z1, y1_z1, z1, 1.0f);
            glm::vec4 b(x2_z1, y2_z1, z1, 1.0f);
            glm::vec4 c(x2_z2, y2_z2, z2, 1.0f);
            glm::vec4 d(x1_z2, y1_z2, z2, 1.0f);

            this->insert_quad(vert_arr, col_arr, a, b, c, d);
        }
    }   
}

Sphere::Sphere(float r, int n, glm::vec4 col)
{
    this->radius = r;
    this->num_tesselations = n;
    this->col = col;
    this->index = 0;
    this->num_vertices = 6*n*n;
    this->vert_arr = new glm::vec4[6*n*n];
    this->col_arr = new glm::vec4[6*n*n];
    this->add_vertices(this->vert_arr, this->col_arr);
}
Sphere::~Sphere()
{
    delete this->vert_arr;
    delete this->col_arr;
}

void Cuboid::add_vertices(glm::vec4* vert_arr, glm::vec4* col_arr)
{     
    glm::vec4 a_v(a/2, b/2, c/2, 1.0f);
    glm::vec4 b_v(a/2, -b/2, c/2, 1.0f);
    glm::vec4 c_v(-a/2, -b/2, c/2, 1.0f);
    glm::vec4 d_v(-a/2, b/2, c/2, 1.0f); 
    glm::vec4 e_v(a/2, b/2, -c/2, 1.0f);
    glm::vec4 f_v(a/2, -b/2, -c/2, 1.0f);  
    glm::vec4 g_v(-a/2, -b/2, -c/2, 1.0f);
    glm::vec4 h_v(-a/2, b/2, -c/2, 1.0f);  

    this->insert_quad(vert_arr, col_arr, a_v, b_v, c_v, d_v);
    this->insert_quad(vert_arr, col_arr, a_v, b_v, f_v, e_v);
    this->insert_quad(vert_arr, col_arr, a_v, e_v, h_v, d_v);
    this->insert_quad(vert_arr, col_arr, b_v, c_v, g_v, f_v);
    this->insert_quad(vert_arr, col_arr, d_v, h_v, g_v, c_v);
    this->insert_quad(vert_arr, col_arr, h_v, e_v, f_v, g_v);
}

Cuboid::Cuboid(float a, float b, float c, glm::vec4 col)
{
    this->a = a;
    this->b = b;
    this->c = c;
    this->col = col;
    this->index = 0;
    this->num_vertices = 36;
    this->vert_arr = new glm::vec4[36];
    this->col_arr = new glm::vec4[36];
    this->add_vertices(this->vert_arr, this->col_arr);
}
Cuboid::~Cuboid()
{
    delete this->vert_arr;
    delete this->col_arr;
}
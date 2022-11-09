#include "shapes.hpp"
using namespace std;

glm::vec2 t_coords[4] = {
    glm::vec2( 0.0, 0.0),
    glm::vec2( 0.0, 1.0),
    glm::vec2( 1.0, 0.0),
    glm::vec2( 1.0, 1.0)
};

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

void Cylinder::add_vertices(glm::vec4* vert_arr, glm::vec4* col_arr, glm::vec4* norm_arr)
{   
    int ind = 0;
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
        norm_arr[ind++] = (a-e);
        norm_arr[ind++] = (b-e);
        norm_arr[ind++] = (c-f);
        norm_arr[ind++] = (a-e);
        norm_arr[ind++] = (c-f);
        norm_arr[ind++] = (d-f);

        this->insert_tri(vert_arr, col_arr, a, b, e);
        norm_arr[ind++] = e;
        norm_arr[ind++] = e;
        norm_arr[ind++] = e;

        this->insert_tri(vert_arr, col_arr, c, d, f);
        norm_arr[ind++] = f;
        norm_arr[ind++] = f;
        norm_arr[ind++] = f;
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
    this->norm_arr = new glm::vec4[12*n];
    this->add_vertices(this->vert_arr, this->col_arr, this->norm_arr);
}
Cylinder::~Cylinder()
{
    delete this->vert_arr;
    delete this->col_arr;
    delete this->norm_arr;
}

void CylinderRim::add_vertices(glm::vec4* vert_arr, glm::vec4* col_arr, glm::vec4* norm_arr)
{   
    int ind = 0;
    double theta = 2*M_PI/(this->num_tesselations);
    for(int i=0; i<(this->num_tesselations); i++)
    {
        float iR = this->inRadius;
        float oR = this->outRadius;
        float h = this->height;
        double curr_theta1 = i*theta;
        double curr_theta2 = (i+1)*theta;

        double ix1 = iR*cos(curr_theta1);
        double ix2 = iR*cos(curr_theta2);

        double iy1 = iR*sin(curr_theta1);
        double iy2 = iR*sin(curr_theta2);

        double z1 = -h/2;
        double z2 = h/2;

        glm::vec4 a(ix1, iy1, z1, 1.0f);
        glm::vec4 b(ix2, iy2, z1, 1.0f);
        glm::vec4 c(ix2, iy2, z2, 1.0f);
        glm::vec4 d(ix1, iy1, z2, 1.0f);

        glm::vec4 e(0.0f, 0.0f, z1, 1.0f);
        glm::vec4 f(0.0f, 0.0f, z2, 1.0f);

        this->insert_quad(vert_arr, col_arr, a, b, c, d);
        norm_arr[ind++] = -(a-e);
        norm_arr[ind++] = -(b-e);
        norm_arr[ind++] = -(c-f);
        norm_arr[ind++] = -(a-e);
        norm_arr[ind++] = -(c-f);
        norm_arr[ind++] = -(d-f);

        double ox1 = oR*cos(curr_theta1);
        double ox2 = oR*cos(curr_theta2);

        double oy1 = oR*sin(curr_theta1);
        double oy2 = oR*sin(curr_theta2);

        glm::vec4 p(ox1, oy1, z1, 1.0f);
        glm::vec4 q(ox2, oy2, z1, 1.0f);
        glm::vec4 r(ox2, oy2, z2, 1.0f);
        glm::vec4 s(ox1, oy1, z2, 1.0f);

        this->insert_quad(vert_arr, col_arr, p, q, r, s);
        norm_arr[ind++] = (p-e);
        norm_arr[ind++] = (q-e);
        norm_arr[ind++] = (r-f);
        norm_arr[ind++] = (p-e);
        norm_arr[ind++] = (r-f);
        norm_arr[ind++] = (s-f);

        this->insert_quad(vert_arr, col_arr, a, b, q, p);
        norm_arr[ind++] = e;
        norm_arr[ind++] = e;
        norm_arr[ind++] = e;
        norm_arr[ind++] = e;
        norm_arr[ind++] = e;
        norm_arr[ind++] = e;

        this->insert_quad(vert_arr, col_arr, d, c, r, s);
        norm_arr[ind++] = f;
        norm_arr[ind++] = f;
        norm_arr[ind++] = f;
        norm_arr[ind++] = f;
        norm_arr[ind++] = f;
        norm_arr[ind++] = f;
    }   
}

CylinderRim::CylinderRim(float h, float iR, float oR, int n, glm::vec4 col)
{
    this->height = h;
    this->inRadius = iR;
    this->outRadius = oR;
    this->index = 0;
    this->col = col;
    this->num_tesselations = n;
    this->num_vertices = 24*n;
    this->vert_arr = new glm::vec4[24*n];
    this->col_arr = new glm::vec4[24*n];
    this->norm_arr = new glm::vec4[24*n];
    this->add_vertices(this->vert_arr, this->col_arr, this->norm_arr);
}
CylinderRim::~CylinderRim()
{
    delete this->vert_arr;
    delete this->col_arr;
    delete this->norm_arr;
}

void Sphere::add_vertices(glm::vec4* vert_arr, glm::vec4* col_arr, glm::vec4* norm_arr)
{
    int ind = 0;

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
            norm_arr[ind++] = a;
            norm_arr[ind++] = b;
            norm_arr[ind++] = c;
            norm_arr[ind++] = a;
            norm_arr[ind++] = c;
            norm_arr[ind++] = d;
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
    this->norm_arr = new glm::vec4[6*n*n];
    this->add_vertices(this->vert_arr, this->col_arr, this->norm_arr);
}
Sphere::~Sphere()
{
    delete this->vert_arr;
    delete this->col_arr;
    delete this->norm_arr;
}

// void Cuboid::insert_quad(glm::vec4* vert_arr, glm::vec4* col_arr, glm::vec4 a, glm::vec4 b, glm::vec4 c, glm::vec4 d)
// {
//     vert_arr[this->index] = a; col_arr[this->index] = this->col; this->index++;
//     vert_arr[this->index] = b; col_arr[this->index] = this->col; this->index++;
//     vert_arr[this->index] = c; col_arr[this->index] = this->col; this->index++;
//     vert_arr[this->index] = a; col_arr[this->index] = this->col; this->index++;
//     vert_arr[this->index] = c; col_arr[this->index] = this->col; this->index++;
//     vert_arr[this->index] = d; col_arr[this->index] = this->col; this->index++;
// }



void Cuboid::add_vertices(glm::vec4* vert_arr, glm::vec4* col_arr, glm::vec4* norm_arr)
{   
    int ind = 0;
    
    glm::vec4 a_v(a/2, b/2, c/2, 1.0f);
    glm::vec4 b_v(a/2, -b/2, c/2, 1.0f);
    glm::vec4 c_v(-a/2, -b/2, c/2, 1.0f);
    glm::vec4 d_v(-a/2, b/2, c/2, 1.0f); 
    glm::vec4 e_v(a/2, b/2, -c/2, 1.0f);
    glm::vec4 f_v(a/2, -b/2, -c/2, 1.0f);  
    glm::vec4 g_v(-a/2, -b/2, -c/2, 1.0f);
    glm::vec4 h_v(-a/2, b/2, -c/2, 1.0f);  

    glm::vec4 normal[6];
    normal[0] = -glm::vec4(triangleNormal(glm::vec3(a_v), glm::vec3(b_v), glm::vec3(c_v)), 1.0f);
    normal[1] = glm::vec4(triangleNormal(glm::vec3(a_v), glm::vec3(b_v), glm::vec3(f_v)), 1.0f);
    normal[2] = glm::vec4(triangleNormal(glm::vec3(a_v), glm::vec3(e_v), glm::vec3(h_v)), 1.0f);
    normal[3] = glm::vec4(triangleNormal(glm::vec3(b_v), glm::vec3(c_v), glm::vec3(g_v)), 1.0f);
    normal[4] = glm::vec4(triangleNormal(glm::vec3(d_v), glm::vec3(h_v), glm::vec3(g_v)), 1.0f);
    normal[5] = glm::vec4(triangleNormal(glm::vec3(h_v), glm::vec3(e_v), glm::vec3(f_v)), 1.0f);

    this->insert_quad(vert_arr, col_arr, a_v, b_v, c_v, d_v);
    norm_arr[ind++] = normal[0];
    norm_arr[ind++] = normal[0];
    norm_arr[ind++] = normal[0];
    norm_arr[ind++] = normal[0];
    norm_arr[ind++] = normal[0];
    norm_arr[ind++] = normal[0];

    this->insert_quad(vert_arr, col_arr, a_v, b_v, f_v, e_v);
    norm_arr[ind++] = normal[1];
    norm_arr[ind++] = normal[1];
    norm_arr[ind++] = normal[1];
    norm_arr[ind++] = normal[1];
    norm_arr[ind++] = normal[1];
    norm_arr[ind++] = normal[1];

    this->insert_quad(vert_arr, col_arr, a_v, e_v, h_v, d_v);
    norm_arr[ind++] = normal[2];
    norm_arr[ind++] = normal[2];
    norm_arr[ind++] = normal[2];
    norm_arr[ind++] = normal[2];
    norm_arr[ind++] = normal[2];
    norm_arr[ind++] = normal[2];
    
    this->insert_quad(vert_arr, col_arr, b_v, c_v, g_v, f_v);
    norm_arr[ind++] = normal[3];
    norm_arr[ind++] = normal[3];
    norm_arr[ind++] = normal[3];
    norm_arr[ind++] = normal[3];
    norm_arr[ind++] = normal[3];
    norm_arr[ind++] = normal[3];

    this->insert_quad(vert_arr, col_arr, d_v, h_v, g_v, c_v);
    norm_arr[ind++] = normal[4];
    norm_arr[ind++] = normal[4];
    norm_arr[ind++] = normal[4];
    norm_arr[ind++] = normal[4];
    norm_arr[ind++] = normal[4];
    norm_arr[ind++] = normal[4];

    this->insert_quad(vert_arr, col_arr, h_v, e_v, f_v, g_v);
    norm_arr[ind++] = normal[5];
    norm_arr[ind++] = normal[5];
    norm_arr[ind++] = normal[5];
    norm_arr[ind++] = normal[5];
    norm_arr[ind++] = normal[5];
    norm_arr[ind++] = normal[5];
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
    this->norm_arr = new glm::vec4[36];
    this->add_vertices(this->vert_arr, this->col_arr, this->norm_arr);
}

Cuboid::~Cuboid()
{
    delete this->vert_arr;
    delete this->col_arr;
    delete this->norm_arr;
}

void TexCuboid::insert_tex_quad(glm::vec4* vert_arr, glm::vec2* tex_arr, glm::vec4 a, glm::vec4 b, glm::vec4 c, glm::vec4 d)
{
    vert_arr[this->index] = a; tex_arr[this->index] = this->tex_coords[1]; this->index++;
    vert_arr[this->index] = b; tex_arr[this->index] = this->tex_coords[0]; this->index++;
    vert_arr[this->index] = c; tex_arr[this->index] = this->tex_coords[2]; this->index++;
    vert_arr[this->index] = a; tex_arr[this->index] = this->tex_coords[1]; this->index++;
    vert_arr[this->index] = c; tex_arr[this->index] = this->tex_coords[2]; this->index++;
    vert_arr[this->index] = d; tex_arr[this->index] = this->tex_coords[3]; this->index++;
}

TexCuboid::TexCuboid(float a, float b, float c, glm::vec4 col)
{
    this->a = a;
    this->b = b;
    this->c = c;
    this->col = col;
    this->index = 0;
    this->num_vertices = 36;
    this->vert_arr = new glm::vec4[36];
    this->norm_arr = new glm::vec4[36];
    this->col_arr = new glm::vec4[36];
    this->tex_arr = new glm::vec2[36];
    this->tex_coords = {
        glm::vec2(0.0, 0.0),
        glm::vec2(0.0, 1.0),
        glm::vec2(1.0, 0.0),
        glm::vec2(1.0, 1.0)
        };
    this->add_vertices(this->vert_arr, this->tex_arr, this->norm_arr);
}

void TexCuboid::add_vertices(glm::vec4* vert_arr, glm::vec2* tex_arr, glm::vec4* norm_arr)
{   
    int ind = 0;
    
    glm::vec4 a_v(a/2, b/2, c/2, 1.0f);
    glm::vec4 b_v(a/2, -b/2, c/2, 1.0f);
    glm::vec4 c_v(-a/2, -b/2, c/2, 1.0f);
    glm::vec4 d_v(-a/2, b/2, c/2, 1.0f); 
    glm::vec4 e_v(a/2, b/2, -c/2, 1.0f);
    glm::vec4 f_v(a/2, -b/2, -c/2, 1.0f);  
    glm::vec4 g_v(-a/2, -b/2, -c/2, 1.0f);
    glm::vec4 h_v(-a/2, b/2, -c/2, 1.0f);  

    glm::vec4 normal[6];
    normal[0] = glm::vec4(triangleNormal(glm::vec3(a_v), glm::vec3(b_v), glm::vec3(c_v)), 1.0f);
    normal[1] = glm::vec4(triangleNormal(glm::vec3(a_v), glm::vec3(b_v), glm::vec3(f_v)), 1.0f);
    normal[2] = glm::vec4(triangleNormal(glm::vec3(a_v), glm::vec3(e_v), glm::vec3(h_v)), 1.0f);
    normal[3] = glm::vec4(triangleNormal(glm::vec3(b_v), glm::vec3(c_v), glm::vec3(g_v)), 1.0f);
    normal[4] = glm::vec4(triangleNormal(glm::vec3(d_v), glm::vec3(h_v), glm::vec3(g_v)), 1.0f);
    normal[5] = glm::vec4(triangleNormal(glm::vec3(h_v), glm::vec3(e_v), glm::vec3(f_v)), 1.0f);

    this->insert_tex_quad(vert_arr, tex_arr, a_v, b_v, c_v, d_v);
    norm_arr[ind++] = normal[0];
    norm_arr[ind++] = normal[0];
    norm_arr[ind++] = normal[0];
    norm_arr[ind++] = normal[0];
    norm_arr[ind++] = normal[0];
    norm_arr[ind++] = normal[0];

    this->insert_tex_quad(vert_arr, tex_arr, a_v, b_v, f_v, e_v);
    norm_arr[ind++] = normal[1];
    norm_arr[ind++] = normal[1];
    norm_arr[ind++] = normal[1];
    norm_arr[ind++] = normal[1];
    norm_arr[ind++] = normal[1];
    norm_arr[ind++] = normal[1];

    this->insert_tex_quad(vert_arr, tex_arr, a_v, e_v, h_v, d_v);
    norm_arr[ind++] = normal[2];
    norm_arr[ind++] = normal[2];
    norm_arr[ind++] = normal[2];
    norm_arr[ind++] = normal[2];
    norm_arr[ind++] = normal[2];
    norm_arr[ind++] = normal[2];
    
    this->insert_tex_quad(vert_arr, tex_arr, b_v, c_v, g_v, f_v);
    norm_arr[ind++] = normal[3];
    norm_arr[ind++] = normal[3];
    norm_arr[ind++] = normal[3];
    norm_arr[ind++] = normal[3];
    norm_arr[ind++] = normal[3];
    norm_arr[ind++] = normal[3];

    this->insert_tex_quad(vert_arr, tex_arr, d_v, h_v, g_v, c_v);
    norm_arr[ind++] = normal[4];
    norm_arr[ind++] = normal[4];
    norm_arr[ind++] = normal[4];
    norm_arr[ind++] = normal[4];
    norm_arr[ind++] = normal[4];
    norm_arr[ind++] = normal[4];

    this->insert_tex_quad(vert_arr, tex_arr, h_v, e_v, f_v, g_v);
    norm_arr[ind++] = normal[5];
    norm_arr[ind++] = normal[5];
    norm_arr[ind++] = normal[5];
    norm_arr[ind++] = normal[5];
    norm_arr[ind++] = normal[5];
    norm_arr[ind++] = normal[5];
}


TexCuboid::~TexCuboid()
{
    delete this->vert_arr;
    delete this->col_arr;
    delete this->norm_arr;
    delete this->tex_arr;
}


Track_curve::Track_curve(float ir, float out_r, int nt, float ang, glm::vec4 col)
{
    this->inner_r = ir;
    this->outer_r = out_r;
    this->angle = ang;
    this->num_t = nt;
    this->col = col;
    this->num_vertices = 6*num_t;
    this->vert_arr = new glm::vec4[this->num_vertices];
    this->col_arr = new glm::vec4[this->num_vertices];
    this->norm_arr = new glm::vec4[this->num_vertices];
    this->add_vertices(this->vert_arr, this->col_arr, this->norm_arr);
}

void Track_curve::add_vertices(glm::vec4* vert_arr, glm::vec4* col_arr, glm::vec4* norm_arr)
{   

    int ind = 0;
    float d_theta = glm::radians(this->angle)/(this->num_t);
    float curr_theta1 = 0.0f;
    float curr_theta2 = 0.0f;

    for(int i=0; i<this->num_t; i++)
    {
        curr_theta1 = d_theta*i;
        curr_theta2 = d_theta*(i+1);
        float x1 = this->inner_r*cos(curr_theta1);
        float x2 = this->outer_r*cos(curr_theta1);
        float x3 = this->outer_r*cos(curr_theta2);
        float x4 = this->inner_r*cos(curr_theta2);

        float y1 = this->inner_r*sin(curr_theta1);
        float y2 = this->outer_r*sin(curr_theta1);
        float y3 = this->outer_r*sin(curr_theta2);
        float y4 = this->inner_r*sin(curr_theta2);

        glm::vec4 a(x1, y1, 0.0f, 1.0f);
        glm::vec4 b(x2, y2, 0.0f, 1.0f);
        glm::vec4 c(x3, y3, 0.0f, 1.0f);
        glm::vec4 d(x4, y4, 0.0f, 1.0f);

        // this->insert_tri(this->vert_arr, this->col_arr, a, b, c);
        // this->insert_tri(this->vert_arr, this->col_arr, a, c, d);
        this->insert_quad(this->vert_arr, this->col_arr, a, b, c, d);
        glm::vec4 normal = glm::vec4(triangleNormal(glm::vec3(a), glm::vec3(b), glm::vec3(c)), 1.0f);
        norm_arr[ind++] = normal;
        norm_arr[ind++] = normal;
        norm_arr[ind++] = normal;
        norm_arr[ind++] = normal;
        norm_arr[ind++] = normal;
        norm_arr[ind++] = normal;
    }
}

Track_curve::~Track_curve()
{
    delete this->vert_arr;
    delete this->col_arr;
    delete this->norm_arr;
}


// track plane
Track_plane::Track_plane(float ph, float pw, glm::vec4 col)
{
    this->plane_h = ph;
    this->plane_w = pw;
    this->num_vertices = 6;
    this->col = col;
    this->vert_arr = new glm::vec4[this->num_vertices];
    this->col_arr = new glm::vec4[this->num_vertices];
    this->norm_arr = new glm::vec4[this->num_vertices];
    this->add_vertices(this->vert_arr, this->col_arr, this->norm_arr);
}

void Track_plane::add_vertices(glm::vec4* vert_arr, glm::vec4* col_arr, glm::vec4* norm_arr)
{   

    glm::vec4 a(this->plane_w/2, this->plane_h/2, 0.0f, 1.0f);
    glm::vec4 b(this->plane_w/2, -this->plane_h/2, 0.0f, 1.0f);
    glm::vec4 c(-this->plane_w/2, -this->plane_h/2, 0.0f, 1.0f);
    glm::vec4 d(-this->plane_w/2, this->plane_h/2, 0.0f, 1.0f);

    this->insert_quad(this->vert_arr, this->col_arr, a, b, c, d);
    
    // The order of insertion in this case is a bit different, leave it for now
    int ind = 0;
    glm::vec4 normal = glm::vec4(triangleNormal(glm::vec3(a), glm::vec3(b), glm::vec3(c)), 1.0f);
    norm_arr[ind++] = -normal;
    norm_arr[ind++] = -normal;
    norm_arr[ind++] = -normal;
    norm_arr[ind++] = -normal;
    norm_arr[ind++] = -normal;
    norm_arr[ind++] = -normal;
}

Track_plane::~Track_plane()
{
    delete this->vert_arr;
    delete this->col_arr;
    delete this->norm_arr;
}

// Track ramp
Track_ramp::Track_ramp(float l, float w, float h, glm::vec4 col)
{
    this->ramp_l = l;
    this->ramp_w = w;
    this->ramp_h = h;
    this->col = col;
    this->num_vertices = 18;
    this->vert_arr = new glm::vec4[this->num_vertices];
    this->col_arr = new glm::vec4[this->num_vertices];
    this->norm_arr = new glm::vec4[this->num_vertices];
    this->add_vertices(this->vert_arr, this->col_arr, this->norm_arr);
}

void Track_ramp::add_vertices(glm::vec4* vert_arr, glm::vec4* col_arr, glm::vec4* norm_arr)
{   
    glm::vec4 a(0.0f, 0.0f, 0.0f, 1.0f);
    glm::vec4 b(this->ramp_l, 0.0f, 0.0f, 1.0f);
    glm::vec4 c(this->ramp_l, this->ramp_w, 0.0f, 1.0f);
    glm::vec4 d(0.0f, this->ramp_w, 0.0f, 1.0f);
    glm::vec4 e(this->ramp_l, 0.0f, this->ramp_h, 1.0f);
    glm::vec4 f(this->ramp_l, this->ramp_w, this->ramp_h, 1.0f);

    int ind = 0;
    this->insert_quad(this->vert_arr, this->col_arr, a, e, f, d);
    glm::vec4 normal = glm::vec4(triangleNormal(glm::vec3(a), glm::vec3(e), glm::vec3(f)), 1.0f);
    norm_arr[ind++] = normal;
    norm_arr[ind++] = normal;
    norm_arr[ind++] = normal;
    norm_arr[ind++] = normal;
    norm_arr[ind++] = normal;
    norm_arr[ind++] = normal;

    this->insert_quad(this->vert_arr, this->col_arr, e, b, c, f);
    normal = glm::vec4(triangleNormal(glm::vec3(e), glm::vec3(b), glm::vec3(c)), 1.0f);
    norm_arr[ind++] = normal;
    norm_arr[ind++] = normal;
    norm_arr[ind++] = normal;
    norm_arr[ind++] = normal;
    norm_arr[ind++] = normal;
    norm_arr[ind++] = normal;

    this->insert_tri(this->vert_arr, this->col_arr, a, b, e);
    normal = glm::vec4(triangleNormal(glm::vec3(a), glm::vec3(b), glm::vec3(e)), 1.0f);
    norm_arr[ind++] = normal;
    norm_arr[ind++] = normal;
    norm_arr[ind++] = normal;

    this->insert_tri(this->vert_arr, this->col_arr, c, d, f);
    normal = glm::vec4(triangleNormal(glm::vec3(c), glm::vec3(d), glm::vec3(f)), 1.0f);
    norm_arr[ind++] = normal;
    norm_arr[ind++] = normal;
    norm_arr[ind++] = normal;
}

Track_ramp::~Track_ramp()
{
    delete this->vert_arr;
    delete this->col_arr;
    delete this->norm_arr;
}



void SkyBox::insert_tex_quad(glm::vec4* vert_arr, glm::vec2* tex_arr, glm::vec4 a, glm::vec4 b, glm::vec4 c, glm::vec4 d, float s_x, float s_y)
{
    vert_arr[this->index] = a; tex_arr[this->index] = glm::vec2(s_x, s_y); this->index++;
    vert_arr[this->index] = b; tex_arr[this->index] = glm::vec2(s_x, s_y+1.0/3); this->index++;
    vert_arr[this->index] = c; tex_arr[this->index] = glm::vec2(s_x+1.0/4, s_y+1.0/3); this->index++;
    vert_arr[this->index] = a; tex_arr[this->index] = glm::vec2(s_x, s_y); this->index++;
    vert_arr[this->index] = c; tex_arr[this->index] = glm::vec2(s_x+1.0/4, s_y+1.0/3); this->index++;
    vert_arr[this->index] = d; tex_arr[this->index] = glm::vec2(s_x+1.0/4, s_y); this->index++;
//    vert_arr[this->index] = a; tex_arr[this->index] = this->tex_coords[1]; this->index++;
//     vert_arr[this->index] = b; tex_arr[this->index] = this->tex_coords[0]; this->index++;
//     vert_arr[this->index] = c; tex_arr[this->index] = this->tex_coords[2]; this->index++;
//     vert_arr[this->index] = a; tex_arr[this->index] = this->tex_coords[1]; this->index++;
//     vert_arr[this->index] = c; tex_arr[this->index] = this->tex_coords[2]; this->index++;
//     vert_arr[this->index] = d; tex_arr[this->index] = this->tex_coords[3]; this->index++;

}

SkyBox::SkyBox(float a, float b, float c, glm::vec4 col)
{
    this->a = a;
    this->b = b;
    this->c = c;
    this->col = col;
    this->index = 0;
    this->num_vertices = 36;
    this->vert_arr = new glm::vec4[36];
    this->norm_arr = new glm::vec4[36];
    this->col_arr = new glm::vec4[36];
    this->tex_arr = new glm::vec2[36];
    this->tex_coords = {
        glm::vec2(0.0, 0.0),
        glm::vec2(0.0, 1.0),
        glm::vec2(1.0, 0.0),
        glm::vec2(1.0, 1.0)
        };
    this->add_vertices(this->vert_arr, this->tex_arr, this->norm_arr);
}

void SkyBox::add_vertices(glm::vec4* vert_arr, glm::vec2* col_arr, glm::vec4* norm_arr)
{   
    int ind = 0;
    
    glm::vec4 a_v(a/2, b/2, c/2, 1.0f);
    glm::vec4 b_v(a/2, -b/2, c/2, 1.0f);
    glm::vec4 c_v(-a/2, -b/2, c/2, 1.0f);
    glm::vec4 d_v(-a/2, b/2, c/2, 1.0f); 
    glm::vec4 e_v(a/2, b/2, -c/2, 1.0f);
    glm::vec4 f_v(a/2, -b/2, -c/2, 1.0f);  
    glm::vec4 g_v(-a/2, -b/2, -c/2, 1.0f);
    glm::vec4 h_v(-a/2, b/2, -c/2, 1.0f);  

    glm::vec4 normal[6];
    normal[0] = glm::vec4(triangleNormal(glm::vec3(a_v), glm::vec3(b_v), glm::vec3(c_v)), 1.0f);
    normal[1] = glm::vec4(triangleNormal(glm::vec3(a_v), glm::vec3(b_v), glm::vec3(f_v)), 1.0f);
    normal[2] = glm::vec4(triangleNormal(glm::vec3(a_v), glm::vec3(e_v), glm::vec3(h_v)), 1.0f);
    normal[3] = glm::vec4(triangleNormal(glm::vec3(b_v), glm::vec3(c_v), glm::vec3(g_v)), 1.0f);
    normal[4] = glm::vec4(triangleNormal(glm::vec3(d_v), glm::vec3(h_v), glm::vec3(g_v)), 1.0f);
    normal[5] = glm::vec4(triangleNormal(glm::vec3(h_v), glm::vec3(e_v), glm::vec3(f_v)), 1.0f);

    float eps = 0.01;
    this->insert_tex_quad(vert_arr, tex_arr, c_v, d_v, a_v, b_v, 0.25, 2.0/3);//
    norm_arr[ind++] = normal[0];
    norm_arr[ind++] = normal[0];
    norm_arr[ind++] = normal[0];
    norm_arr[ind++] = normal[0];
    norm_arr[ind++] = normal[0];
    norm_arr[ind++] = normal[0];

    this->insert_tex_quad(vert_arr, tex_arr, g_v, c_v, b_v, f_v, 0.25, 1.0/3);//
    norm_arr[ind++] = normal[1];
    norm_arr[ind++] = normal[1];
    norm_arr[ind++] = normal[1];
    norm_arr[ind++] = normal[1];
    norm_arr[ind++] = normal[1];
    norm_arr[ind++] = normal[1];

    this->insert_tex_quad(vert_arr, tex_arr, h_v, d_v, c_v, g_v, 0.0+eps, 1.0/3);//
    norm_arr[ind++] = normal[2];
    norm_arr[ind++] = normal[2];
    norm_arr[ind++] = normal[2];
    norm_arr[ind++] = normal[2];
    norm_arr[ind++] = normal[2];
    norm_arr[ind++] = normal[2];
    
    this->insert_tex_quad(vert_arr, tex_arr, h_v, g_v, f_v, e_v, 1.0/4, 0.0);//
    norm_arr[ind++] = normal[3];
    norm_arr[ind++] = normal[3];
    norm_arr[ind++] = normal[3];
    norm_arr[ind++] = normal[3];
    norm_arr[ind++] = normal[3];
    norm_arr[ind++] = normal[3];

    this->insert_tex_quad(vert_arr, tex_arr, f_v, b_v, a_v, e_v, 0.5, 1.0/3);//
    norm_arr[ind++] = normal[4];
    norm_arr[ind++] = normal[4];
    norm_arr[ind++] = normal[4];
    norm_arr[ind++] = normal[4];
    norm_arr[ind++] = normal[4];
    norm_arr[ind++] = normal[4];

    this->insert_tex_quad(vert_arr, tex_arr, e_v, a_v, d_v, h_v, 0.75, 1.0/3);//
    norm_arr[ind++] = normal[5];
    norm_arr[ind++] = normal[5];
    norm_arr[ind++] = normal[5];
    norm_arr[ind++] = normal[5];
    norm_arr[ind++] = normal[5];
    norm_arr[ind++] = normal[5];
}


SkyBox::~SkyBox()
{
    delete this->vert_arr;
    delete this->col_arr;
    delete this->norm_arr;
    delete this->tex_arr;
}

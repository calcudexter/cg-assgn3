#include "bike.hpp"

#define cosR(x) cos(glm::radians(GLfloat(x)))
#define sinR(x) sin(glm::radians(GLfloat(x)))

Bike::Bike(float* param)
{
    int i = 0;
    this->wheel_r = param[i]; i++;
    this->wheel_t = param[i]; i++;
    this->body_w = param[i]; i++;
    this->body_h = param[i]; i++;
    this->body_t = param[i]; i++;
    this->rod_r = param[i]; i++;
    this->rod_l = param[i]; i++;
    this->rod_hinge_l = param[i]; i++;
    this->rod_body_angle = param[i]; i++;

    this->rear_rot = 0.0f;
    this->front_rot = 0.0f;
    this->rod_rot = 0.0f;
    this->initialize_hnode();
}

void Bike::render_bike()
{
  this->body->render_tree();
}

void Bike::update_bike(float* param)
{ 
  int i = 0;
  this->rear_rot = param[i];i++;
  this->front_rot = param[i];i++;
  this->rod_rot = param[i];

  // constraints
  if(this->rod_rot < -90.0f || this->rod_rot > 90.0f)
  {
    this->rod_rot = (this->rod_rot<0)?90.0f:-90.0f;
  }

  this->update_hnode();
}

void Bike::initialize_hnode()
{
  TexCuboid* body_s = new TexCuboid(this->body_w, this->body_h, this->body_t, glm::vec4(0.0f, 1.0f, 1.0f, 1.0f));
  Cylinder* rod_s = new Cylinder(this->rod_l, this->rod_r, NUM_T, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
  CylinderRim* rear_wheel_s = new CylinderRim(this->wheel_t, 0.7*this->wheel_r, this->wheel_r, NUM_T, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
  CylinderRim* front_wheel_s = new CylinderRim(this->wheel_t, 0.7*this->wheel_r, this->wheel_r, NUM_T, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
  Cylinder* spoke = new Cylinder(0.7*this->wheel_r, 0.2f, NUM_T, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
  Cylinder* steering_s = new Cylinder(this->rod_l/2, 0.7*this->rod_r, NUM_T, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

  this->body = new HNode(NULL, body_s->num_vertices, body_s->vert_arr, body_s->tex_arr, body_s->norm_arr, body_s->num_vertices*sizeof(glm::vec4), body_s->num_vertices*sizeof(glm::vec2), body_s->num_vertices*sizeof(glm::vec4), "Body", "darth.bmp", 256, 256);

  this->rod = new HNode(this->body, rod_s->num_vertices, rod_s->vert_arr, rod_s->col_arr, rod_s->norm_arr, rod_s->num_vertices*sizeof(glm::vec4), rod_s->num_vertices*sizeof(glm::vec4), rod_s->num_vertices*sizeof(glm::vec4), "Rod");
  GLfloat delta = -1*(this->body_h/sin(this->rod_body_angle)-this->rod_l/2);
  this->rod->change_parameters(this->body_w/2+delta*cos(this->rod_body_angle), this->body_h/2-delta*sin(this->rod_body_angle), 0.0f, -90.0f, -(this->rod_body_angle), 90.0f+this->rod_rot);

  this->front_wheel = new HNode(this->rod, front_wheel_s->num_vertices, front_wheel_s->vert_arr, front_wheel_s->col_arr, front_wheel_s->norm_arr, front_wheel_s->num_vertices*sizeof(glm::vec4), front_wheel_s->num_vertices*sizeof(glm::vec4), front_wheel_s->num_vertices*sizeof(glm::vec4), "FrontWheel");
  this->front_wheel->change_parameters(0.0f, 0.0f, -this->rod_l/2, 0.0f, 90.0f, this->front_rot);

  this->rear_wheel = new HNode(this->body, rear_wheel_s->num_vertices, rear_wheel_s->vert_arr, rear_wheel_s->col_arr, rear_wheel_s->norm_arr, rear_wheel_s->num_vertices*sizeof(glm::vec4), rear_wheel_s->num_vertices*sizeof(glm::vec4), rear_wheel_s->num_vertices*sizeof(glm::vec4), "RearWheel");
  this->rear_wheel->change_parameters(-this->body_w/2, -this->body_h/2, 0.0f, 0.0f, 0.0f, this->rear_rot);
  
  glm::vec3 rots[20];
  for(int i = 0; i < 20; i++) {
    glm::mat4 mat = glm::rotate(glm::mat4(1.0f), glm::radians(18.0f*i), glm::vec3(0.0f, 0.0f, 1.0f))
                    * glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    GLfloat X = atan2(mat[2][1], mat[2][2]), Y = atan2(-mat[2][0], sqrt(mat[2][1]*mat[2][1] + mat[2][2]*mat[2][2]));
		GLfloat Z = atan2(mat[1][0], mat[0][0]);

    rots[i] = glm::vec3(X, Y, Z)/GLfloat(M_PI)*180.0f;
  }


  for(int i = 0; i < 20; i++)
  {
    this->front_spoke[i] = new HNode(this->front_wheel, spoke->num_vertices, spoke->vert_arr, spoke->col_arr, spoke->norm_arr, spoke->num_vertices*sizeof(glm::vec4), spoke->num_vertices*sizeof(glm::vec4), spoke->num_vertices*sizeof(glm::vec4), "FrontSpoke"+std::to_string(i));
    
    this->front_spoke[i]->change_parameters(0.35*this->wheel_r*cosR(90+18.0f*i), 0.35*this->wheel_r*sinR(90+18.0f*i), 0.0f, rots[i][0], rots[i][1], rots[i][2]);

    this->rear_spoke[i] = new HNode(this->rear_wheel, spoke->num_vertices, spoke->vert_arr, spoke->col_arr, spoke->norm_arr, spoke->num_vertices*sizeof(glm::vec4), spoke->num_vertices*sizeof(glm::vec4), spoke->num_vertices*sizeof(glm::vec4), "RearSpoke"+std::to_string(i));
    this->rear_spoke[i]->change_parameters(0.35*this->wheel_r*cosR(90+18.0f*i), 0.35*this->wheel_r*sinR(90+18.0f*i), 0.0f, rots[i][0], rots[i][1], rots[i][2]);
  }
  
  this->steering = new HNode(this->rod, steering_s->num_vertices, steering_s->vert_arr, steering_s->col_arr, steering_s->norm_arr, steering_s->num_vertices*sizeof(glm::vec4), steering_s->num_vertices*sizeof(glm::vec4), steering_s->num_vertices*sizeof(glm::vec4), "Steering");
  this->steering->change_parameters(0.0f, 0.0f, 0.9*this->rod_l/2, 0.0f, 90.0f, 0.0f);

  return;
}

void Bike::update_hnode()
{
  GLfloat delta = -1*(this->body_h/sin(this->rod_body_angle)-this->rod_l/2);
  this->rod->change_parameters(this->body_w/2+delta*cos(this->rod_body_angle), this->body_h/2-delta*sin(this->rod_body_angle), 0.0f, -90.0f, -(this->rod_body_angle), 90.0f+this->rod_rot);
  this->front_wheel->change_parameters(0.0f, 0.0f, -this->rod_l/2, 0.0f, 90.0f, this->front_rot);
  this->rear_wheel->change_parameters(-this->body_w/2, -this->body_h/2, 0.0f, 0.0f, 0.0f, this->rear_rot);
}

void Bike::print_rot()
{
  this->body->print_rot();
}

void Bike::init_arrange()
{
  this->body->init_rot(1.570796, 0.0, 0.0);
}
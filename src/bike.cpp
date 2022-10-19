#include "bike.hpp"

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
    // std::cout<<"Parameters read"<<std::endl;
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
  // std::cout<<"rod_rot" << (this->rod_rot)<<std::endl;

  // constraints
  if(this->rod_rot < -90.0f || this->rod_rot > 90.0f)
  {
    this->rod_rot = (this->rod_rot<0)?90.0f:-90.0f;
  }

  this->update_hnode();
}

void Bike::initialize_hnode()
{
  Cuboid* body_s = new Cuboid(this->body_w, this->body_h, this->body_t, glm::vec4(0.0f, 1.0f, 1.0f, 1.0f));
  Cylinder* rod_s = new Cylinder(this->rod_l, this->rod_r, NUM_T, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
  Cylinder* rear_wheel_s = new Cylinder(this->wheel_t, this->wheel_r, NUM_T, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
  Cylinder* front_wheel_s = new Cylinder(this->wheel_t, this->wheel_r, NUM_T, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
  Cylinder* front_dot_s = new Cylinder(this->wheel_t+0.1f, this->wheel_r/4, NUM_T, glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));
  Cylinder* rear_dot_s = new Cylinder(this->wheel_t+0.1f, this->wheel_r/4, NUM_T, glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));

  this->body = new HNode(NULL, body_s->num_vertices, body_s->vert_arr, body_s->col_arr, body_s->num_vertices*sizeof(glm::vec4), body_s->num_vertices*sizeof(glm::vec4));
  // this->body->change_parameters();

  this->rod = new HNode(this->body, rod_s->num_vertices, rod_s->vert_arr, rod_s->col_arr, rod_s->num_vertices*sizeof(glm::vec4), rod_s->num_vertices*sizeof(glm::vec4));
  this->rod->change_parameters(this->body_w/2, this->body_h/2, 0.0f, -90.0f, -(this->rod_body_angle), 90.0f+this->rod_rot);

  this->front_wheel = new HNode(this->rod, front_wheel_s->num_vertices, front_wheel_s->vert_arr, front_wheel_s->col_arr, front_wheel_s->num_vertices*sizeof(glm::vec4), front_wheel_s->num_vertices*sizeof(glm::vec4));
  this->front_wheel->change_parameters(0.0f, 0.0f, -this->rod_l/2, 0.0f, 90.0f, this->front_rot);

  this->rear_wheel = new HNode(this->body, rear_wheel_s->num_vertices, rear_wheel_s->vert_arr, rear_wheel_s->col_arr, rear_wheel_s->num_vertices*sizeof(glm::vec4), rear_wheel_s->num_vertices*sizeof(glm::vec4));
  this->rear_wheel->change_parameters(-this->body_w/2, -this->body_h/2, 0.0f, 0.0f, 0.0f, this->rear_rot);
  
  this->rear_wheel_dot = new HNode(this->rear_wheel, rear_dot_s->num_vertices, rear_dot_s->vert_arr, rear_dot_s->col_arr, rear_dot_s->num_vertices*sizeof(glm::vec4), rear_dot_s->num_vertices*sizeof(glm::vec4));
  this->rear_wheel_dot->change_parameters(-this->wheel_r/2, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
  
  this->front_wheel_dot = new HNode(this->front_wheel, front_dot_s->num_vertices, front_dot_s->vert_arr, front_dot_s->col_arr, front_dot_s->num_vertices*sizeof(glm::vec4), front_dot_s->num_vertices*sizeof(glm::vec4));
  this->front_wheel_dot->change_parameters(this->wheel_r/2, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
  return;
}

void Bike::update_hnode()
{
  this->rod->change_parameters(this->body_w/2, this->body_h/2, 0.0f, -90.0f, -(this->rod_body_angle), 90.0f+this->rod_rot);
  this->front_wheel->change_parameters(0.0f, 0.0f, -this->rod_l/2, 0.0f, 90.0f, this->front_rot);
  this->rear_wheel->change_parameters(-this->body_w/2, -this->body_h/2, 0.0f, 0.0f, 0.0f, this->rear_rot);
}

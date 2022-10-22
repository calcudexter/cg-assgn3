#include "track.hpp"

Track::Track(float* param)
{
    // float track_w, track_l, ramp_l, ramp_h, double_ramp_w;
    // float inner_r;
    // float scale;

    int i = 0;
    this->scale = param[i]; i++;
    this->track_t = param[i]*this->scale; i++;
    this->track_l = param[i]*this->scale; i++;
    this->ramp_l = param[i]*this->scale; i++;
    this->ramp_h = param[i]*this->scale; i++;
    this->double_ramp_w = param[i]; i++;
    this->inner_r = param[i]*this->scale;
    this->initialize_hnode();
}

void Track::render_track()
{
  this->plane1->render_tree();
}

void Track::update_track(float* param)
{ 
  int i = 0;
  this->track_t = param[i]*this->scale; i++;
  this->track_l = param[i]*this->scale; i++;
  this->ramp_l = param[i]*this->scale; i++;
  this->ramp_h = param[i]*this->scale; i++;
  this->double_ramp_w = param[i]*this->scale;
  this->update_hnode();
}

void Track::initialize_hnode()
{
  glm::vec4 tmp_color(0.62f, 0.164f, 0.164f, 1.0f);
  
  Track_plane plane1_s(this->track_t, this->track_l, tmp_color);
  Track_plane plane2_s(this->track_t, this->track_l, tmp_color);
  Track_curve curve1_s(this->inner_r, this->inner_r+this->track_t, NUM_T, tmp_color);
  Track_curve curve2_s(this->inner_r, this->inner_r+this->track_t, NUM_T, tmp_color);
  tmp_color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
  
  Track_ramp ramp1_s(this->ramp_l, this->track_t, this->ramp_h, tmp_color);
  Track_ramp double_ramp1_s(this->ramp_l, this->double_ramp_w, this->ramp_h, tmp_color);
  Track_ramp double_ramp2_s(this->ramp_l, this->double_ramp_w, this->ramp_h, tmp_color);

  this->plane1 = new HNode(NULL, plane1_s.num_vertices, plane1_s.vert_arr, plane1_s.col_arr, plane1_s.num_vertices*sizeof(glm::vec4), plane1_s.num_vertices*sizeof(glm::vec4), "Plane1");
  //this->plane1->change_parameters();
  
  this->plane2 = new HNode(this->plane1, plane2_s.num_vertices, plane2_s.vert_arr, plane2_s.col_arr, plane2_s.num_vertices*sizeof(glm::vec4), plane2_s.num_vertices*sizeof(glm::vec4), "Plane2");
  this->plane2->change_parameters(0.0f, 2*this->inner_r+this->track_t, 0.0f, 0.0f, 0.0f, 0.0f);
  
  this->curve1 = new HNode(this->plane1, curve1_s.num_vertices, curve1_s.vert_arr, curve1_s.col_arr, curve1_s.num_vertices*sizeof(glm::vec4), curve1_s.num_vertices*sizeof(glm::vec4), "Curve1");
  this->curve1->change_parameters(this->track_l/2, this->inner_r+this->track_t/2, 0.0f, 0.0f, 0.0f, -90.0f);

  this->curve2 = new HNode(this->plane1, curve2_s.num_vertices, curve2_s.vert_arr, curve2_s.col_arr, curve2_s.num_vertices*sizeof(glm::vec4), curve2_s.num_vertices*sizeof(glm::vec4), "Curve2");
  this->curve2->change_parameters(-this->track_l/2, this->inner_r+this->track_t/2, 0.0f, 0.0f, 0.0f, 90.0f);

  
  this->ramp = new HNode(this->plane1, ramp1_s.num_vertices, ramp1_s.vert_arr, ramp1_s.col_arr, ramp1_s.num_vertices*sizeof(glm::vec4), ramp1_s.num_vertices*sizeof(glm::vec4), "Ramp1");
  this->ramp->change_parameters(0.0f, 2*this->inner_r+this->track_t/2, 0.0f, 0.0f, 0.0f, 0.0f);
  
  this->double_ramp1 = new HNode(this->plane1, double_ramp1_s.num_vertices, double_ramp1_s.vert_arr, double_ramp1_s.col_arr, double_ramp1_s.num_vertices*sizeof(glm::vec4), double_ramp1_s.num_vertices*sizeof(glm::vec4), "DRamp1");
  this->double_ramp1->change_parameters(0.0f, -this->track_t/2+this->double_ramp_w, 0.0f, 0.0f, 0.0f, 180.0f);
  
  this->double_ramp2 = new HNode(this->plane1, double_ramp2_s.num_vertices, double_ramp2_s.vert_arr, double_ramp2_s.col_arr, double_ramp2_s.num_vertices*sizeof(glm::vec4), double_ramp2_s.num_vertices*sizeof(glm::vec4), "DRamp2");
  this->double_ramp2->change_parameters(0.0f, this->track_t/2, 0.0f, 0.0f, 0.0f, 180.0f);


  return;
}

void Track::update_hnode()
{
  ;
}

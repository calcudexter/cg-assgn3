#include "track.hpp"

#define cosR(x) cos(glm::radians(GLfloat(x)))
#define sinR(x) sin(glm::radians(GLfloat(x)))

Track::Track(float* param)
{
  int i = 0;
  this->scale = param[i]; i++;
  this->track_t = param[i]*this->scale; i++;
  this->track_l = param[i]*this->scale; i++;
  this->ramp_l = param[i]*this->scale; i++;
  this->ramp_h = param[i]*this->scale; i++;
  this->double_ramp_w = param[i]*this->scale; i++;
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
  this->double_ramp_w = param[i]*this->scale; i++;
  this->inner_r = param[i]*this->scale;
  this->update_hnode();
}

void Track::initialize_hnode()
{

  // SkyBox
  SkyBox sb(50, 50, 50, glm::vec4(1.0,1.0,1.0,1.0));

  float tl = this->track_l, r = this->inner_r + this->track_t/2;
  float del = (tl*cosR(36) + 2*r*sinR(36));
  float del2 = tl*(1+cosR(72)) + 4*r*cosR(36)*cosR(36);
  
  glm::vec4 tmp_color(0.62f, 0.164f, 0.164f, 1.0f);
  
  Track_plane plane1_s(this->track_t, this->track_l, tmp_color);
  Track_curve curve1_s(this->inner_r, this->inner_r+this->track_t, NUM_T, 72, tmp_color);
  tmp_color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
  
  Track_ramp ramp1_s(this->ramp_l, this->track_t, this->ramp_h, tmp_color);
  Track_ramp double_ramp1_s(this->ramp_l, this->double_ramp_w, this->ramp_h, tmp_color);
  Track_ramp double_ramp2_s(this->ramp_l, this->double_ramp_w, this->ramp_h, tmp_color);

  Track_ramp bump1_s(this->ramp_l/3, this->track_t, this->ramp_h/2, tmp_color);

  // this->plane1 = new HNode(NULL, plane1_s.num_vertices, plane1_s.vert_arr, plane1_s.col_arr, plane1_s.norm_arr, plane1_s.num_vertices*sizeof(glm::vec4), plane1_s.num_vertices*sizeof(glm::vec4), plane1_s.num_vertices*sizeof(glm::vec4), "Plane1");

  this->plane1 = new HNode(NULL, plane1_s.num_vertices, plane1_s.vert_arr, plane1_s.tex_arr, plane1_s.norm_arr, plane1_s.num_vertices*sizeof(glm::vec4), plane1_s.num_vertices*sizeof(glm::vec2), plane1_s.num_vertices*sizeof(glm::vec4), "Plane1", "road.bmp", 800, 800);
  
  this->skybox = new HNode(this->plane1, sb.num_vertices, sb.vert_arr, sb.tex_arr, sb.norm_arr, sb.num_vertices*sizeof(glm::vec4), sb.num_vertices*sizeof(glm::vec2), sb.num_vertices*sizeof(glm::vec4), "SkyBox", "skybox.bmp", 1024, 768);
  this->skybox->change_parameters(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);

  // this->plane2 = new HNode(this->plane1, plane1_s.num_vertices, plane1_s.vert_arr, plane1_s.col_arr, plane1_s.norm_arr, plane1_s.num_vertices*sizeof(glm::vec4), plane1_s.num_vertices*sizeof(glm::vec4), plane1_s.num_vertices*sizeof(glm::vec4), "Plane2");

  this->plane2 = new HNode(this->plane1, plane1_s.num_vertices, plane1_s.vert_arr, plane1_s.tex_arr, plane1_s.norm_arr, plane1_s.num_vertices*sizeof(glm::vec4), plane1_s.num_vertices*sizeof(glm::vec2), plane1_s.num_vertices*sizeof(glm::vec4), "Plane2", "road.bmp", 800, 800);
  this->plane2->change_parameters(del*cosR(36), del*sinR(36), 0.0f, 0.0f, 0.0f, 72.0f);

  // this->plane3 = new HNode(this->plane2, plane1_s.num_vertices, plane1_s.vert_arr, plane1_s.col_arr, plane1_s.norm_arr, plane1_s.num_vertices*sizeof(glm::vec4), plane1_s.num_vertices*sizeof(glm::vec4), plane1_s.num_vertices*sizeof(glm::vec4), "Plane3");

  this->plane3 = new HNode(this->plane2, plane1_s.num_vertices, plane1_s.vert_arr, plane1_s.tex_arr, plane1_s.norm_arr, plane1_s.num_vertices*sizeof(glm::vec4), plane1_s.num_vertices*sizeof(glm::vec2), plane1_s.num_vertices*sizeof(glm::vec4), "Plane3", "road.bmp", 800, 800);
  this->plane3->change_parameters(del*cosR(36), del*sinR(36), 0.0f, 0.0f, 0.0f, 72.0f);

  // this->plane4 = new HNode(this->plane3, plane1_s.num_vertices, plane1_s.vert_arr, plane1_s.col_arr, plane1_s.norm_arr, plane1_s.num_vertices*sizeof(glm::vec4), plane1_s.num_vertices*sizeof(glm::vec4), plane1_s.num_vertices*sizeof(glm::vec4), "Plane4");

  this->plane4 = new HNode(this->plane3, plane1_s.num_vertices, plane1_s.vert_arr, plane1_s.tex_arr, plane1_s.norm_arr, plane1_s.num_vertices*sizeof(glm::vec4), plane1_s.num_vertices*sizeof(glm::vec2), plane1_s.num_vertices*sizeof(glm::vec4), "Plane4", "road.bmp", 800, 800);
  this->plane4->change_parameters(del*cosR(36), del*sinR(36), 0.0f, 0.0f, 0.0f, 72.0f);

  // this->plane5 = new HNode(this->plane4, plane1_s.num_vertices, plane1_s.vert_arr, plane1_s.col_arr, plane1_s.norm_arr, plane1_s.num_vertices*sizeof(glm::vec4), plane1_s.num_vertices*sizeof(glm::vec4), plane1_s.num_vertices*sizeof(glm::vec4), "Plane5");

  this->plane5 = new HNode(this->plane4, plane1_s.num_vertices, plane1_s.vert_arr, plane1_s.tex_arr, plane1_s.norm_arr, plane1_s.num_vertices*sizeof(glm::vec4), plane1_s.num_vertices*sizeof(glm::vec2), plane1_s.num_vertices*sizeof(glm::vec4), "Plane5", "road.bmp", 800, 800);
  this->plane5->change_parameters(del*cosR(36), del*sinR(36), 0.0f, 0.0f, 0.0f, 72.0f);
  

  // Curves
  this->curve1 = new HNode(this->plane1, curve1_s.num_vertices, curve1_s.vert_arr, curve1_s.col_arr, curve1_s.norm_arr, curve1_s.num_vertices*sizeof(glm::vec4), curve1_s.num_vertices*sizeof(glm::vec4), curve1_s.num_vertices*sizeof(glm::vec4), "Curve1");
  this->curve1->change_parameters(this->track_l/2, this->inner_r+this->track_t/2, 0.0f, 0.0f, 0.0f, -90.0f);

  this->curve2 = new HNode(this->plane2, curve1_s.num_vertices, curve1_s.vert_arr, curve1_s.col_arr, curve1_s.norm_arr, curve1_s.num_vertices*sizeof(glm::vec4), curve1_s.num_vertices*sizeof(glm::vec4), curve1_s.num_vertices*sizeof(glm::vec4), "Curve2");
  this->curve2->change_parameters(this->track_l/2, this->inner_r+this->track_t/2, 0.0f, 0.0f, 0.0f, -90.0f);

  this->curve3 = new HNode(this->plane3, curve1_s.num_vertices, curve1_s.vert_arr, curve1_s.col_arr, curve1_s.norm_arr, curve1_s.num_vertices*sizeof(glm::vec4), curve1_s.num_vertices*sizeof(glm::vec4), curve1_s.num_vertices*sizeof(glm::vec4), "Curve3");
  this->curve3->change_parameters(this->track_l/2, this->inner_r+this->track_t/2, 0.0f, 0.0f, 0.0f, -90.0f);

  this->curve4 = new HNode(this->plane4, curve1_s.num_vertices, curve1_s.vert_arr, curve1_s.col_arr, curve1_s.norm_arr, curve1_s.num_vertices*sizeof(glm::vec4), curve1_s.num_vertices*sizeof(glm::vec4), curve1_s.num_vertices*sizeof(glm::vec4), "Curve4");
  this->curve4->change_parameters(this->track_l/2, this->inner_r+this->track_t/2, 0.0f, 0.0f, 0.0f, -90.0f);

  this->curve5 = new HNode(this->plane5, curve1_s.num_vertices, curve1_s.vert_arr, curve1_s.col_arr, curve1_s.norm_arr, curve1_s.num_vertices*sizeof(glm::vec4), curve1_s.num_vertices*sizeof(glm::vec4), curve1_s.num_vertices*sizeof(glm::vec4), "Curve5");
  this->curve5->change_parameters(this->track_l/2, this->inner_r+this->track_t/2, 0.0f, 0.0f, 0.0f, -90.0f);

  // Obstacles
  this->ramp = new HNode(this->plane1, ramp1_s.num_vertices, ramp1_s.vert_arr, ramp1_s.col_arr, ramp1_s.norm_arr, ramp1_s.num_vertices*sizeof(glm::vec4), ramp1_s.num_vertices*sizeof(glm::vec4), ramp1_s.num_vertices*sizeof(glm::vec4), "Ramp1");
  this->ramp->change_parameters(0.0f, -this->track_t/2, 0.0f, 0.0f, 0.0f, 0.0f);
  
  this->double_ramp1 = new HNode(this->plane2, double_ramp1_s.num_vertices, double_ramp1_s.vert_arr, double_ramp1_s.col_arr, double_ramp1_s.norm_arr, double_ramp1_s.num_vertices*sizeof(glm::vec4), double_ramp1_s.num_vertices*sizeof(glm::vec4), double_ramp1_s.num_vertices*sizeof(glm::vec4), "DRamp1");
  this->double_ramp1->change_parameters(0.0f, -this->track_t/2, 0.0f, 0.0f, 0.0f, 0.0f);
  
  this->double_ramp2 = new HNode(this->plane2, double_ramp2_s.num_vertices, double_ramp2_s.vert_arr, double_ramp2_s.col_arr, double_ramp2_s.norm_arr, double_ramp2_s.num_vertices*sizeof(glm::vec4), double_ramp2_s.num_vertices*sizeof(glm::vec4), double_ramp2_s.num_vertices*sizeof(glm::vec4), "DRamp2");
  this->double_ramp2->change_parameters(0.0f, this->track_t/2-this->double_ramp_w, 0.0f, 0.0f, 0.0f, 0.0f);

  this->two_ramp1 = new HNode(this->plane3, ramp1_s.num_vertices, ramp1_s.vert_arr, ramp1_s.col_arr, ramp1_s.norm_arr, ramp1_s.num_vertices*sizeof(glm::vec4), ramp1_s.num_vertices*sizeof(glm::vec4), ramp1_s.num_vertices*sizeof(glm::vec4), "TwoRamp1");
  this->two_ramp1->change_parameters(-this->ramp_l, -this->track_t/2, 0.0f, 0.0f, 0.0f, 0.0f);

  this->two_ramp2 = new HNode(this->plane3, ramp1_s.num_vertices, ramp1_s.vert_arr, ramp1_s.col_arr, ramp1_s.norm_arr, ramp1_s.num_vertices*sizeof(glm::vec4), ramp1_s.num_vertices*sizeof(glm::vec4), ramp1_s.num_vertices*sizeof(glm::vec4), "TwoRamp2");
  this->two_ramp2->change_parameters(this->ramp_l, this->track_t/2, 0.0f, 0.0f, 0.0f, 180.0f);


  this->bump1[0] = new HNode(this->plane4, bump1_s.num_vertices, bump1_s.vert_arr, bump1_s.col_arr, bump1_s.norm_arr, bump1_s.num_vertices*sizeof(glm::vec4), bump1_s.num_vertices*sizeof(glm::vec4), bump1_s.num_vertices*sizeof(glm::vec4), "Bump10");
  this->bump1[0]->change_parameters(-4*this->ramp_l/3, -this->track_t/2, 0.0f, 0.0f, 0.0f, 0.0f);

  this->bump2[0] = new HNode(this->plane4, bump1_s.num_vertices, bump1_s.vert_arr, bump1_s.col_arr, bump1_s.norm_arr, bump1_s.num_vertices*sizeof(glm::vec4), bump1_s.num_vertices*sizeof(glm::vec4), bump1_s.num_vertices*sizeof(glm::vec4), "Bump20");
  this->bump2[0]->change_parameters(-2*this->ramp_l/3, this->track_t/2, 0.0f, 0.0f, 0.0f, 180.0f);

  this->bump1[1] = new HNode(this->bump1[0], bump1_s.num_vertices, bump1_s.vert_arr, bump1_s.col_arr, bump1_s.norm_arr, bump1_s.num_vertices*sizeof(glm::vec4), bump1_s.num_vertices*sizeof(glm::vec4), bump1_s.num_vertices*sizeof(glm::vec4), "Bump11");
  this->bump1[1]->change_parameters(2*this->ramp_l/3, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);

  this->bump2[1] = new HNode(this->bump2[0], bump1_s.num_vertices, bump1_s.vert_arr, bump1_s.col_arr, bump1_s.norm_arr, bump1_s.num_vertices*sizeof(glm::vec4), bump1_s.num_vertices*sizeof(glm::vec4), bump1_s.num_vertices*sizeof(glm::vec4), "Bump21");
  this->bump2[1]->change_parameters(-2*this->ramp_l/3, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);

  this->bump1[2] = new HNode(this->bump1[1], bump1_s.num_vertices, bump1_s.vert_arr, bump1_s.col_arr, bump1_s.norm_arr, bump1_s.num_vertices*sizeof(glm::vec4), bump1_s.num_vertices*sizeof(glm::vec4), bump1_s.num_vertices*sizeof(glm::vec4), "Bump12");
  this->bump1[2]->change_parameters(2*this->ramp_l/3, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);

  this->bump2[2] = new HNode(this->bump2[1], bump1_s.num_vertices, bump1_s.vert_arr, bump1_s.col_arr, bump1_s.norm_arr, bump1_s.num_vertices*sizeof(glm::vec4), bump1_s.num_vertices*sizeof(glm::vec4), bump1_s.num_vertices*sizeof(glm::vec4), "Bump22");
  this->bump2[2]->change_parameters(-2*this->ramp_l/3, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);

  this->bump1[3] = new HNode(this->bump1[2], bump1_s.num_vertices, bump1_s.vert_arr, bump1_s.col_arr, bump1_s.norm_arr, bump1_s.num_vertices*sizeof(glm::vec4), bump1_s.num_vertices*sizeof(glm::vec4), bump1_s.num_vertices*sizeof(glm::vec4), "Bump13");
  this->bump1[3]->change_parameters(2*this->ramp_l/3, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);

  this->bump2[3] = new HNode(this->bump2[2], bump1_s.num_vertices, bump1_s.vert_arr, bump1_s.col_arr, bump1_s.norm_arr, bump1_s.num_vertices*sizeof(glm::vec4), bump1_s.num_vertices*sizeof(glm::vec4), bump1_s.num_vertices*sizeof(glm::vec4), "Bump23");
  this->bump2[3]->change_parameters(-2*this->ramp_l/3, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
  return;
}

void Track::update_hnode()
{
  ;
}

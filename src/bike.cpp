#include <vector>
#include "gl_framework.hpp"
#include "shader_util.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "hierarchy_node.hpp"
#include "shapes.h"

#define NUM_T 1000

using namespace csX75;
//Running variable to toggle culling on/off
bool enable_culling=true;
//Running variable to toggle wireframe/solid modelling
bool solid=true;
//Enable/Disable perspective view
bool enable_perspective=false;
//Shader program attribs
GLuint vPosition,vColor;

//global matrix stack for hierarchical modelling
std::vector<glm::mat4> matrixStack;

GLuint shaderProgram;

glm::mat4 rotation_matrix;
glm::mat4 projection_matrix;
glm::mat4 c_rotation_matrix;
glm::mat4 lookat_matrix;

glm::mat4 model_matrix;
glm::mat4 view_matrix;


glm::mat4 modelview_matrix;

GLuint uModelViewMatrix;
const int num_vertices = 36;

class Bike
{
private:
    HNode* body;
    HNode* rear_wheel;
    HNode* front_wheel;
    HNode* rod;

    float wheel_r, wheel_t, body_w, body_h, body_t;
    float rod_r, rod_l; 
    float rod_hinge_l, rod_body_angle;
    float rear_rot, front_rot, rod_rot;


public:
    Bike(float* param);
    void render_bike();
    void update_bike(float* param);
    void initialize_hnode();
};

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
}

void Bike::render_bike()
{
  body->render_tree();
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
}

void Bike::initialize_hnode()
{
  Cuboid* body_s = new Cuboid(this->body_w, this->body_h, this->body_t);
  Cylinder* rod_s = new Cylinder(this->rod_l, this->rod_r, NUM_T);
  Cylinder* rear_wheel_s = new Cylinder(this->wheel_t, this->wheel_r, NUM_T);
  Cylinder* front_wheel_s = new Cylinder(this->wheel_t, this->wheel_r, NUM_T);
  
  
  return;
}





//-----------------------------------------------------------------

//Eight vertices in homogenous coordinates
//elongated cuboid , basic arm in the hierarchy;
glm::vec4 positions[8] = {
  glm::vec4(0.0, -0.25, 0.25, 1.0),
  glm::vec4(0.0, 0.25, 0.25, 1.0),
  glm::vec4(2.0, 0.25, 0.25, 1.0),
  glm::vec4(2.0, -0.25, 0.25, 1.0),
  glm::vec4(0.0, -0.25, -0.25, 1.0),
  glm::vec4(0.0, 0.25, -0.25, 1.0),
  glm::vec4(2.0, 0.25, -0.25, 1.0),
  glm::vec4(2.0, -0.25, -0.25, 1.0)
};

//RGBA colors
glm::vec4 colors[8] = {
  glm::vec4(0.0, 0.0, 0.0, 1.0),
  glm::vec4(1.0, 0.0, 0.0, 1.0),
  glm::vec4(1.0, 1.0, 0.0, 1.0),
  glm::vec4(0.0, 1.0, 0.0, 1.0),
  glm::vec4(0.0, 0.0, 1.0, 1.0),
  glm::vec4(1.0, 0.0, 1.0, 1.0),
  glm::vec4(1.0, 1.0, 1.0, 1.0),
  glm::vec4(0.0, 1.0, 1.0, 1.0)
};

int tri_idx=0;
glm::vec4 v_positions[num_vertices];
glm::vec4 v_colors[num_vertices];

// quad generates two triangles for each face and assigns colors to the vertices
void quad(int a, int b, int c, int d)
{
  v_colors[tri_idx] = colors[a]; v_positions[tri_idx] = positions[a]; tri_idx++;
  v_colors[tri_idx] = colors[b]; v_positions[tri_idx] = positions[b]; tri_idx++;
  v_colors[tri_idx] = colors[c]; v_positions[tri_idx] = positions[c]; tri_idx++;
  v_colors[tri_idx] = colors[a]; v_positions[tri_idx] = positions[a]; tri_idx++;
  v_colors[tri_idx] = colors[c]; v_positions[tri_idx] = positions[c]; tri_idx++;
  v_colors[tri_idx] = colors[d]; v_positions[tri_idx] = positions[d]; tri_idx++;
 }

// generate 12 triangles: 36 vertices and 36 colors
void colorcube(void)
{
    quad( 1, 0, 3, 2 );
    quad( 2, 3, 7, 6 );
    quad( 3, 0, 4, 7 );
    quad( 6, 5, 1, 2 );
    quad( 4, 5, 6, 7 );
    quad( 5, 4, 0, 1 );
}


//-----------------------------------------------------------------

void initBuffersGL(void)
{

  // Load shaders and use the resulting shader program
  std::string vertex_shader_file("07_vshader.glsl");
  std::string fragment_shader_file("07_fshader.glsl");

  std::vector<GLuint> shaderList;
  shaderList.push_back(csX75::LoadShaderGL(GL_VERTEX_SHADER, vertex_shader_file));
  shaderList.push_back(csX75::LoadShaderGL(GL_FRAGMENT_SHADER, fragment_shader_file));

  shaderProgram = csX75::CreateProgramGL(shaderList);
  glUseProgram( shaderProgram );

  // getting the attributes from the shader program
  vPosition = glGetAttribLocation( shaderProgram, "vPosition" );
  vColor = glGetAttribLocation( shaderProgram, "vColor" ); 
  uModelViewMatrix = glGetUniformLocation( shaderProgram, "uModelViewMatrix");

  // Creating the hierarchy:
  // We are using the original colorcube function to generate the vertices of the cuboid
  colorcube();

  //note that the buffers are initialized in the respective constructors
 
  node1 = new csX75::HNode(NULL,num_vertices,v_positions,v_colors,sizeof(v_positions),sizeof(v_colors));
  node2 = new csX75::HNode(node1,num_vertices,v_positions,v_colors,sizeof(v_positions),sizeof(v_colors));
  node2->change_parameters(2.0,0.0,0.0,0.0,0.0,0.0);
  node3 = new csX75::HNode(node2,num_vertices,v_positions,v_colors,sizeof(v_positions),sizeof(v_colors));
  node3->change_parameters(2.0,0.0,0.0,0.0,0.0,0.0);
  root_node = node1;
  curr_node = node3;

}

void renderGL(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  matrixStack.clear();

  //Creating the lookat and the up vectors for the camera
  c_rotation_matrix = glm::rotate(glm::mat4(1.0f), glm::radians(c_xrot), glm::vec3(1.0f,0.0f,0.0f));
  c_rotation_matrix = glm::rotate(c_rotation_matrix, glm::radians(c_yrot), glm::vec3(0.0f,1.0f,0.0f));
  c_rotation_matrix = glm::rotate(c_rotation_matrix, glm::radians(c_zrot), glm::vec3(0.0f,0.0f,1.0f));

  glm::vec4 c_pos = glm::vec4(c_xpos,c_ypos,c_zpos, 1.0)*c_rotation_matrix;
  glm::vec4 c_up = glm::vec4(c_up_x,c_up_y,c_up_z, 1.0)*c_rotation_matrix;
  //Creating the lookat matrix
  lookat_matrix = glm::lookAt(glm::vec3(c_pos),glm::vec3(0.0),glm::vec3(c_up));

  //creating the projection matrix
  if(enable_perspective)
    projection_matrix = glm::frustum(-7.0, 7.0, -7.0, 7.0, 1.0, 7.0);
    //projection_matrix = glm::perspective(glm::radians(90.0),1.0,0.1,5.0);
  else
    projection_matrix = glm::ortho(-7.0, 7.0, -7.0, 7.0, -5.0, 5.0);

  view_matrix = projection_matrix*lookat_matrix;

  matrixStack.push_back(view_matrix);

  node1->render_tree();

}

int main(int argc, char** argv)
{
  //! The pointer to the GLFW window
  GLFWwindow* window;

  //! Setting up the GLFW Error callback
  glfwSetErrorCallback(csX75::error_callback);

  //! Initialize GLFW
  if (!glfwInit())
    return -1;

  //We want OpenGL 4.0
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  //This is for MacOSX - can be omitted otherwise
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); 
  //We don't want the old OpenGL 
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 

  //! Create a windowed mode window and its OpenGL context
  window = glfwCreateWindow(512, 512, "CS475/CS675 Tutorial 7: Hierarchical Modelling", NULL, NULL);
  if (!window)
    {
      glfwTerminate();
      return -1;
    }
  
  //! Make the window's context current 
  glfwMakeContextCurrent(window);

  //Initialize GLEW
  //Turn this on to get Shader based OpenGL
  glewExperimental = GL_TRUE;
  GLenum err = glewInit();
  if (GLEW_OK != err)
    {
      //Problem: glewInit failed, something is seriously wrong.
      std::cerr<<"GLEW Init Failed : %s"<<std::endl;
    }

  //Print and see what context got enabled
  std::cout<<"Vendor: "<<glGetString (GL_VENDOR)<<std::endl;
  std::cout<<"Renderer: "<<glGetString (GL_RENDERER)<<std::endl;
  std::cout<<"Version: "<<glGetString (GL_VERSION)<<std::endl;
  std::cout<<"GLSL Version: "<<glGetString (GL_SHADING_LANGUAGE_VERSION)<<std::endl;

  //Keyboard Callback
  glfwSetKeyCallback(window, csX75::key_callback);
  //Framebuffer resize callback
  glfwSetFramebufferSizeCallback(window, csX75::framebuffer_size_callback);

  // Ensure we can capture the escape key being pressed below
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

  //Initialize GL state
  csX75::initGL();
  initBuffersGL();

  // Loop until the user closes the window
  while (glfwWindowShouldClose(window) == 0)
    {
       
      // Render here
      renderGL();

      // Swap front and back buffers
      glfwSwapBuffers(window);
      
      // Poll for and process events
      glfwPollEvents();
    }
  
  glfwTerminate();
  return 0;
}

//-------------------------------------------------------------------------


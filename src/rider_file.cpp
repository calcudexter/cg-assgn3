#include "07_hierarchical_modelling.hpp"

GLuint shaderProgram;

glm::mat4 rotation_matrix;
glm::mat4 projection_matrix;
glm::mat4 c_rotation_matrix;
glm::mat4 lookat_matrix;
glm::mat4 model_matrix;
glm::mat4 view_matrix;
glm::mat4 modelview_matrix;
GLuint uModelViewMatrix;

Human* h;
Bike* b;
Track* t;

bool bike = false, rider = true, track = false;
int selected;

float bike_params[] = 
{
  3.0f, //wheel radius
  0.8f, //wheel thickness
  10.0f, //body width
  4.0f, //body height
  1.0f, //body thickness
  1.0f, //rod radius
  10.0f, //rod length
  1.0f, // rod hinge length
  45.0f // rod body angle
};

float* dof_param;

float track_params[] =
{
  0.16f, // scale
  5.0f, // track thickness
  30.0f, // track length
  5.0f, // ramp length
  2.0f, // ramp height
  2.0f, // double ramp width
  3.0f // inner radius of curves
};


void initBuffersGL(void)
{

  // Load shaders and use the resulting shader program
  std::string vertex_shader_file("../src/07_vshader.glsl");
  std::string fragment_shader_file("../src/07_fshader.glsl");

  std::vector<GLuint> shaderList;
  shaderList.push_back(csX75::LoadShaderGL(GL_VERTEX_SHADER, vertex_shader_file));
  shaderList.push_back(csX75::LoadShaderGL(GL_FRAGMENT_SHADER, fragment_shader_file));

  shaderProgram = csX75::CreateProgramGL(shaderList);
  glUseProgram( shaderProgram );

  // getting the attributes from the shader program
  vPosition = glGetAttribLocation( shaderProgram, "vPosition" );
  vColor = glGetAttribLocation( shaderProgram, "vColor" ); 
  uModelViewMatrix = glGetUniformLocation( shaderProgram, "uModelViewMatrix");

  //note that the buffers are initialized in the respective constructors
  h = new Human();
  if(rider && !bike) curr_node = h->torso;
  b = new Bike(bike_params);
  if(bike && !rider) curr_node = b->body;
  t = new Track(track_params);

  curr_node = h->torso;
  selected = 1;
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
    projection_matrix = glm::ortho(-20.0, 20.0, -20.0, 20.0, -500.0, 500.0);

  view_matrix = projection_matrix*lookat_matrix;

  matrixStack.push_back(view_matrix);
  
  if(rider) {
    matrixStack.push_back(glm::translate(glm::mat4(1.0f), glm::vec3(gtx[1], gty[1], gtz[1])));
    matrixStack.push_back(glm::scale(glm::mat4(1.0f), scaling[1] * glm::vec3(1.0f, 1.0f, 1.0f)));
    h->torso->render_tree();
    matrixStack.pop_back();
    matrixStack.pop_back();
  }
  if(bike) {
    matrixStack.push_back(glm::translate(glm::mat4(1.0f), glm::vec3(gtx[0], gty[0], gtz[0])));
    matrixStack.push_back(glm::scale(glm::mat4(1.0f), scaling[0] * glm::vec3(1.0f, 1.0f, 1.0f)));
    b->update_bike(dof_param);
    b->render_bike();
    matrixStack.pop_back();
    matrixStack.pop_back();
  }
  if(track) {
    matrixStack.push_back(glm::translate(glm::mat4(1.0f), glm::vec3(gtx[2], gty[2], gtz[2])));
    matrixStack.push_back(glm::scale(glm::mat4(1.0f), scaling[2] * glm::vec3(1.0f, 1.0f, 1.0f)));
    t->render_track();
    matrixStack.pop_back();
    matrixStack.pop_back();
  }
}

HNode* getNode(char key)
{
  switch(key)
  {
    case 'a':
      printf("Returning Torso\n");
      return h->torso;
      
    case 'b':
      printf("Returning Left Upper Arm\n");
      return h->left_arm[0];
      
    case 'c':
      printf("Returning Left Lower Arm\n");
      return h->left_arm[1];
      
    case 'd':
      printf("Returning Right Upper Arm\n");
      return h->right_arm[0];
      
    case 'e':
      printf("Returning Right Lower Arm\n");
      return h->right_arm[1];
      
    case 'f':
      printf("Returning Left Upper Leg\n");
      return h->left_leg[0];
      
    case 'g':
      printf("Returning Left Lower Leg\n");
      return h->left_leg[1];
      
    case 'h':
      printf("Returning Right Upper Leg\n");
      return h->right_leg[0];
      
    case 'i':
      printf("Returning Right Lower Leg\n");
      return h->right_leg[1];
      
    case 'j':
      printf("Returning Neck\n");
      return h->neck;
      
    case 'k':
      printf("Returning Head\n");
      return h->head;
    
    default:
      return h->torso;
  }
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
  window = glfwCreateWindow(512, 512, "CS475 Assignment 2 Rider", NULL, NULL);
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

  dof_param = (float*) malloc(3*sizeof(float));

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


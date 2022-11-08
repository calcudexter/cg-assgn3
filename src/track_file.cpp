#include "07_hierarchical_modelling.hpp"

GLuint worldShaderProgram, lightShaderProgram;

glm::mat4 rotation_matrix;
glm::mat4 projection_matrix;
glm::mat4 c_rotation_matrix;
glm::mat4 lookat_matrix;


glm::mat4 model_matrix;
glm::mat4 view_matrix;


glm::mat4 modelview_matrix;
glm::mat3 normal_matrix;

GLuint uModelViewMatrix;
GLuint viewMatrix;
GLuint normalMatrix;
GLuint lPos, l1On;

int lightVertices;

Human* h;
Bike* b;
Track* t;

// Buffers for light source
GLuint vbo, vao;

bool bike = true, rider = true, track = true;
int selected;

Camera chosen_cam = GLOBAL;

glm::mat4 getCameraMatrix(Camera cam = GLOBAL)
{
  if(cam == GLOBAL)
  {
    //Creating the lookat and the up vectors for the camera
    // std::cout<<"htg"<<std::endl;
    glm::mat4 global_cam_mat = glm::rotate(glm::mat4(1.0f), glm::radians(c_xrot), glm::vec3(1.0f,0.0f,0.0f));
    global_cam_mat = glm::rotate(global_cam_mat, glm::radians(c_yrot), glm::vec3(0.0f,1.0f,0.0f));
    global_cam_mat = glm::rotate(global_cam_mat, glm::radians(c_zrot), glm::vec3(0.0f,0.0f,1.0f));
    glm::vec4 c_pos = glm::vec4(c_xpos,c_ypos,c_zpos, 1.0)*global_cam_mat;
    glm::vec4 c_up = glm::vec4(c_up_x,c_up_y,c_up_z, 1.0)*global_cam_mat;

    return glm::lookAt(glm::vec3(c_pos),glm::vec3(0.0),glm::vec3(c_up));
  }
  else if(cam == RIDER)
  {
    glm::mat4 rider_cam_mat = h->torso->rot_mat;
    glm::mat4 rider_trans = glm::translate(glm::mat4(1.0f), glm::vec3(gtx[1], gty[1], gtz[1]));
    glm::vec4 c_up = rider_cam_mat*glm::vec4(0.0, 1.0, 0.0, 1.0);
    glm::vec4 c_pos = rider_trans*rider_cam_mat*glm::vec4(0.0, 0.0, -5.0, 1.0);
    glm::vec4 origin(0.0, 0.0, 1.0, 1.0);
    // std::cout<<((h->torso->translation)*origin).w<<std::endl;
    return glm::lookAt(glm::vec3(c_pos),glm::vec3(rider_trans*rider_cam_mat*origin),glm::vec3(c_up));
  }
  else if(cam == FPV)
  {
    glm::mat4 rider_cam_mat = h->torso->rot_mat;
    glm::mat4 neck_trans = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, 0.0));
    glm::mat4 rider_trans = glm::translate(neck_trans, glm::vec3(gtx[1], gty[1], gtz[1]));
    //glm::mat4 rider_trans = glm::translate(glm::mat4(1.0f), glm::vec3(gtx[1], gty[1], gtz[1]));
    glm::vec4 c_up = rider_cam_mat*glm::vec4(0.0, 2.0, 0.0, 1.0);
    glm::vec4 c_pos = rider_trans*rider_cam_mat*glm::vec4(0.0, 2.5, -0.1, 1.0);
    glm::vec4 origin(0.0, 2.5, 1.0, 1.0);
    // std::cout<<((h->torso->translation)*origin).w<<std::endl;
    return glm::lookAt(glm::vec3(c_pos),glm::vec3(rider_trans*rider_cam_mat*origin),glm::vec3(c_up));
  }
  else
  {
    throw std::invalid_argument("Invalid Camera");
  }
}

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
  std::string world_vertex_shader_file("../src/07_vshader.glsl");
  std::string lightSource_vertex_shader_file("../src/light_vshader.glsl");
  std::string fragment_shader_file("../src/07_fshader.glsl");

  std::vector<GLuint> worldShaderList;
  worldShaderList.push_back(csX75::LoadShaderGL(GL_VERTEX_SHADER, world_vertex_shader_file));
  worldShaderList.push_back(csX75::LoadShaderGL(GL_FRAGMENT_SHADER, fragment_shader_file));

  worldShaderProgram = csX75::CreateProgramGL(worldShaderList);
  glUseProgram( worldShaderProgram );

  // getting the attributes from the shader program
  vPosition = glGetAttribLocation( worldShaderProgram, "vPosition" );
  vColor = glGetAttribLocation( worldShaderProgram, "vColor" );
  vNormal = glGetAttribLocation( worldShaderProgram, "vNormal" );

  uModelViewMatrix = glGetUniformLocation( worldShaderProgram, "uModelViewMatrix" );
  normalMatrix = glGetUniformLocation( worldShaderProgram, "normalMatrix" );
  viewMatrix = glGetUniformLocation( worldShaderProgram, "viewMatrix" );
  
  lPos = glGetUniformLocation( worldShaderProgram, "lPos" );
  l1On = glGetUniformLocation( worldShaderProgram, "l1On" );

  //note that the buffers are initialized in the respective constructors
  h = new Human();
  if(rider && !bike) curr_node = h->torso;
  b = new Bike(bike_params);
  if(bike && !rider) curr_node = b->body;
  t = new Track(track_params);

  curr_node = t->plane1;
  selected = 2;

  h->init_arrange();
  b->init_arrange();

  // Initialising the buffers to render the light sources
  std::vector<GLuint> lightSourceShaderList;
  lightSourceShaderList.push_back(csX75::LoadShaderGL(GL_VERTEX_SHADER, lightSource_vertex_shader_file));
  lightSourceShaderList.push_back(csX75::LoadShaderGL(GL_FRAGMENT_SHADER, fragment_shader_file));

  lightShaderProgram = csX75::CreateProgramGL(lightSourceShaderList);
  glUseProgram(lightShaderProgram);

  // getting the attributes from the shader program
  vPosition = glGetAttribLocation( lightShaderProgram, "vPosition" );
  vColor = glGetAttribLocation( lightShaderProgram, "vColor" );
  uModelViewMatrix = glGetUniformLocation( lightShaderProgram, "uModelViewMatrix" );

  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);

  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);

  Sphere light(0.5, 20, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
  lightVertices = light.num_vertices;

  glBufferData(GL_ARRAY_BUFFER, lightVertices*sizeof(glm::vec4) + lightVertices*sizeof(glm::vec4), NULL, GL_STATIC_DRAW);
  glBufferSubData(GL_ARRAY_BUFFER, 0, lightVertices*sizeof(glm::vec4), light.vert_arr);
  glBufferSubData(GL_ARRAY_BUFFER, lightVertices*sizeof(glm::vec4), lightVertices*sizeof(glm::vec4), light.col_arr);

  glEnableVertexAttribArray(vPosition);
  glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

  glEnableVertexAttribArray(vColor);
  glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(lightVertices*sizeof(glm::vec4)));
}

void renderGL(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  matrixStack.clear();

  // Creating the lookat and the up vectors for the camera
  // c_rotation_matrix = glm::rotate(glm::mat4(1.0f), glm::radians(c_xrot), glm::vec3(1.0f,0.0f,0.0f));
  // c_rotation_matrix = glm::rotate(c_rotation_matrix, glm::radians(c_yrot), glm::vec3(0.0f,1.0f,0.0f));
  // c_rotation_matrix = glm::rotate(c_rotation_matrix, glm::radians(c_zrot), glm::vec3(0.0f,0.0f,1.0f));

  // glm::vec4 c_pos = glm::vec4(c_xpos,c_ypos,c_zpos, 1.0)*c_rotation_matrix;
  // glm::vec4 c_up = glm::vec4(c_up_x,c_up_y,c_up_z, 1.0)*c_rotation_matrix;
  // Creating the lookat matrix
  lookat_matrix = getCameraMatrix(chosen_cam);

  //creating the projection matrix

  // projection_matrix = glm::ortho(-20.0, 20.0, -20.0, 20.0, -500.0, 500.0);
  projection_matrix = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 500.0f);
  // projection_matrix = glm::frustum(-20.0, 20.0, -20.0, 20.0, -500.0, 500.0);

  view_matrix = projection_matrix*lookat_matrix;

  glUseProgram(worldShaderProgram);
  glUniformMatrix4fv(viewMatrix, 1, GL_FALSE, glm::value_ptr(view_matrix));
  
  glm::vec3 lightPos = glm::vec3(lxPos, lyPos, lzPos);

  // printf("Light pos is %f, %f, %f\n", lxPos, lyPos, lzPos);
  glUniform3fv(lPos, 1, glm::value_ptr(lightPos));

  glUniform1i(l1On, source1);

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

  // Rendering the light sources
  glUseProgram(lightShaderProgram);
  
  glm::mat4 model_matrix = glm::translate(glm::mat4(1.0f), lightPos);
  glm::mat4 modelViewProjectionMatrix = view_matrix * model_matrix;

  glUniformMatrix4fv(uModelViewMatrix, 1, GL_FALSE, glm::value_ptr(modelViewProjectionMatrix));
  glBindVertexArray(vao);
  glDrawArrays(GL_TRIANGLES, 0, lightVertices);
}

HNode* getNode(char key)
{
  switch(key)
  {
    case 'a':
      // printf("Returning Torso\n");
      return h->torso;
      
    case 'b':
      // printf("Returning Left Upper Arm\n");
      return h->left_arm[0];
      
    case 'c':
      // printf("Returning Left Lower Arm\n");
      return h->left_arm[1];
      
    case 'd':
      // printf("Returning Right Upper Arm\n");
      return h->right_arm[0];
      
    case 'e':
      // printf("Returning Right Lower Arm\n");
      return h->right_arm[1];
      
    case 'f':
      // printf("Returning Left Upper Leg\n");
      return h->left_leg[0];
      
    case 'g':
      // printf("Returning Left Lower Leg\n");
      return h->left_leg[1];
      
    case 'h':
      // printf("Returning Right Upper Leg\n");
      return h->right_leg[0];
      
    case 'i':
      // printf("Returning Right Lower Leg\n");
      return h->right_leg[1];
      
    case 'j':
      // printf("Returning Neck\n");
      return h->neck;
      
    case 'k':
      // printf("Returning Head\n");
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
  window = glfwCreateWindow(800, 800, "CS475 Assignment 2 Track", NULL, NULL);
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
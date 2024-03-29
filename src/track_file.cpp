#include "07_hierarchical_modelling.hpp"

GLuint worldShaderProgram, lightShaderProgram, texShaderProgram;

glm::mat4 rotation_matrix;
glm::mat4 projection_matrix;
glm::mat4 c_rotation_matrix;
glm::mat4 lookat_matrix;


glm::mat4 model_matrix;
glm::mat4 view_matrix;
glm::mat4 view_projection_matrix;


glm::mat4 modelview_matrix;
glm::mat3 normal_matrix;

GLuint uModelViewMatrix;
GLuint viewMatrix;
GLuint normalMatrix;
GLuint modelMatrix;

// The structure of the array below is as follows :
// lPos = {Global light 1, Global light 2, Spotlight, Headlight}
GLuint lPos[4], l1On;

glm::vec3 lightPos[4], headlightDir;

// The structure of spotDir is as follows :
// spotDir = {Spotlight, Headlight}
GLuint spotDir[2];

int lightVertices;

Human* h;
Bike* b;
Track* t;

// Buffers for light source
GLuint vbo[4], vao[4];

bool bike = true, rider = true, track = true;
int selected;

Camera chosen_cam = GLOBAL;


glm::mat4 getCameraMatrix(Camera cam = GLOBAL)
{
  if(cam == GLOBAL)
  {
    //Creating the lookat and the up vectors for the camera
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
    return glm::lookAt(glm::vec3(c_pos),glm::vec3(rider_trans*rider_cam_mat*origin),glm::vec3(c_up));
  }
  else if(cam == FPV)
  {
    glm::mat4 rider_cam_mat = h->torso->rot_mat;
    glm::mat4 neck_trans = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, 0.0));
    glm::mat4 rider_trans = glm::translate(neck_trans, glm::vec3(gtx[1], gty[1], gtz[1]));

    glm::vec4 c_up = rider_cam_mat*glm::vec4(0.0, 2.0, 0.0, 1.0);
    glm::vec4 c_pos = rider_trans*rider_cam_mat*glm::vec4(0.0, 2.5, -0.1, 1.0);
    glm::vec4 origin(0.0, 2.5, 1.0, 1.0);
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
  std::string lightSource_fragment_shader_file("../src/light_fshader.glsl");

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
  modelMatrix = glGetUniformLocation( worldShaderProgram, "modelMatrix" );
  
  lPos[0] = glGetUniformLocation( worldShaderProgram, "lPos[0]" );
  lPos[1] = glGetUniformLocation( worldShaderProgram, "lPos[1]" );
  lPos[2] = glGetUniformLocation( worldShaderProgram, "lPos[2]" );
  lPos[3] = glGetUniformLocation( worldShaderProgram, "lPos[3]" );

  l1On = glGetUniformLocation( worldShaderProgram, "l1On" );

  spotDir[0] = glGetUniformLocation( worldShaderProgram, "spotDir[0]" );
  spotDir[1] = glGetUniformLocation( worldShaderProgram, "spotDir[1]" );
  // For texture mapping
  // Initialising the buffers to render the light sources


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
  lightSourceShaderList.push_back(csX75::LoadShaderGL(GL_FRAGMENT_SHADER, lightSource_fragment_shader_file));

  lightShaderProgram = csX75::CreateProgramGL(lightSourceShaderList);
  glUseProgram(lightShaderProgram);

  // getting the attributes from the shader program
  vPosition = glGetAttribLocation( lightShaderProgram, "vPosition" );
  vColor = glGetAttribLocation( lightShaderProgram, "vColor" );
  uModelViewMatrix = glGetUniformLocation( lightShaderProgram, "uModelViewMatrix" );

  glGenVertexArrays(4, vao);
  glGenBuffers(4, vbo);

  // Source 1
  glBindVertexArray(vao[0]);
  glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

  Sphere light1(0.5, 20, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
  lightVertices = light1.num_vertices;

  glBufferData(GL_ARRAY_BUFFER, lightVertices*sizeof(glm::vec4) + lightVertices*sizeof(glm::vec4), NULL, GL_STATIC_DRAW);
  glBufferSubData(GL_ARRAY_BUFFER, 0, lightVertices*sizeof(glm::vec4), light1.vert_arr);
  glBufferSubData(GL_ARRAY_BUFFER, lightVertices*sizeof(glm::vec4), lightVertices*sizeof(glm::vec4), light1.col_arr);

  glEnableVertexAttribArray(vPosition);
  glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

  glEnableVertexAttribArray(vColor);
  glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(lightVertices*sizeof(glm::vec4)));

  // Source 2
  glBindVertexArray(vao[1]);
  glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);

  Sphere light2(0.5, 20, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
  lightVertices = light2.num_vertices;

  glBufferData(GL_ARRAY_BUFFER, lightVertices*sizeof(glm::vec4) + lightVertices*sizeof(glm::vec4), NULL, GL_STATIC_DRAW);
  glBufferSubData(GL_ARRAY_BUFFER, 0, lightVertices*sizeof(glm::vec4), light2.vert_arr);
  glBufferSubData(GL_ARRAY_BUFFER, lightVertices*sizeof(glm::vec4), lightVertices*sizeof(glm::vec4), light2.col_arr);

  glEnableVertexAttribArray(vPosition);
  glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

  glEnableVertexAttribArray(vColor);
  glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(lightVertices*sizeof(glm::vec4)));

  // Source 3 : Spotlight
  glBindVertexArray(vao[2]);
  glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);

  Sphere light3(0.5, 20, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
  lightVertices = light3.num_vertices;

  glBufferData(GL_ARRAY_BUFFER, lightVertices*sizeof(glm::vec4) + lightVertices*sizeof(glm::vec4), NULL, GL_STATIC_DRAW);
  glBufferSubData(GL_ARRAY_BUFFER, 0, lightVertices*sizeof(glm::vec4), light3.vert_arr);
  glBufferSubData(GL_ARRAY_BUFFER, lightVertices*sizeof(glm::vec4), lightVertices*sizeof(glm::vec4), light3.col_arr);

  glEnableVertexAttribArray(vPosition);
  glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

  glEnableVertexAttribArray(vColor);
  glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(lightVertices*sizeof(glm::vec4)));

  // Source 4 : Headlight
  glBindVertexArray(vao[3]);
  glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);

  Sphere light4(0.25, 20, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
  lightVertices = light4.num_vertices;

  glBufferData(GL_ARRAY_BUFFER, lightVertices*sizeof(glm::vec4) + lightVertices*sizeof(glm::vec4), NULL, GL_STATIC_DRAW);
  glBufferSubData(GL_ARRAY_BUFFER, 0, lightVertices*sizeof(glm::vec4), light4.vert_arr);
  glBufferSubData(GL_ARRAY_BUFFER, lightVertices*sizeof(glm::vec4), lightVertices*sizeof(glm::vec4), light4.col_arr);

  glEnableVertexAttribArray(vPosition);
  glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

  glEnableVertexAttribArray(vColor);
  glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(lightVertices*sizeof(glm::vec4)));

}

void renderGL(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  matrixStack.clear();
  lookat_matrix = getCameraMatrix(chosen_cam);

  //creating the projection matrix

  projection_matrix = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 500.0f);

  view_matrix = lookat_matrix;
  view_projection_matrix = projection_matrix * lookat_matrix;

  glUseProgram(worldShaderProgram);
  glUniformMatrix4fv(viewMatrix, 1, GL_FALSE, glm::value_ptr(view_matrix));
  
  // These are the positions of light in the world coordinate system
  lightPos[0] = glm::vec3(lxPos[0], lyPos[0], lzPos[0]);
  lightPos[1] = glm::vec3(lxPos[1], lyPos[1], lzPos[1]);
  lightPos[2] = glm::vec3(gtx[1], gty[1], 10.0f);
  lightPos[3] = glm::vec3(0.0f);

  glUniform3fv(lPos[0], 1, glm::value_ptr(lightPos[0]));
  glUniform3fv(lPos[1], 1, glm::value_ptr(lightPos[1]));
  glUniform3fv(lPos[2], 1, glm::value_ptr(lightPos[2]));

  glUniform1iv(l1On, 4, sourceStat);

  glUniform3fv(spotDir[0], 1, glm::value_ptr(glm::vec3(0.0f, 0.0f, -1.0f)));

  matrixStack.push_back(projection_matrix);
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

    lightPos[3] = b->body->getWCSPos();
    glUniform3fv(lPos[3], 1, glm::value_ptr(lightPos[3]));

    headlightDir = b->body->getWCSDir();
    glUniform3fv(spotDir[1], 1, glm::value_ptr(headlightDir));

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
  // Source 1
  glUseProgram(lightShaderProgram);
  
  glm::mat4 model_matrix = glm::translate(glm::mat4(1.0f), lightPos[0]);
  glm::mat4 modelViewProjectionMatrix = view_projection_matrix * model_matrix;

  glUniformMatrix4fv(uModelViewMatrix, 1, GL_FALSE, glm::value_ptr(modelViewProjectionMatrix));
  glBindVertexArray(vao[0]);
  glDrawArrays(GL_TRIANGLES, 0, lightVertices);

  // Source 2
  model_matrix = glm::translate(glm::mat4(1.0f), lightPos[1]);
  modelViewProjectionMatrix = view_projection_matrix * model_matrix;

  glUniformMatrix4fv(uModelViewMatrix, 1, GL_FALSE, glm::value_ptr(modelViewProjectionMatrix));
  glBindVertexArray(vao[1]);
  glDrawArrays(GL_TRIANGLES, 0, lightVertices);

  // Source 3
  model_matrix = glm::translate(glm::mat4(1.0f), lightPos[2]);
  modelViewProjectionMatrix = view_projection_matrix * model_matrix;

  glUniformMatrix4fv(uModelViewMatrix, 1, GL_FALSE, glm::value_ptr(modelViewProjectionMatrix));
  glBindVertexArray(vao[2]);
  glDrawArrays(GL_TRIANGLES, 0, lightVertices);

  // Source 4
  model_matrix = glm::translate(glm::mat4(1.0f), lightPos[3]);
  modelViewProjectionMatrix = view_projection_matrix * model_matrix;

  glUniformMatrix4fv(uModelViewMatrix, 1, GL_FALSE, glm::value_ptr(modelViewProjectionMatrix));
  glBindVertexArray(vao[3]);
  glDrawArrays(GL_TRIANGLES, 0, lightVertices);
}

HNode* getNode(char key)
{
  switch(key)
  {
    case GLFW_KEY_1:
      // printf("Returning Torso\n");
      return h->torso;
      
    case GLFW_KEY_2:
      // printf("Returning Left Upper Arm\n");
      return h->left_arm[0];
      
    case GLFW_KEY_3:
      // printf("Returning Left Lower Arm\n");
      return h->left_arm[1];
      
    case GLFW_KEY_4:
      // printf("Returning Right Upper Arm\n");
      return h->right_arm[0];
      
    case GLFW_KEY_5:
      // printf("Returning Right Lower Arm\n");
      return h->right_arm[1];
      
    case GLFW_KEY_6:
      // printf("Returning Left Upper Leg\n");
      return h->left_leg[0];
      
    case GLFW_KEY_7:
      // printf("Returning Left Lower Leg\n");
      return h->left_leg[1];
      
    case GLFW_KEY_8:
      // printf("Returning Right Upper Leg\n");
      return h->right_leg[0];
      
    case GLFW_KEY_9:
      // printf("Returning Right Lower Leg\n");
      return h->right_leg[1];
      
    case GLFW_KEY_0:
      // printf("Returning Neck\n");
      return h->neck;
      
    case GLFW_KEY_MINUS:
      // printf("Returning Head\n");
      return h->head;
    
    default:
      return NULL;
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
  window = glfwCreateWindow(800, 800, "CS475 Assignment 3", NULL, NULL);
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
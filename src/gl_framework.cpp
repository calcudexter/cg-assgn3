#include "gl_framework.hpp"
#include "hierarchy_node.hpp"

extern GLfloat c_xrot,c_yrot,c_zrot;
extern bool enable_perspective;
extern csX75::HNode* getNode(char), *curr_node;
extern float* dof_param;

bool shift_held = false;
namespace csX75
{
  //! Initialize GL State
  void initGL(void)
  {
    //Set framebuffer clear color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    //Set depth buffer furthest depth
    glClearDepth(1.0);
    //Set depth test to less-than
    glDepthFunc(GL_LESS);
    //Enable depth testing
    glEnable(GL_DEPTH_TEST);
  }
  
  //!GLFW Error Callback
  void error_callback(int error, const char* description)
  {
    std::cerr<<description<<std::endl;
  }
  
  //!GLFW framebuffer resize callback
  void framebuffer_size_callback(GLFWwindow* window, int width, int height)
  {
    //!Resize the viewport to fit the window size - draw to entire window
    glViewport(0, 0, width, height);
  }
  
  //!GLFW keyboard callback
  void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
  {
    int cond = (curr_node->name == "LArmL") ||
               (curr_node->name == "LArmR") ||
               (curr_node->name == "LegL") ||
               (curr_node->name == "LegR");

    //!Close the window if the ESC key was pressed
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
      glfwSetWindowShouldClose(window, GL_TRUE);
    else if (key == GLFW_KEY_LEFT && action == GLFW_PRESS | GLFW_REPEAT)
      {if(!cond) curr_node->dec_ry();}
    else if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS | GLFW_REPEAT)
      {if(!cond) curr_node->inc_ry();}
    else if (key == GLFW_KEY_UP && action == GLFW_PRESS | GLFW_REPEAT)
      {curr_node->dec_rx();}
    else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS | GLFW_REPEAT)
      {curr_node->inc_rx();}
    else if (key == GLFW_KEY_PAGE_UP && action == GLFW_PRESS | GLFW_REPEAT)
      {if(!cond) curr_node->inc_rz();}
    else if (key == GLFW_KEY_PAGE_DOWN && action == GLFW_PRESS | GLFW_REPEAT)
      {if(!cond) curr_node->dec_rz();}
    else if (key == GLFW_KEY_P && action == GLFW_PRESS)
      enable_perspective = !enable_perspective;   
    else if (key == GLFW_KEY_A  && action == GLFW_PRESS | GLFW_REPEAT)
      c_yrot -= 3.0;
    else if (key == GLFW_KEY_D  && action == GLFW_PRESS | GLFW_REPEAT)
      c_yrot += 3.0;
    else if (key == GLFW_KEY_W  && action == GLFW_PRESS | GLFW_REPEAT)
      c_xrot -= 3.0;
    else if (key == GLFW_KEY_S  && action == GLFW_PRESS | GLFW_REPEAT)
      c_xrot += 3.0;
    else if (key == GLFW_KEY_Q  && action == GLFW_PRESS | GLFW_REPEAT)
      c_zrot -= 3.0;
    else if (key == GLFW_KEY_E  && action == GLFW_PRESS | GLFW_REPEAT)
      c_zrot += 3.0;
    else if (key == GLFW_KEY_C && action == GLFW_PRESS) {
      printf("Press keys to select the part\n");
      char c;
      scanf(" %c", &c);
      printf("Key pressed %c\n", c);

      curr_node = getNode(c);
    }
    else if (key == GLFW_KEY_F && action == GLFW_PRESS | GLFW_REPEAT)
    {
      if(shift_held)
        dof_param[1] -= 1.0f;
      else 
        dof_param[1] += 1.0f;
    }
    else if (key == GLFW_KEY_R && action == GLFW_PRESS | GLFW_REPEAT)
    {
      if(shift_held)
        dof_param[0] -= 1.0f;
      else 
        dof_param[0] += 1.0f;
    }
    else if (key == GLFW_KEY_T && action == GLFW_PRESS | GLFW_REPEAT)
    {
      if(shift_held)
        dof_param[2] -= 1.0f;
      else 
        dof_param[2] += 1.0f;
    }
    else if ((key == GLFW_KEY_LEFT_SHIFT || key == GLFW_KEY_RIGHT_SHIFT))
    {
      if (action == GLFW_PRESS)
      {  shift_held = true;}
      else if(action == GLFW_RELEASE)
      { shift_held = false;}
    }    
  }
};  
  



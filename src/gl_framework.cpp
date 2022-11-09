#include "gl_framework.hpp"
#include "hierarchy_node.hpp"

extern GLfloat c_xrot,c_yrot,c_zrot;
extern GLfloat lxPos[2], lyPos[2], lzPos[2];
extern GLfloat gtx[3], gty[3], gtz[3];
extern GLfloat scaling[3];
extern csX75::HNode* getNode(char), *curr_node;
extern float* dof_param;
extern Track* t;
extern Bike* b;
extern Human* h;
extern Camera chosen_cam;

extern bool bike, rider, track;
extern int selected, sourceStat[4], sourceSelected;
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
    glEnable (GL_TEXTURE_2D);
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
    {
      // Rotations

      // printf("Scaling factor (bike) %f\n", scaling[0]);
      // printf("Scaling factor (rider) %f\n", scaling[1]);
      // printf("Scaling factor (track) %f\n", scaling[2]);

      // printf("Translations (bike): {%f, %f, %f}\n", gtx[0], gty[0], gtz[0]);
      // printf("Translations (rider): {%f, %f, %f}\n", gtx[1], gty[1], gtz[1]);
      // printf("Translations (track): {%f, %f, %f}\n", gtx[2], gty[2], gtz[2]);

      // b->print_rot();
      // h->print_rot();
      // if(sourceSelected == 2) return;
      sourceStat[sourceSelected] = (1-sourceStat[sourceSelected]);
    }
    else if (key == GLFW_KEY_O  && action == GLFW_PRESS) {
      sourceSelected += 1;
      sourceSelected %= 4;
    }
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
    else if((key == GLFW_KEY_2 || key == GLFW_KEY_KP_2) && action == GLFW_PRESS | GLFW_REPEAT) {
      if(selected == 3) {
        gty[0] -= 0.1f;
        gty[1] -= 0.1f;
        gty[2] -= 0.1f;
      }
      else
        gty[selected] -= 0.1f;
    }
    else if((key == GLFW_KEY_8 || key == GLFW_KEY_KP_8) && action == GLFW_PRESS | GLFW_REPEAT)
      if(selected == 3) {
        gty[0] += 0.1f;
        gty[1] += 0.1f;
        gty[2] += 0.1f;
      }
      else
        gty[selected] += 0.1f;
    else if((key == GLFW_KEY_4 || key == GLFW_KEY_KP_4) && action == GLFW_PRESS | GLFW_REPEAT)
      if(selected == 3) {
        gtx[0] -= 0.1f;
        gtx[1] -= 0.1f;
        gtx[2] -= 0.1f;
      }
      else
        gtx[selected] -= 0.1f;
    else if((key == GLFW_KEY_6 || key == GLFW_KEY_KP_6) && action == GLFW_PRESS | GLFW_REPEAT)
      if(selected == 3) {
        gtx[0] += 0.1f;
        gtx[1] += 0.1f;
        gtx[2] += 0.1f;
      }
      else
        gtx[selected] += 0.1f;
    else if((key == GLFW_KEY_5 || key == GLFW_KEY_KP_5) && action == GLFW_PRESS | GLFW_REPEAT) {
      if(shift_held)
        if(selected == 3) {
          gtz[0] -= 0.1f;
          gtz[1] -= 0.1f;
          gtz[2] -= 0.1f;
        }
        else
          gtz[selected] -= 0.1f;
      else {
        if(selected == 3) {
          gtz[0] += 0.1f;
          gtz[1] += 0.1f;
          gtz[2] += 0.1f;
        }
        else
          gtz[selected] += 0.1f;
      }
    }
    else if (key == GLFW_KEY_C && action == GLFW_PRESS) {
      if(rider)
      {
        selected = 1;
        printf("Press keys to select the part\n");
        char c;
        scanf(" %c", &c);
        // printf("Key pressed %c\n", c);

        curr_node = getNode(c);
      }
    }
    else if (key == GLFW_KEY_F && action == GLFW_PRESS | GLFW_REPEAT)
    {
      if(shift_held)
        dof_param[1] -= 5.0f;
      else 
        dof_param[1] += 5.0f;
    }
    else if (key == GLFW_KEY_R && action == GLFW_PRESS | GLFW_REPEAT)
    {
      if(shift_held)
        dof_param[0] -= 5.0f;
      else 
        dof_param[0] += 5.0f;
    }
    else if (key == GLFW_KEY_T && action == GLFW_PRESS | GLFW_REPEAT)
    {
      if(shift_held)
        dof_param[2] -= 5.0f;
      else 
        dof_param[2] += 5.0f;
    }
    else if ((key == GLFW_KEY_LEFT_SHIFT || key == GLFW_KEY_RIGHT_SHIFT))
    {
      if (action == GLFW_PRESS)
      {  shift_held = true;}
      else if(action == GLFW_RELEASE)
      { shift_held = false;}
    }
    else if(key == GLFW_KEY_Z && action == GLFW_PRESS)
      selected = 3;
    else if(key == GLFW_KEY_X && action == GLFW_PRESS) {
      if(track)
      {
        selected = 2;
        curr_node = t->plane1;
      }
    }
    else if(key == GLFW_KEY_V && action == GLFW_PRESS) {
      if(bike)
      {
        selected = 0;
        curr_node = b->body;
      }
    }
    else if(key == GLFW_KEY_M && action == GLFW_PRESS | GLFW_REPEAT)
    {
      GLfloat *fac;
      if(selected <= 2)
      {
        fac = &scaling[selected];
    
        if(shift_held) {
          *fac -= 0.01f;
          if(*fac <= 0) *fac = 0;
        }
        else {
          *fac += 0.01f;
        }
      }
      else {
        for(int i = 0; i < 3; i++) {
          fac = &scaling[i];
    
          if(shift_held) {
            *fac -= 0.01f;
            if(*fac <= 0) *fac = 0;
          }
          else {
            *fac += 0.01f;
          }
        }
      }

      // printf("Scaling factor (bike) %f\n", fac[0]);
      // printf("Scaling factor (rider) %f\n", fac[1]);
      // printf("Scaling factor (track) %f\n", fac[2]);
    }
    else if(key == GLFW_KEY_J && action == GLFW_PRESS | GLFW_REPEAT) {
      if(sourceSelected > 1) return;

      if(shift_held) lxPos[sourceSelected] -= 1.0f;
      else lxPos[sourceSelected] += 1.0f;
    }
    else if(key == GLFW_KEY_K && action == GLFW_PRESS | GLFW_REPEAT) {
      if(sourceSelected > 1) return;
      
      if(shift_held) lyPos[sourceSelected] -= 1.0f;
      else lyPos[sourceSelected] += 1.0f;
    }
    else if(key == GLFW_KEY_L && action == GLFW_PRESS | GLFW_REPEAT) {
      if(sourceSelected > 1) return;
      
      if(shift_held) lzPos[sourceSelected] -= 1.0f;
      else lzPos[sourceSelected] += 1.0f;
    }
    else if(key == GLFW_KEY_N && action == GLFW_PRESS) {
      if(chosen_cam == GLOBAL)
        chosen_cam = RIDER;
      else if(chosen_cam == RIDER)
        chosen_cam = FPV;
      else if(chosen_cam == FPV)
        chosen_cam = GLOBAL;
    }
  }
};
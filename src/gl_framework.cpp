#include "stb_image.h"
#include "stb_image_write.h"
#include "gl_framework.hpp"
#include "hierarchy_node.hpp"
#include <iomanip>

extern GLfloat c_xpos, c_ypos, c_zpos;
extern GLfloat c_up_x, c_up_y, c_up_z;
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
bool shift_held = false, control_held = false;

// Animation related
int timestamp = 0;
int isCont[211];
std::vector<std::vector<float>> attrs;
int fps = 100;
extern void renderGL();

int frame_index = 0; // for saving frames
extern bool save_frames;

namespace csX75
{
  void saveImage(const char* filepath, GLFWwindow* w) {
    int width, height;
    glfwGetFramebufferSize(w, &width, &height);
    GLsizei nrChannels = 3;
    GLsizei stride = nrChannels * width;
    stride += (stride % 4) ? (4 - stride % 4) : 0;
    GLsizei bufferSize = stride * height;
    std::vector<char> buffer(bufferSize);
    glPixelStorei(GL_PACK_ALIGNMENT, 4);
    glReadBuffer(GL_FRONT);
    glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, buffer.data());
    // stbi_flip_vertically_on_write(true);
    stbi_write_png(filepath, width, height, nrChannels, buffer.data(), stride);
  }


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
  
  // Assumes comma separated values and line ends with a comma
  std::vector<float> splitstring(std::string s) {
    s = ", " + s;
    
    std::vector<float> out;
    int prev_comma = 0;

    for(int i = 1; i < s.length(); i++) {
      if(s[i] == ',') {
        out.push_back(stof(s.substr(prev_comma + 2, (i-prev_comma-2))));

        prev_comma = i;
      }
    }

    return out;
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
    else if (key == GLFW_KEY_I && action == GLFW_PRESS)
    {
      sourceStat[sourceSelected] = (1-sourceStat[sourceSelected]);
    }
    else if (key == GLFW_KEY_O  && action == GLFW_PRESS) {
      sourceSelected += 1;
      sourceSelected %= 4;
    }
    else if (shift_held && key == GLFW_KEY_A  && action == GLFW_PRESS | GLFW_REPEAT)
      c_yrot -= 3.0;
    else if (shift_held && key == GLFW_KEY_D  && action == GLFW_PRESS | GLFW_REPEAT)
      c_yrot += 3.0;
    else if (shift_held && key == GLFW_KEY_W  && action == GLFW_PRESS | GLFW_REPEAT)
      c_xrot -= 3.0;
    else if (shift_held && key == GLFW_KEY_S  && action == GLFW_PRESS | GLFW_REPEAT)
      c_xrot += 3.0;
    else if (!shift_held && key == GLFW_KEY_S  && action == GLFW_PRESS) {
      // Save the animation state

      FILE *out_file = fopen("../src/keyframes.txt", "a+");

      if(out_file == NULL) {
        printf("Error! Couldn't open keyframes.txt\n");
      }
      
      // Writing to the file now

      // Camera attributes
      for(int i = 0; i < 11; i++) isCont[i] = 1;
      isCont[1] = 0;
      fprintf(out_file, "%d, %d, %f, %f, %f, %f, %f, %f, %f, %f, %f, ", timestamp, (int) chosen_cam, c_xpos, c_ypos, c_zpos, c_up_x, c_up_y, c_up_z, c_xrot, c_yrot, c_zrot);
      timestamp += 100;

      // Light attributes
      for(int i = 11; i < 17; i++) isCont[i] = 1;
      for(int i = 17; i < 21; i++) isCont[i] = 0;
      // The order of insertion is as follows
      // In positions, global lights 1 then 2
      // Then, states of lights 1 and 2, spotlight and headlight
      fprintf(out_file, "%f, %f, %f, %f, %f, %f, %d, %d, %d, %d, ", lxPos[0], lyPos[0], lzPos[0], lxPos[1], lyPos[1], lzPos[1], sourceStat[0], sourceStat[1], sourceStat[2], sourceStat[3]);

      // Bike then Rider attributes
      for(int i = 21; i < 211; i++) isCont[i] = 1;
      fprintf(out_file, "%f, %f, %f, %f, %f, %f, %f, %f, %f, %f, ", gtx[0], gty[0], gtz[0], gtx[1], gty[1], gtz[1], gtx[2], gty[2], gtz[2], dof_param[2]);

      b->body->print_rot_tree(out_file);
      h->torso->print_rot_tree(out_file);

      fprintf(out_file, "\n");
      fflush(out_file);
      fclose(out_file);

      printf("Appended this keyframe to src/keyframes.txt\n");
    }
    else if (!control_held && key == GLFW_KEY_L && action == GLFW_PRESS) {
      // Loads the array attrs with the keyframes read from the file
      attrs.clear();

      FILE *in_file = fopen("../src/keyframes.txt", "r");

      if(in_file == NULL) {
        printf("Error! Couldn't read keyframes.txt\n");
      }

      char* line  = NULL;
      size_t len = 0;
      ssize_t read;
      int lines_read = 0;

      while((read = getline(&line, &len, in_file)) != -1) {
        lines_read++;

        // Need to store the data here within some data structure
        // First of all, need to process the string into proper format
        // and get rid of the commas

        std::string line_read(line);
        attrs.push_back(splitstring(line_read));
      }

      fclose(in_file);
      printf("Read %d keyframes from the saved file\n", lines_read);
    }
    else if (!control_held && key == GLFW_KEY_P && action == GLFW_PRESS) {
      // Plays the animation by interpolating between the frames
      // Steps:
      // 1. Interpolate the keyframes
      // 2. Reload all the variables with the values that you have now stored
      // this will automatically render it to the screen with the new parameters


      // Base case is to load and render the scene with the first frame
      // by default

      for(int i = 0; i < 11; i++) isCont[i] = 1;
      isCont[1] = 0;

      for(int i = 11; i < 17; i++) isCont[i] = 1;
      for(int i = 17; i < 21; i++) isCont[i] = 0;

      for(int i = 21; i < 211; i++) isCont[i] = 1;

      int render = 0;

      for(int i = 1; i < attrs.size(); i++) {
        int frame_diff = attrs[i][0] - attrs[i-1][0];

        for(int j = 0; j < frame_diff; j++) {
          std::vector<float> curr_frame;

          for(int k = 0; k < attrs[0].size(); k++) {
            if(isCont[k] == 0) {
              curr_frame.push_back(attrs[i-1][k]);
              continue;
            }

            // Find the interpolated value here

            float var = (frame_diff-j-1)*attrs[i-1][k] + (j+1)*attrs[i][k];
            var /= frame_diff;

            curr_frame.push_back(var);
          }

          // Now, reload curr_frame to render and using a timer callback
          // go to sleep

          int ind = 1;
          chosen_cam = static_cast<Camera>(curr_frame[ind++]);
          c_xpos = curr_frame[ind++];
          c_ypos = curr_frame[ind++];
          c_zpos = curr_frame[ind++];

          c_up_x = curr_frame[ind++];
          c_up_y = curr_frame[ind++];
          c_up_z = curr_frame[ind++];

          c_xrot = curr_frame[ind++];
          c_yrot = curr_frame[ind++];
          c_zrot = curr_frame[ind++];

          lxPos[0] = curr_frame[ind++];
          lyPos[0] = curr_frame[ind++];
          lzPos[0] = curr_frame[ind++];

          lxPos[1] = curr_frame[ind++];
          lyPos[1] = curr_frame[ind++];
          lzPos[1] = curr_frame[ind++];

          sourceStat[0] = curr_frame[ind++];
          sourceStat[1] = curr_frame[ind++];
          sourceStat[2] = curr_frame[ind++];
          sourceStat[3] = curr_frame[ind++];

          gtx[0] = curr_frame[ind++];
          gty[0] = curr_frame[ind++];
          gtz[0] = curr_frame[ind++];

          gtx[1] = curr_frame[ind++];
          gty[1] = curr_frame[ind++];
          gtz[1] = curr_frame[ind++];

          gtx[2] = curr_frame[ind++];
          gty[2] = curr_frame[ind++];
          gtz[2] = curr_frame[ind++];
          // Need to reload bike and rider

          dof_param[2] = curr_frame[ind++];

          b->body->load_tree(curr_frame, ind);
          h->torso->load_tree(curr_frame, ind);

          glfwWaitEventsTimeout(1.0/fps);
          renderGL();
          if(save_frames)
          {
            std::ostringstream ss;
            ss << std::setfill('0')<<std::setw(5)<<frame_index;
            std::string s = ss.str();
            std::string filename = std::string("../video_frames/frame-") + s + std::string(".png");
            saveImage(filename.c_str(), window);
            std::cout<<"image_saved "<<frame_index<<std::endl;
            frame_index++;
          }
          glfwSwapBuffers(window);
        }
      }
      frame_index = 0;
      printf("Animation ended\n");
    }
    else if (shift_held && key == GLFW_KEY_Q  && action == GLFW_PRESS | GLFW_REPEAT)
      c_zrot -= 3.0;
    else if (shift_held && key == GLFW_KEY_E  && action == GLFW_PRESS | GLFW_REPEAT)
      c_zrot += 3.0;
    else if((key == GLFW_KEY_KP_2) && action == GLFW_PRESS | GLFW_REPEAT) {
      if(selected == 3) {
        gty[0] -= 0.1f;
        gty[1] -= 0.1f;
        gty[2] -= 0.1f;
      }
      else if(selected == 4) {
        gty[0] -= 0.1f;
        gty[1] -= 0.1f;
      }
      else
        gty[selected] -= 0.1f;
    }
    else if(( key == GLFW_KEY_KP_8) && action == GLFW_PRESS | GLFW_REPEAT){
      if(selected == 3) {
        gty[0] += 0.1f;
        gty[1] += 0.1f;
        gty[2] += 0.1f;
      }
      else if(selected == 4) {
        gty[0] += 0.1f;
        gty[1] += 0.1f;
      }
      else
        gty[selected] += 0.1f;
    }
    else if(( key == GLFW_KEY_KP_4) && action == GLFW_PRESS | GLFW_REPEAT)
      if(selected == 3) {
        gtx[0] -= 0.1f;
        gtx[1] -= 0.1f;
        gtx[2] -= 0.1f;
      }
      else if(selected == 4) {
        gtx[0] -= 0.1f;
        gtx[1] -= 0.1f;
      }
      else
        gtx[selected] -= 0.1f;
    else if(( key == GLFW_KEY_KP_6) && action == GLFW_PRESS | GLFW_REPEAT){
      if(selected == 3) {
        gtx[0] += 0.1f;
        gtx[1] += 0.1f;
        gtx[2] += 0.1f;
      }
      else if(selected == 4) {
        gtx[0] += 0.1f;
        gtx[1] += 0.1f;
      }
      else
        gtx[selected] += 0.1f;
    }
    else if(( key == GLFW_KEY_KP_5) && action == GLFW_PRESS | GLFW_REPEAT) {
      if(shift_held)
      {
        if(selected == 3) {
          gtz[0] -= 0.1f;
          gtz[1] -= 0.1f;
          gtz[2] -= 0.1f;
        }
        else if(selected == 4) {
          gtz[0] -= 0.1f;
          gtz[1] -= 0.1f;
        }
        else
          gtz[selected] -= 0.1f;
      }
      else {
        if(selected == 3) {
          gtz[0] += 0.1f;
          gtz[1] += 0.1f;
          gtz[2] += 0.1f;
        }
        else if(selected == 4) {
          gtz[0] += 0.1f;
          gtz[1] += 0.1f;
        }
        else
          gtz[selected] += 0.1f;
      }
    }
    else if (rider && control_held && action == GLFW_PRESS) {
      curr_node = getNode(key);
      if(curr_node)
      {
        selected = 1;
        printf("Selected %c\n", key);
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
    else if (key == GLFW_KEY_LEFT_CONTROL || key == GLFW_KEY_RIGHT_CONTROL) {
      if(action == GLFW_PRESS) {
        control_held = true;
      }
      else {
        control_held = false;
      }
    }
    else if(key == GLFW_KEY_Z && action == GLFW_PRESS)
      selected = 3;
    else if(key == GLFW_KEY_X && action == GLFW_PRESS) {
      selected = 4;
    }
    else if(key == GLFW_KEY_C && action == GLFW_PRESS) {
      selected = 2;
      curr_node = t->plane1;
    }
    else if(key == GLFW_KEY_V && action == GLFW_PRESS) {
      selected = 0;
      curr_node = b->body;
    }
    else if(key == GLFW_KEY_B && action == GLFW_PRESS) {
      selected = 1;
      curr_node = h->torso;
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
    else if(control_held && key == GLFW_KEY_J && action == GLFW_PRESS | GLFW_REPEAT) {
      if(sourceSelected > 1) return;

      if(shift_held) lxPos[sourceSelected] -= 1.0f;
      else lxPos[sourceSelected] += 1.0f;
    }
    else if(control_held && key == GLFW_KEY_K && action == GLFW_PRESS | GLFW_REPEAT) {
      if(sourceSelected > 1) return;
      
      if(shift_held) lyPos[sourceSelected] -= 1.0f;
      else lyPos[sourceSelected] += 1.0f;
    }
    else if(control_held && key == GLFW_KEY_L && action == GLFW_PRESS | GLFW_REPEAT) {
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
#ifndef _HNODE_HPP_
#define _HNODE_HPP_

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <string>
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"


namespace csX75	 { 

	// A simple class that represents a node in the hierarchy tree
	class HNode {
		GLfloat tx,ty,tz,rx,ry,rz;

		std::size_t vertex_buffer_size;
		std::size_t color_buffer_size;
		std::size_t texture_buffer_size;
		std::size_t normal_buffer_size;
  		
		std::string tex_vertex_shader_file;
  		std::string tex_fragment_shader_file;
		
		GLuint num_vertices;
		GLuint vao,vbo;
		GLuint vtexPos, vtexCoord, vtexNormal, utexModelViewMatrix;
		GLuint texShaderProgram;
		GLuint tex;

		glm::mat4 pre_rot;
		glm::mat4 inv_trans;
		glm::mat4 rotation;
		std::vector<HNode*> children;
		HNode* parent;

		int render_texture = false;

		void update_matrices();

	  public:
		std::string name;
		glm::mat4 rot_mat;
		glm::mat4 translation;
		glm::mat4 pre_trans;
		GLfloat ptx, pty, ptz;

		HNode (HNode*, GLuint, glm::vec4*,  glm::vec4*, glm::vec4*, std::size_t, std::size_t, std::size_t, std::string);
		HNode (HNode*, GLuint, glm::vec4*,  glm::vec2*, glm::vec4*, std::size_t, std::size_t, std::size_t, std::string);
		
		void add_child(HNode*);
		void render();
		void change_parameters(GLfloat,GLfloat,GLfloat,GLfloat,GLfloat,GLfloat,GLfloat,GLfloat,GLfloat);
		void change_parameters(GLfloat,GLfloat,GLfloat,GLfloat,GLfloat,GLfloat);
		void render_tree();
		void inc_rx();
		void inc_ry();
		void inc_rz();
		void dec_rx();
		void dec_ry();
		void dec_rz();
		void print_rot();
		void init_rot(GLfloat rx, GLfloat ry, GLfloat rz);
	};

	glm::mat4* multiply_stack(std::vector <glm::mat4> );
};	

#endif
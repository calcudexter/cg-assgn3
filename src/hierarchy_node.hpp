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
		GLfloat ptx, pty, ptz;

		std::size_t vertex_buffer_size;
		std::size_t color_buffer_size;
		std::size_t normal_buffer_size;

		GLuint num_vertices;
		GLuint vao,vbo;

		glm::mat4 pre_rot;
		glm::mat4 pre_trans;
		glm::mat4 inv_trans;
		glm::mat4 rotation;
		glm::mat4 translation;
		
		std::vector<HNode*> children;
		HNode* parent;

		void update_matrices();

	  public:
		std::string name;
		glm::mat4 rot_mat;

		HNode (HNode*, GLuint, glm::vec4*,  glm::vec4*, glm::vec4*, std::size_t, std::size_t, std::size_t, std::string);

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
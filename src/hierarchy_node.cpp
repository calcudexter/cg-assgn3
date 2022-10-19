#include "hierarchy_node.hpp"

#include <iostream>

extern GLuint vPosition,vColor,uModelViewMatrix;
extern std::vector<glm::mat4> matrixStack;

namespace csX75
{

	HNode::HNode(HNode* a_parent, GLuint num_v, glm::vec4* a_vertices, glm::vec4* a_colours, std::size_t v_size, std::size_t c_size, std::string name){

		num_vertices = num_v;

		// sizes in bytes
		vertex_buffer_size = v_size;
		color_buffer_size = c_size;
		// initialize vao and vbo of the object;


		//Ask GL for a Vertex Attribute Objects (vao)
		glGenVertexArrays (1, &vao);
		//Ask GL for aVertex Buffer Object (vbo)
		glGenBuffers (1, &vbo);

		//bind them
		glBindVertexArray (vao);
		glBindBuffer (GL_ARRAY_BUFFER, vbo);

		
		glBufferData (GL_ARRAY_BUFFER, vertex_buffer_size + color_buffer_size, NULL, GL_STATIC_DRAW);
		glBufferSubData( GL_ARRAY_BUFFER, 0, vertex_buffer_size, a_vertices );
		glBufferSubData( GL_ARRAY_BUFFER, vertex_buffer_size, color_buffer_size, a_colours );

		//setup the vertex array as per the shader
		glEnableVertexAttribArray( vPosition );
		glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

		glEnableVertexAttribArray( vColor );
		glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(vertex_buffer_size));


		// set parent

		if(a_parent == NULL){
			parent = NULL;
		}
		else{
			parent = a_parent;
			parent->add_child(this);
		}

		//initial parameters are set to 0;

		tx=ty=tz=rx=ry=rz=ptx=pty=ptz=0;

		update_matrices();
		rot_mat = glm::mat4(1.0f);	

		this->name = name;
	}

	void HNode::update_matrices(){

		pre_rot = glm::rotate(glm::mat4(1.0f), glm::radians(rx), glm::vec3(1.0f,0.0f,0.0f));
		pre_rot = glm::rotate(pre_rot, glm::radians(ry), glm::vec3(0.0f,1.0f,0.0f));
		pre_rot = glm::rotate(pre_rot, glm::radians(rz), glm::vec3(0.0f,0.0f,1.0f));

		translation = glm::translate(glm::mat4(1.0f),glm::vec3(tx,ty,tz));
		pre_trans = glm::translate(glm::mat4(1.0f),glm::vec3(ptx,pty,ptz));
		inv_trans = glm::translate(glm::mat4(1.0f),glm::vec3(-ptx,-pty,-ptz));
		rotation = rot_mat * pre_rot;
	}

	void HNode::add_child(HNode* a_child){
		children.push_back(a_child);
	}

	void HNode::change_parameters(GLfloat atx, GLfloat aty, GLfloat atz, GLfloat arx, GLfloat ary, GLfloat arz, GLfloat ptx, GLfloat pty, GLfloat ptz){
		tx = atx;
		ty = aty;
		tz = atz;
		rx = arx;
		ry = ary;
		rz = arz;
		this->ptx = ptx;
		this->pty = pty;
		this->ptz = ptz;
		update_matrices();
	}

	void HNode::render(){

		//matrixStack multiply
		glm::mat4* ms_mult = multiply_stack(matrixStack);

		glUniformMatrix4fv(uModelViewMatrix, 1, GL_FALSE, glm::value_ptr(*ms_mult));
		glBindVertexArray (vao);
		glDrawArrays(GL_TRIANGLES, 0, num_vertices);

		// for memory 
		delete ms_mult;

	}

	void HNode::render_tree(){
		matrixStack.push_back(inv_trans);
		matrixStack.push_back(rot_mat);
		matrixStack.push_back(pre_trans);
		matrixStack.push_back(translation);
		matrixStack.push_back(pre_rot);

		render();
		for(int i=0;i<children.size();i++){
			children[i]->render_tree();
		}

		matrixStack.pop_back();
		matrixStack.pop_back();
		matrixStack.pop_back();
		matrixStack.pop_back();
		matrixStack.pop_back();
	}

	void HNode::inc_rx(){
		// rx++;
		rot_mat = glm::rotate(glm::mat4(1.0f), glm::radians(3.0f), glm::vec3(1.0f, 0.0f, 0.0f)) * rot_mat;
		update_matrices();
	}

	void HNode::inc_ry(){
		rot_mat = glm::rotate(glm::mat4(1.0f), glm::radians(3.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * rot_mat;
		update_matrices();
	}

	void HNode::inc_rz(){
		rot_mat = glm::rotate(glm::mat4(1.0f), glm::radians(3.0f), glm::vec3(0.0f, 0.0f, 1.0f)) * rot_mat;
		update_matrices();
	}

	void HNode::dec_rx(){
		rot_mat = glm::rotate(glm::mat4(1.0f), glm::radians(-3.0f), glm::vec3(1.0f, 0.0f, 0.0f)) * rot_mat;
		update_matrices();
	}

	void HNode::dec_ry(){
		rot_mat = glm::rotate(glm::mat4(1.0f), glm::radians(-3.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * rot_mat;
		update_matrices();
	}

	void HNode::dec_rz(){
		rot_mat = glm::rotate(glm::mat4(1.0f), glm::radians(-3.0f), glm::vec3(0.0f, 0.0f, 1.0f)) * rot_mat;
		update_matrices();
	}


	glm::mat4* multiply_stack(std::vector<glm::mat4> matStack){
		glm::mat4* mult;
		mult = new glm::mat4(1.0f);
	
		for(int i=0;i<matStack.size();i++){
			*mult = (*mult) * matStack[i];
		}	

		return mult;
	}

}
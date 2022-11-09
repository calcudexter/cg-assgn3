#include "hierarchy_node.hpp"
#include "gl_framework.hpp"

#include <iostream>

extern GLuint vPosition,vColor,vNormal,uModelViewMatrix,normalMatrix,modelMatrix;
extern GLuint worldShaderProgram, lightShaderProgram, texShaderProgram;
extern std::vector<glm::mat4> matrixStack;

namespace csX75
{

	HNode::HNode(HNode* a_parent, GLuint num_v, glm::vec4* a_vertices, glm::vec4* a_colours, glm::vec4* a_normals, std::size_t v_size, std::size_t c_size, std::size_t n_size, std::string name){

		num_vertices = num_v;

		// sizes in bytes
		vertex_buffer_size = v_size;
		color_buffer_size = c_size;
		normal_buffer_size = n_size;
		// initialize vao and vbo of the object;


		//Ask GL for a Vertex Attribute Objects (vao)
		glGenVertexArrays (1, &vao);
		//Ask GL for aVertex Buffer Object (vbo)
		glGenBuffers (1, &vbo);

		//bind them
		glBindVertexArray (vao);
		glBindBuffer (GL_ARRAY_BUFFER, vbo);

		
		glBufferData (GL_ARRAY_BUFFER, vertex_buffer_size + color_buffer_size + normal_buffer_size, NULL, GL_STATIC_DRAW);
		glBufferSubData( GL_ARRAY_BUFFER, 0, vertex_buffer_size, a_vertices );
		glBufferSubData( GL_ARRAY_BUFFER, vertex_buffer_size, color_buffer_size, a_colours );
		glBufferSubData( GL_ARRAY_BUFFER, vertex_buffer_size + color_buffer_size, normal_buffer_size, a_normals );

		//setup the vertex array as per the shader
		glEnableVertexAttribArray( vPosition );
		glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

		glEnableVertexAttribArray( vColor );
		glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(vertex_buffer_size));

		glEnableVertexAttribArray( vNormal );
		glVertexAttribPointer( vNormal, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(vertex_buffer_size + color_buffer_size));


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

	HNode::HNode(HNode* a_parent, GLuint num_v, glm::vec4* a_vertices, glm::vec2* a_textures, glm::vec4* a_normals, std::size_t v_size, std::size_t tex_size, std::size_t n_size, std::string name, std::string filename,uint w=256, uint h=256){
		num_vertices = num_v;
		
		// tex files
		this->tex_vertex_shader_file = std::string("../src/tex_vshader.glsl");
  		this->tex_fragment_shader_file = std::string("../src/tex_fshader.glsl");
		
		std::vector<GLuint> texShaderList;
		texShaderList.push_back(csX75::LoadShaderGL(GL_VERTEX_SHADER, tex_vertex_shader_file));
		texShaderList.push_back(csX75::LoadShaderGL(GL_FRAGMENT_SHADER, tex_fragment_shader_file));

		this->texShaderProgram = csX75::CreateProgramGL(texShaderList);
		glUseProgram(this->texShaderProgram);
		//std::cout<<"shader "<<this->texShaderProgram<<std::endl;
		this->vtexPos = glGetAttribLocation(this->texShaderProgram, "vPosition" );
		this->vtexCoord = glGetAttribLocation(this->texShaderProgram, "texCoord" );
		this->vtexNormal = glGetAttribLocation(this->texShaderProgram, "vNormal" );
  		this->utexModelViewMatrix = glGetUniformLocation(this->texShaderProgram, "uModelViewMatrix" );
		// std::cout<<"p: "<<vPosition << "coord : "<<this->vtexCoord<<"umatrix : "<<this->utexModelViewMatrix<<std::endl;
		  // Load Textures 
		this->tex = LoadTexture((std::string("../src/images/")+filename).c_str(),w,h);
		if(this->tex <= 0)
		{
			std::cout<<"Invalid file\n";
			exit(1);
		}
		glBindTexture(GL_TEXTURE_2D, this->tex);
		std::cout<<"tex loaded : "<<this->tex<<std::endl;

		// sizes in bytes
		vertex_buffer_size = v_size;
		texture_buffer_size = tex_size;
		normal_buffer_size = n_size;
		// initialize vao and vbo of the object;

		//Ask GL for a Vertex Attribute Objects (vao)
		glGenVertexArrays (1, &vao);
		//Ask GL for aVertex Buffer Object (vbo)
		glGenBuffers (1, &vbo);

		//bind them
		glBindVertexArray (vao);
		glBindBuffer (GL_ARRAY_BUFFER, vbo);

		
		glBufferData (GL_ARRAY_BUFFER, vertex_buffer_size + texture_buffer_size + normal_buffer_size, NULL, GL_STATIC_DRAW);
		glBufferSubData( GL_ARRAY_BUFFER, 0, vertex_buffer_size, a_vertices );
		glBufferSubData( GL_ARRAY_BUFFER, vertex_buffer_size, texture_buffer_size, a_textures);
		glBufferSubData( GL_ARRAY_BUFFER, vertex_buffer_size + texture_buffer_size, normal_buffer_size, a_normals );

		//setup the vertex array as per the shader
		glEnableVertexAttribArray( this->vtexPos );
		glVertexAttribPointer( this->vtexPos, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

		glEnableVertexAttribArray( this->vtexCoord );
		glVertexAttribPointer( this->vtexCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(vertex_buffer_size));

		glEnableVertexAttribArray( this->vtexNormal );
		glVertexAttribPointer( this->vtexNormal, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(vertex_buffer_size + texture_buffer_size));

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
		this->render_texture = true;
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

	void HNode::change_parameters(GLfloat atx, GLfloat aty, GLfloat atz, GLfloat arx, GLfloat ary, GLfloat arz){
		tx = atx;
		ty = aty;
		tz = atz;
		rx = arx;
		ry = ary;
		rz = arz;
		update_matrices();
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

		if(this->render_texture)
		{
			glUseProgram(this->texShaderProgram);
			// std::cout<<"tex" << this->texShaderProgram<<std::endl;
			glBindTexture(GL_TEXTURE_2D, this->tex);
			glm::mat4* ms_mult = multiply_stack(matrixStack);

			glUniformMatrix4fv(this->utexModelViewMatrix, 1, GL_FALSE, glm::value_ptr(*ms_mult));

			glBindVertexArray (vao);
			glDrawArrays(GL_TRIANGLES, 0, num_vertices);
			
			delete ms_mult;
			glUseProgram(worldShaderProgram);
			return;
		}
		//matrixStack multiply
		glm::mat4* ms_mult = multiply_stack(matrixStack);

		glUniformMatrix4fv(uModelViewMatrix, 1, GL_FALSE, glm::value_ptr(*ms_mult));
		
		// Need the model matrix in shaders too
		glm::mat4 model_matrix = glm::inverse(matrixStack[0] * matrixStack[1]) * (*ms_mult);
		glUniformMatrix4fv(modelMatrix, 1, GL_FALSE, glm::value_ptr(model_matrix));

		// ms_mult is the product of complete matrix stack
		// It contains the projection and viewing transformations too
		glUniformMatrix4fv(this->utexModelViewMatrix, 1, GL_FALSE, glm::value_ptr(*ms_mult));

		// So, just to keep lighting in the WCS, I am removing the projection component
		// in the normal matrix
		glm::mat3 normal_matrix = glm::transpose(glm::inverse(glm::mat3(model_matrix)));
		glUniformMatrix3fv(normalMatrix, 1, GL_FALSE, glm::value_ptr(normal_matrix));


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
		rot_mat =  rot_mat * glm::rotate(glm::mat4(1.0f), glm::radians(3.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		update_matrices();
	}

	void HNode::inc_ry(){
		rot_mat = rot_mat * glm::rotate(glm::mat4(1.0f), glm::radians(3.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		update_matrices();
	}

	void HNode::inc_rz(){
		rot_mat = rot_mat * glm::rotate(glm::mat4(1.0f), glm::radians(3.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		update_matrices();
	}

	void HNode::dec_rx(){
		rot_mat = rot_mat * glm::rotate(glm::mat4(1.0f), glm::radians(-3.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		update_matrices();
	}

	void HNode::dec_ry(){
		rot_mat = rot_mat * glm::rotate(glm::mat4(1.0f), glm::radians(-3.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		update_matrices();
	}

	void HNode::dec_rz(){
		rot_mat = rot_mat * glm::rotate(glm::mat4(1.0f), glm::radians(-3.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		update_matrices();
	}

	void HNode::print_rot() {
		GLfloat rx = atan2(rot_mat[2][1], rot_mat[2][2]), ry = atan2(-rot_mat[2][0], sqrt(rot_mat[2][1]*rot_mat[2][1] + rot_mat[2][2]*rot_mat[2][2]));
		GLfloat rz = atan2(rot_mat[1][0], rot_mat[0][0]);

		printf("Rotations of %s {%f, %f, %f}\n", this->name.c_str(), rx, ry, rz);
	}

	void HNode::init_rot(GLfloat rx, GLfloat ry, GLfloat rz)
	{
		glm::mat4 mat = glm::rotate(glm::mat4(1.0f), rx, glm::vec3(1.0f, 0.0f, 0.0f));
		mat = glm::rotate(mat, ry, glm::vec3(0.0f, 1.0f, 0.0f));
		mat = glm::rotate(mat, rz, glm::vec3(0.0f, 0.0f, 1.0f));

		rot_mat = mat;
	}

	glm::mat4* multiply_stack(std::vector<glm::mat4> matStack){
		glm::mat4* mult;
		mult = new glm::mat4(1.0f);
	
		for(int i=0;i<matStack.size();i++){
			*mult = (*mult) * matStack[i];
		}	

		return mult;
	}

	glm::vec3 HNode::getWCSPos() {
		glm::vec4 OCSPos = glm::vec4(6.0f, 3.0f, 0.0f, 1.0f);

		OCSPos = matrixStack[2] * matrixStack[3] * inv_trans * rot_mat * pre_trans * translation * pre_rot * OCSPos;

		return glm::vec3(OCSPos);
	}

	glm::vec3 HNode::getWCSDir() {
		glm::vec4 OCSDir = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);

		OCSDir = matrixStack[2] * matrixStack[3] * inv_trans * rot_mat * pre_trans * translation * pre_rot * OCSDir;

		return glm::vec3(OCSDir);
	}

}
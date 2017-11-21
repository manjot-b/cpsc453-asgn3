#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "OBJDisplayer.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "objmodel.h"
#include <string>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
using namespace std;

OBJDisplayer::OBJDisplayer(const char *objPath, const char *texPath)
{
	obj = new OBJmodel();
	obj->load(objPath);
	extractVertexData();
	
	initWindow();
	//adjustAspectRatio(); 
	shader = new Shader("rsc/vertex.glsl", "rsc/fragment.glsl");
	shader->link();
	
	texture = new Texture(texPath);
}

OBJDisplayer::~OBJDisplayer()
{
	delete shader;
	delete texture;
	delete obj;
}

int OBJDisplayer::initWindow()
{
    glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(800, 600, "OBJ Displayer", NULL, NULL);

	if (!window)
	{
		cout << "Failed to create GLFW window. TERMINATING" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to initialize GLAD" << endl;
		return -1;
	}

	glViewport(0, 0, 800, 600);
	glfwSetWindowUserPointer(window, this);

	glfwSetFramebufferSizeCallback(window, 
		[] (GLFWwindow *window, int width, int height) {
			glViewport(0, 0, width, height);
		}
	);

	
	glGenBuffers(1, &VBO); // gen 2 buffer and store id in VBO
	glGenBuffers(1, &EBO);
	glGenVertexArrays(1, &VAO);	// one is for texture, the other is for catmull rom points
	glBindVertexArray(VAO);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertexBuffer.size() * sizeof(float), vertexBuffer.data(), GL_STATIC_DRAW);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, squareIndices.size() * sizeof(float), squareIndices.data(), GL_STATIC_DRAW);

	const int TEXTURE_COMPONENTS = 2;
	const int POSITION_COMPONENTS = 3;
	const int NORMAL_COMPONENTS = 3;
	const int VERTEX_COMPONENTS = TEXTURE_COMPONENTS + POSITION_COMPONENTS + NORMAL_COMPONENTS;
	const int STRIDE = VERTEX_COMPONENTS * sizeof(float);
	//position attributes
	glVertexAttribPointer(0, POSITION_COMPONENTS, GL_FLOAT, GL_FALSE, STRIDE, (void*)0);
	glEnableVertexAttribArray(0);
	
	//texture attributes
	glVertexAttribPointer(1, TEXTURE_COMPONENTS, GL_FLOAT, GL_FALSE, STRIDE,
		(void*)(POSITION_COMPONENTS * sizeof(float)));
	glEnableVertexAttribArray(1);
	
	//normal attributes
	glVertexAttribPointer(2, NORMAL_COMPONENTS, GL_FLOAT, GL_FALSE, STRIDE, 
		(void*)((POSITION_COMPONENTS + TEXTURE_COMPONENTS) * sizeof(float)));
	glEnableVertexAttribArray(2);

	// unbind VAO, VBO, and EBO
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return 0;	// if we made it here then success
}

int OBJDisplayer::run()
{
	while(!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.3f, 1.0f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glm::mat4 model(1.0f);
		//model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
		// DRAW IMAGE AS A TEXTURE
		glUseProgram(shader->getProgramID());		
		shader->setUniformMatrix4fv("model", model);

		glBindVertexArray(VAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture->getID());


		glDrawArrays(GL_TRIANGLES, 0, obj->triangleCount());
		
		glBindVertexArray(0);
		
		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &EBO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();
	return 0;
}

void OBJDisplayer::processInput(GLFWwindow *window)
{
	
								
}

void OBJDisplayer::extractVertexData()
{
	for (uint tri = 0; tri < obj->triangleCount(); tri++)
	{
		const Triangle &triangle = (*obj)[tri];

		for(uint vert = 0; vert < 3; vert++)
		{
			vertexBuffer.push_back(triangle.vertex[vert].pos.x);
			vertexBuffer.push_back(triangle.vertex[vert].pos.y);
			vertexBuffer.push_back(triangle.vertex[vert].pos.z);

			vertexBuffer.push_back(triangle.vertex[vert].tex.s);
			vertexBuffer.push_back(triangle.vertex[vert].tex.t);

			vertexBuffer.push_back(triangle.vertex[vert].norm.x);
			vertexBuffer.push_back(triangle.vertex[vert].norm.y);
			vertexBuffer.push_back(triangle.vertex[vert].norm.z);
		}
	}
}

void OBJDisplayer::adjustAspectRatio()
{
	//glfwSetWindowAspectRatio(window, texture->getWidth(), texture->getHeight());
}
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
	int vertexComps[] = {
		3,	// position components
		2,	// texture components
		3	// normal components
	};
	size_t vertCompSize = sizeof(vertexComps) / sizeof(int);
	vertexArray = new VertexArray(vertexComps, vertCompSize, vertexBuffer.data(), vertexBuffer.size());
}

OBJDisplayer::~OBJDisplayer()
{
	delete vertexArray;
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
	glEnable(GL_DEPTH_TEST);
	
	return 0;	// if we made it here then success
}

int OBJDisplayer::run()
{
	while(!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.3f, 1.0f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 model(1.0f);
		//model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
		// DRAW IMAGE AS A TEXTURE
		glUseProgram(shader->getProgramID());		
		shader->setUniformMatrix4fv("model", model);

		glBindVertexArray(vertexArray->getID());
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture->getID());


		glDrawArrays(GL_TRIANGLES, 0, obj->triangleCount() * 3);
		
		glBindVertexArray(0);
		
		glfwSwapBuffers(window);
		glfwPollEvents();

	}

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
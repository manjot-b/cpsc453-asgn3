#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "OBJDisplayer.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "objmodel.h"
#include <string>
#include <algorithm>
#include <cmath>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
using namespace std;

OBJDisplayer::OBJDisplayer(const char *objPath, const char *texPath) :
rotX(glm::radians(360.0f)), rotY(glm::radians(360.0f)), rotZ(glm::radians(360.0f)),
lightPosition(-10.0f, 10.0f, 5.0f), lightColor(1.0f, 1.0f, 1.0f), lightConstantsIdx(0)
{
	obj = new OBJmodel();
	obj->load(objPath);
	extractVertexData();
	calcBoundingBox();
	scale = 1 / boundingBox.length;
	
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

	view = glm::lookAt(
			glm::vec3(0.0f, 2.0f, 3.0f),	// camera position
			glm::vec3(0, 0, 0),				// where camera is lookin
			glm::vec3(0, 1, 0)				// up vector
		);
	perspective = glm::perspective(glm::radians(45.0f), (float)800 / 800, 0.1f, 100.0f);

	lightConstants = {
				// ambient	diffues		specular	shininess
		glm::vec4(0.329412f, 0.780392f, 0.992157f, 27.8974f),	// brass
		glm::vec4(0.05375f, 0.18275f, 0.332741f, 38.4f),		// obsidian
		glm::vec4(0.1745f, 0.61424f, 0.727811f, 76.8f),			// ruby
		glm::vec4(0.0f, 0.01f, 0.50f, 32.0f)					// black
	};	
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

	window = glfwCreateWindow(800, 800, "OBJ Displayer", NULL, NULL);

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

	glViewport(0, 0, 800, 800);
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
		model = glm::scale(model, glm::vec3(scale, scale, scale));
		model = glm::translate(model, glm::vec3(0, -boundingBox.length / 2, -boundingBox.length / 2));		
		model = glm::rotate(model, rotX, glm::vec3(1, 0, 0));
		model = glm::rotate(model, rotY, glm::vec3(0, 1, 0));
		model = glm::rotate(model, rotZ, glm::vec3(0, 0, 1));
		
		// DRAW IMAGE AS A TEXTURE
		glUseProgram(shader->getProgramID());		
		shader->setUniformMatrix4fv("model", model);
		shader->setUniformMatrix4fv("view", view);
		shader->setUniformMatrix4fv("perspective", perspective);
		shader->setUniform3fv("lightPosition", lightPosition);
		shader->setUniform3fv("lightColor", lightColor);
		shader->setUniform4fv("lightConstants", lightConstants[lightConstantsIdx]);

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
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	
	float rotSpeed = glm::radians(3.0f);
	float scaleSpeed = 1.01f;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		rotY += rotSpeed;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		rotY -= rotSpeed;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		rotX -= rotSpeed;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		rotX += rotSpeed;
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		rotZ -= rotSpeed;
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		rotZ += rotSpeed;
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
		scale *= scaleSpeed;
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
		scale /= scaleSpeed;
	
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		lightConstantsIdx = 0;
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		lightConstantsIdx = 1;
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
		lightConstantsIdx = 2;
	if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
		lightConstantsIdx = 3;							
}

void OBJDisplayer::extractVertexData()
{
	for (unsigned int tri = 0; tri < obj->triangleCount(); tri++)
	{
		const Triangle &triangle = (*obj)[tri];

		for(unsigned int vert = 0; vert < 3; vert++)
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

void OBJDisplayer::calcBoundingBox()
{
	float minX = (*obj)[0].vertex[0].pos.x;
	float maxX = minX;
	float minY = (*obj)[0].vertex[0].pos.y;
	float maxY = minY;
	float minZ = (*obj)[0].vertex[0].pos.z;
	float maxZ = minZ;

	for (unsigned int tri = 0; tri < obj->triangleCount(); tri++)
	{
		const Triangle &triangle = (*obj)[tri];

		for(unsigned int vert = 0; vert < 3; vert++)
		{
			minX = min(minX, triangle.vertex[vert].pos.x);
			maxX = max(maxX, triangle.vertex[vert].pos.x);
			
			minY = min(minY, triangle.vertex[vert].pos.y);
			maxY = max(maxY, triangle.vertex[vert].pos.y);
			
			minZ = min(minZ, triangle.vertex[vert].pos.z);
			maxZ = max(maxZ, triangle.vertex[vert].pos.z);
			
		}
	}
	float length = max( abs(maxX - minX), max( abs(maxY - minY), abs(maxZ - minZ) ) );
	boundingBox.x = minX;
	boundingBox.y = minY;
	boundingBox.z = maxZ;
	boundingBox.length = length;

	cout <<  "minX " << minX << "	maxX " << maxX << endl
		<<  "minY " << minY << "	maxY " << maxY << endl
		<<  "minZ " << minZ << "	maxZ " << maxZ << endl
		<< "length " << length;
}

void OBJDisplayer::adjustAspectRatio()
{
	//glfwSetWindowAspectRatio(window, texture->getWidth(), texture->getHeight());
}
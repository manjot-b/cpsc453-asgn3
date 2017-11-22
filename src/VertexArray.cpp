#include "VertexArray.h"
#include <iostream>

using namespace std;

VertexArray::VertexArray(const int vertexComponents[], size_t vertCompSize, const float buffer[], size_t buffSize)
{
    glGenBuffers(1, &vbo); // gen buffer and store id in VBO
	glGenVertexArrays(1, &id);
    
    glBindVertexArray(id);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER,  buffSize * sizeof(float), buffer, GL_STATIC_DRAW);

    const int totalComponents = sumArray(0, vertCompSize, vertexComponents);
    const int stride = totalComponents * sizeof(float);

    for(unsigned int i = 0; i < vertCompSize; i++)
    {
        int components = vertexComponents[i];
        int offset = sumArray(0, i, vertexComponents);

        glVertexAttribPointer(i, components, GL_FLOAT, GL_FALSE, stride, (void*)(offset * sizeof(float)));
	    glEnableVertexAttribArray(i);    
    }


	// unbind VAO, VBO, and EBO
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &id);
	glDeleteBuffers(1, &vbo);
}

int VertexArray::sumArray(int start, int end, const int array[]) const
{
    int sum = 0;
    for (int i = start; i < end; i++)
    {
        sum += array[i];
    }
    return sum;
}
/*
*   Displays images and draws catmull points
*
*/

#ifndef OBJDISPLAYER_H
#define OBJSDISPLAYER_H

#include <GLFW/glfw3.h>
#include "Texture.h"
#include "Shader.h"
#include "objmodel.h"
#include <vector>
#include <glm/glm.hpp>
#include <string>

class OBJDisplayer
{
    public: 
        OBJDisplayer(const char *objPath, const char *texPath);
        ~OBJDisplayer();
        
        /*
        *   run the program and exits once
        *   the windows is closed
        */
        int run();          
    
    private:
        /*
        *   Initializes the glfw window
        */
        int initWindow();
        GLFWwindow *window;
        GLuint VBO, VAO, EBO;
        OBJmodel *obj;
        Shader *shader;
        Texture *texture;
        std::vector<float> vertexBuffer;

        /*
        *   Process the input made by the user
        */
        void processInput(GLFWwindow *window);
        void adjustAspectRatio();
        void extractVertexData();
};

#endif
/*
*   Displays images and draws catmull points
*
*/

#ifndef OBJDISPLAYER_H
#define OBJSDISPLAYER_H

#include <GLFW/glfw3.h>
#include "Texture.h"
#include "Shader.h"
#include "VertexArray.h"
#include "objmodel.h"
#include <vector>
#include <glm/glm.hpp>
#include <string>

class OBJDisplayer
{
    public: 
        OBJDisplayer(const char *objPath, const char *texPath, const char *aoPath);
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
        
        struct BoundingBox      // box is a sqare
        {
            float x, y, z, width, height, depth;
        };
        BoundingBox boundingBox; 
        
        GLuint VBO, VAO, EBO;
        OBJmodel *obj;
        Shader *shader;
        Texture *texture;
        Texture *aoMap;
        VertexArray *vertexArray;
        std::vector<float> vertexBuffer;
        glm::mat4 perspective;
        glm::mat4 view;
        float rotX, rotY, rotZ, scale;
        glm::vec3 lightPosition;
        glm::vec3 lightColor;
        vector<glm::vec4> lightConstants;
        float lightConstantsIdx;
        bool renderTexture, renderAOMap, T_KeyHeld, Y_KeyHeld;

        /*
        *   Process the input made by the user
        */
        void processInput(GLFWwindow *window);
        void adjustAspectRatio();

        /*
            Extracts all the vertex data from the object
            and stores it in a float buffer
        */
        void extractVertexData();
        void calcBoundingBox();
        
};

#endif
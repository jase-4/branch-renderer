
#ifndef MESH_HPP
#define MESH_HPP

#include <glad/glad.h> // holds all OpenGL type declarations

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "renderer/shader.hpp"
#include "renderer/render_components.hpp"

#include <string>
#include <vector>
using namespace std;


struct Texture {
    unsigned int id;
    string type;
    string path;
};

class Mesh {
public:
    // mesh Data
    vector<Vertex>       vertices;
    vector<unsigned int> indices;
    vector<Texture>      textures;
    GLuint VAO;
    GLuint VBO;
    GLuint IBO;


    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures)
    {
        this->vertices = vertices;
        this->indices = indices;
        this->textures = textures;

        setupMesh();
    }

   
    void Draw(Shader &shader) 
    {

        //need to figured out better texture system

    
        // // bind appropriate textures
        // unsigned int diffuseNr  = 1;
        // unsigned int specularNr = 1;
        // unsigned int normalNr   = 1;
        // unsigned int heightNr   = 1;
        // for(unsigned int i = 0; i < textures.size(); i++)
        // {
        //     //glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
        //     // retrieve texture number (the N in diffuse_textureN)
        //     string number;
        //     string name = textures[i].type;
        //     if(name == "texture_diffuse")
        //         number = std::to_string(diffuseNr++);
        //     else if(name == "texture_specular")
        //         number = std::to_string(specularNr++); // transfer unsigned int to string
        //     else if(name == "texture_normal")
        //         number = std::to_string(normalNr++); // transfer unsigned int to string
        //      else if(name == "texture_height")
        //         number = std::to_string(heightNr++); // transfer unsigned int to string

            // now set the sampler to the correct texture unit
             //glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i);
             //GLuint uniformLocation = glGetUniformLocation(shader.ID, (name + number).c_str());
             //GLuint uniformLocation = glGetUniformLocation(shader.ID, "kyle");

            //glUniform1i(uniformLocation,i);
           // std::cout << "Uniform location for " << (name + number) << ": " << uniformLocation << std::endl;
            

           //std::cout << (name + number).c_str() << std::endl;
            // and finally bind the texture
            //glBindTexture(GL_TEXTURE_2D, textures[i].id);
        //     glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i);
        //     // and finally bind the texture
        //     glBindTexture(GL_TEXTURE_2D, textures[i].id);
        // }

        
       
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        glActiveTexture(GL_TEXTURE0);
    }

private:
   
    void setupMesh()
    {
       
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &IBO);

        glBindVertexArray(VAO);
      
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
      
      
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);  

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);	
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        glEnableVertexAttribArray(1);	
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
        glEnableVertexAttribArray(2);	
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));
		glEnableVertexAttribArray(5);
		glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, m_BoneIDs));
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_Weights));
        glBindVertexArray(0);
    }
};
#endif

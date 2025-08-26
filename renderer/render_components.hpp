

#ifndef RENDER_COMPONENETS_HPP
#define RENDER_COMPONENETS_HPP

#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.hpp"

#include "entity/component.hpp"

#include <string>
#include <vector>
#include <iostream>
#include <glm/gtc/quaternion.hpp>


#define MAX_BONE_INFLUENCE 4

//class Component;

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    glm::vec3 Tangent;
    glm::vec3 Bitangent;
	int m_BoneIDs[MAX_BONE_INFLUENCE];
	float m_Weights[MAX_BONE_INFLUENCE];
};

struct TextureComponent {
    unsigned int id;
    std::string type;
    std::string path;
};




struct TransformComponent :  public Component {
    glm::vec3 position = glm::vec3(0.0f);
    glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f); 
    glm::vec3 scale = glm::vec3(1.0f); 

    glm::mat4 getModelMatrix() const {
        glm::mat4 translation = glm::translate(glm::mat4(1.0f), position);
        glm::mat4 rotationMat = glm::mat4_cast(rotation);
        glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), scale);
        return translation * rotationMat * scaleMat;
    }
};



struct MeshComponent {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<TextureComponent>      textures;
  
    GLuint VAO, VBO, IBO;

    MeshComponent(const std::vector<Vertex>& verts,
        const std::vector<unsigned int>& inds,
        const std::vector<TextureComponent>& texs)
    : vertices(verts), indices(inds), textures(texs) {}
};



struct RenderableComponent :public Component {
    std::vector<MeshComponent> meshes; 
    std::vector<TextureComponent> textures; 
    bool should_render = true;
    bool isTextured = true;
    glm::vec3 color = glm::vec3(0.8, 0.8, 0.8);
};



// void printVec3(const glm::vec3& vec, const std::string& label = "") {
//     std::cout << label << ": (" << vec.x << ", " << vec.y << ", " << vec.z << ")\n";
// }

// void printEulerFromQuat(const glm::quat& q)
// {
//     glm::vec3 eulerDegrees = glm::degrees(glm::eulerAngles(q));

//     std::cout << "Euler angles (degrees):" << std::endl;
//     std::cout << "  Pitch (X): " << eulerDegrees.x << std::endl;
//     std::cout << "  Yaw   (Y): " << eulerDegrees.y << std::endl;
//     std::cout << "  Roll  (Z): " << eulerDegrees.z << std::endl;
// }

// void printMat3(const glm::mat3& mat, const std::string& label = "")
// {
//     std::cout << label << ":\n";
//     for (int i = 0; i < 3; ++i) {
//         std::cout << "  (" << mat[i][0] << ", " << mat[i][1] << ", " << mat[i][2] << ")\n";
//     }
// }



#endif
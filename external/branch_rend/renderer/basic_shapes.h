#ifndef BASIC_SHAPES_H
#define BASIC_SHAPES_H

#include <glm/glm.hpp>
#include "render_components.hpp"

struct Cube {
    glm::vec3 position;
    float size;
};

struct BoundingVolume
{
    glm::vec3 position;
    virtual ~BoundingVolume() = default;  
};

struct Sphere : BoundingVolume{
   
    float radius;
    int segments; 
};

struct Plane : BoundingVolume {
    
    glm::vec3 normal;
    float size;
};

struct aabb  : BoundingVolume {  
    glm::vec3 halfSize;  
    aabb(glm::vec3 p, glm::vec3 hf) {
        position = p;  
        halfSize = hf;
    }

    glm::vec3 getMin() const { return position - halfSize; }
    glm::vec3 getMax() const { return position + halfSize; }
    aabb() = default;
};

RenderableComponent generatePlane(const Plane& plane) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    glm::vec3 right = glm::normalize(glm::cross(plane.normal, glm::vec3(0.0, 0.0, 1.0)));
    if (glm::length(right) < 0.01f)
        right = glm::normalize(glm::cross(plane.normal, glm::vec3(0.0, 1.0, 0.0)));

    glm::vec3 up = glm::normalize(glm::cross(right, plane.normal));
    glm::vec3 halfExtents = plane.size * 0.5f * right;
    glm::vec3 halfHeight = plane.size * 0.5f * up;
    
    glm::vec3 corners[4] = {
        glm::vec3(50.0, 0.0, 50.0),  // Top-right
        glm::vec3(50.0, 0.0, -50.0), // Bottom-right
        glm::vec3(-50.0, 0.0, -50.0),// Bottom-left
        glm::vec3(-50.0, 0.0, 50.0)  // Top-left
    };

    glm::vec2 uvs[4] = {
        {0.0f, 0.0f},
        {1.0f, 0.0f},
        {1.0f, 1.0f},
        {0.0f, 1.0f},
    };

    for (int i = 0; i < 4; ++i) {
        Vertex v;
        v.Position = corners[i];
        v.Normal = plane.normal;
        v.TexCoords = uvs[i];
        v.Tangent = right;
        v.Bitangent = up;
        std::fill(std::begin(v.m_BoneIDs), std::end(v.m_BoneIDs), 0);
        std::fill(std::begin(v.m_Weights), std::end(v.m_Weights), 0.0f);
        vertices.push_back(v);
    }

    indices = { 0, 1, 2, 2, 3, 0 };

    MeshComponent mesh(vertices, indices, {});
    mesh.VAO = 0; mesh.VBO = 0; mesh.IBO = 0; 
    
    glGenVertexArrays(1, &mesh.VAO);
    glGenBuffers(1, &mesh.VBO);
    glGenBuffers(1, &mesh.IBO);

    glBindVertexArray(mesh.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

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

    RenderableComponent rend;
    rend.meshes.push_back(mesh);
    rend.isTextured = false;
    return rend;
}


RenderableComponent generateCube(glm::vec3 halfExtents) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    glm::vec3 positions[8] = {
        {-halfExtents.x, -halfExtents.y, -halfExtents.z}, // 0
        { halfExtents.x, -halfExtents.y, -halfExtents.z}, // 1
        { halfExtents.x,  halfExtents.y, -halfExtents.z}, // 2
        {-halfExtents.x,  halfExtents.y, -halfExtents.z}, // 3
        {-halfExtents.x, -halfExtents.y,  halfExtents.z}, // 4
        { halfExtents.x, -halfExtents.y,  halfExtents.z}, // 5
        { halfExtents.x,  halfExtents.y,  halfExtents.z}, // 6
        {-halfExtents.x,  halfExtents.y,  halfExtents.z}  // 7
    };

    glm::vec3 normals[6] = {
        { 0.0f,  0.0f, -1.0f}, // Back
        { 0.0f,  0.0f,  1.0f}, // Front
        {-1.0f,  0.0f,  0.0f}, // Left
        { 1.0f,  0.0f,  0.0f}, // Right
        { 0.0f,  1.0f,  0.0f}, // Top
        { 0.0f, -1.0f,  0.0f}  // Bottom
    };

    unsigned int cubeIndices[] = {
        // Back face
        0, 1, 2, 2, 3, 0,
        // Front face
        4, 5, 6, 6, 7, 4,
        // Left face
        0, 3, 7, 7, 4, 0,
        // Right face
        1, 5, 6, 6, 2, 1,
        // Top face
        3, 2, 6, 6, 7, 3,
        // Bottom face
        0, 1, 5, 5, 4, 0
    };

    for (int i = 0; i < 8; ++i) {
        Vertex vertex;
        vertex.Position = positions[i];
        vertex.Normal = glm::normalize(positions[i]); 
        vertex.TexCoords = glm::vec2(0.0f);
        vertex.Tangent = glm::vec3(1.0f, 0.0f, 0.0f); 
        vertex.Bitangent = glm::vec3(0.0f, 1.0f, 0.0f); 
        std::fill(std::begin(vertex.m_BoneIDs), std::end(vertex.m_BoneIDs), 0);
        std::fill(std::begin(vertex.m_Weights), std::end(vertex.m_Weights), 0.0f);

        vertices.push_back(vertex);
    }

    indices.insert(indices.end(), std::begin(cubeIndices), std::end(cubeIndices));

    MeshComponent mesh(vertices, indices, {});
    mesh.VAO = 0; mesh.VBO = 0; mesh.IBO = 0;

    glGenVertexArrays(1, &mesh.VAO);
    glGenBuffers(1, &mesh.VBO);
    glGenBuffers(1, &mesh.IBO);

    glBindVertexArray(mesh.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

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

    RenderableComponent rend;
    rend.meshes.push_back(mesh);
    rend.isTextured = true;
    return rend;
}




RenderableComponent generateSphere(Sphere& sphere) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

   
    for (int lat = 0; lat <= sphere.segments; ++lat) {
        float theta = glm::pi<float>() * float(lat) / float(sphere.segments); 
        float sinTheta = glm::sin(theta);
        float cosTheta = glm::cos(theta);

        for (int lon = 0; lon <= sphere.segments; ++lon) {
            float phi = 2.0f * glm::pi<float>() * float(lon) / float(sphere.segments);
            float sinPhi = glm::sin(phi);
            float cosPhi = glm::cos(phi);

            sphere.position = glm::vec3(0.0f);
            glm::vec3 position = sphere.position + sphere.radius * glm::vec3(sinTheta * cosPhi, cosTheta, sinTheta * sinPhi);
            glm::vec3 normal = glm::normalize(position - sphere.position);
            glm::vec2 texCoords = glm::vec2(float(lon) / float(sphere.segments), float(lat) / float(sphere.segments));
            glm::vec3 tangent = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), normal));
            glm::vec3 bitangent = glm::normalize(glm::cross(normal, tangent));

            Vertex vertex;
            vertex.Position = position;
            vertex.Normal = normal;
            vertex.TexCoords = texCoords;
            vertex.Tangent = tangent;
            vertex.Bitangent = bitangent;
            std::fill(std::begin(vertex.m_BoneIDs), std::end(vertex.m_BoneIDs), 0);
            std::fill(std::begin(vertex.m_Weights), std::end(vertex.m_Weights), 0.0f);

            vertices.push_back(vertex);
        }
    }

    for (int lat = 0; lat < sphere.segments; ++lat) {
        for (int lon = 0; lon < sphere.segments; ++lon) {
            int first = (lat * (sphere.segments + 1)) + lon;
            int second = first + sphere.segments + 1;

            indices.push_back(first);
            indices.push_back(second);
            indices.push_back(first + 1);

          
            indices.push_back(second);
            indices.push_back(second + 1);
            indices.push_back(first + 1);
        }
    }

   
    MeshComponent mesh(vertices, indices, {});
    mesh.VAO = 0; mesh.VBO = 0; mesh.IBO = 0;
    glGenVertexArrays(1, &mesh.VAO);
    glGenBuffers(1, &mesh.VBO);
    glGenBuffers(1, &mesh.IBO);

    glBindVertexArray(mesh.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

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

    RenderableComponent rend;
    rend.meshes.push_back(mesh);
    rend.isTextured = true;
    return rend;
}


GLuint createColorTexture(glm::vec3 color, int width, int height) {
    unsigned char* data = new unsigned char[width * height * 3];  
    for (int i = 0; i < width * height; ++i) {
        data[i * 3] = static_cast<unsigned char>(color.r * 255);   
        data[i * 3 + 1] = static_cast<unsigned char>(color.g * 255); 
        data[i * 3 + 2] = static_cast<unsigned char>(color.b * 255); 
    }

    GLuint textureID;
    glGenTextures(1, &textureID);  
    glBindTexture(GL_TEXTURE_2D, textureID);  
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glBindTexture(GL_TEXTURE_2D, 0);  
    delete[] data; 
    return textureID;
}



#endif
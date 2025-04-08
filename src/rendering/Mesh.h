#ifndef MESH_H
#define MESH_H

#include "assets/Shader.h"
#include "rendering/Material.h"

#include <glm/glm.hpp>

#include <vector>

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

class Mesh {
public:
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, Material* material);
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
    void Draw();

    unsigned int GetVAO() const { return VAO; }
    const std::vector<unsigned int>& GetIndices() const { return indices; }
    Material* GetMaterial() const { return material; }
    void SetMaterial(Material* mat);

private:
    GLuint VAO, VBO, EBO;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    Material* material;
    
    void SetupMesh();
};

#endif

#pragma once

#include "assets/Shader.h"
#include "rendering/Material.h"
#include "VertexArray.h"
#include "IndexBuffer.h"

#include <glm/glm.hpp>

#include <vector>

namespace Engine {
    struct Vertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texCoords;
    };

    class Mesh {
    public:
        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
        void Draw();

        unsigned int GetVAO() const { return VAO; }
        const VertexArray& GetVA() const { return va; }
        const VertexBuffer& GetVB() const { return vb; }
        const IndexBuffer& GetIB() const { return ib; }
        const std::vector<unsigned int>& GetIndices() const { return indices; }
        Material* GetMaterial() const { return material; }
        void SetMaterial(Material* mat);

    private:
        GLuint VAO, VBO, EBO;
        VertexArray va;
        VertexBuffer vb;
        IndexBuffer ib;
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        Material* material;

        void SetupMesh();
    };
}
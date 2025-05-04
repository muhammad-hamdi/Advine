#pragma once

#include "assets/Shader.h"
#include "rendering/Material.h"
#include "VertexArray.h"
#include "IndexBuffer.h"

#include "render/NRenderer.h"

#include <glm/glm.hpp>

#include <vector>

namespace Engine {
    struct Vertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texCoords;

        static BufferLayout GetLayout() {
            BufferLayout bl;
            bl.Push<float>(3);
            bl.Push<float>(3);
            bl.Push<float>(2);

            return bl;
        }
    };

    class Mesh {
    private:
        // old render api
        GLuint VAO, VBO, EBO;
        VertexArray va;
        VertexBuffer vb;
        IndexBuffer ib;

        // new render api
        GPUHandle vertexDescription;
        GPUHandle vertexBuffer;
        GPUHandle indexBuffer;

        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        Material* material;
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

        // new render api
        inline GPUHandle GetVertexBufferHandle()      const { return vertexBuffer; }
        inline GPUHandle GetIndexBufferHandle()       const { return indexBuffer; }
        inline GPUHandle GetVertexDescriptionHandle() const { return vertexDescription; }

        inline uint32_t GetIndexCount() const { return indices.size(); }

    private:
        void SetupMesh();

        // new render api
        void Upload();
    };
}
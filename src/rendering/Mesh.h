#pragma once

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
        GPUHandle vertexDescription;
        GPUHandle vertexBuffer;
        GPUHandle indexBuffer;

        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
    public:
        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);

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
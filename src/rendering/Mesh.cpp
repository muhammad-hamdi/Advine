#include "Mesh.h"

#include "core/Context.h"

namespace Engine {
    Mesh::Mesh(std::vector<Vertex> verts, std::vector<unsigned int> inds)
        : vertices(verts), indices(inds)
    {
        Upload();
        // SetupMesh();
    }

    Mesh::Mesh(float* verts, int count, BufferLayout layout) {
        auto api = Context::Get().GetRenderer()->GetAPI();
        vertexBuffer = api->CreateVertexBuffer(count * sizeof(float), verts);
        vertexDescription = api->CreateVertexDescription(layout);
    }

    void Mesh::SetMaterial(const std::string matName) {
        material = matName;
    }

    std::string Mesh::GetMaterial() { return material; }

    void Mesh::Upload()
    {
        auto api = Context::Get().GetRenderer()->GetAPI();
        vertexBuffer = api->CreateVertexBuffer(vertices.size() * sizeof(Vertex), vertices.data());
        indexBuffer = api->CreateIndexBuffer(indices.size() * sizeof(unsigned int), indices.data());
        vertexDescription = api->CreateVertexDescription(Vertex::GetLayout());
    }

    void Mesh::SetupMesh() {
        // glGenVertexArrays(1, &VAO);

        // glBindVertexArray(VAO);

        // // Position
        // glEnableVertexAttribArray(0);
        // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

        // // Normal
        // glEnableVertexAttribArray(1);
        // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

        // // TexCoords
        // glEnableVertexAttribArray(2);
        // glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

        // glGenBuffers(1, &VBO);
        // glGenBuffers(1, &EBO);

        // // Upload vertex data
        // glBindBuffer(GL_ARRAY_BUFFER, VBO);
        // glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

        // // Upload index data
        // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        // glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

        // glBindVertexArray(0);
    }
}
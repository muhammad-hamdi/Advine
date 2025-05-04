#include "Mesh.h"

#include "core/Context.h"

namespace Engine {
    Mesh::Mesh(std::vector<Vertex> verts, std::vector<unsigned int> inds)
        : vertices(verts), indices(inds), vb(verts.data(), verts.size() * sizeof(Vertex)), ib(inds.data(), inds.size())
    {
        VertexBufferLayout layout;
        layout.Push<float>(3);
        layout.Push<float>(3);
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);
        Upload();
        // SetupMesh();
    }

    void Mesh::Draw() {
        // glBindVertexArray(VAO);
        // va.Bind();
        // glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        // va.Unbind();
        // glBindVertexArray(0);
    }

    void Mesh::Upload()
    {
        auto api = Context::Get().GetRenderer()->GetAPI();
        vertexDescription = api->CreateVertexDescription(Vertex::GetLayout());
        vertexBuffer = api->CreateVertexBuffer(vertices.size() * sizeof(Vertex), vertices.data());
        indexBuffer = api->CreateIndexBuffer(indices.size() * sizeof(unsigned int), indices.data());
    }

    void Mesh::SetMaterial(Material* mat)
    {
        material = mat;
    }

    void Mesh::SetupMesh() {
        glGenVertexArrays(1, &VAO);

        glBindVertexArray(VAO);

        // Position
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

        // Normal
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

        // TexCoords
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        // Upload vertex data
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

        // Upload index data
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

        glBindVertexArray(0);
    }
}
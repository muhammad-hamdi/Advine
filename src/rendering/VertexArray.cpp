#include "VertexArray.h"

#include "rendering/Renderer.h"

namespace Engine {
    VertexArray::VertexArray()
    {
        GLCall(glGenVertexArrays(1, &m_RendererID));
        Bind(); // Bind here before construction and upload of Buffer data
    }

    VertexArray::~VertexArray()
    {
        GLCall(glDeleteVertexArrays(1, &m_RendererID));
    }

    void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
    {
        std::cout << "Adding Buffer Layout" << std::endl;
        Bind();
        vb.Bind();
        const auto& elements = layout.GetElements();
        unsigned int offset = 0;
        for (size_t i = 0; i < elements.size(); i++)
        {
            const auto& element = elements[i];
            GLCall(glEnableVertexAttribArray(i));
            GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (void*)offset));
            offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
        }
        Unbind();
    }

    void VertexArray::Bind() const
    {
        GLCall(glBindVertexArray(m_RendererID));
    }

    void VertexArray::Unbind() const
    {
        GLCall(glBindVertexArray(0));
    }
}
#include "OpenGLRenderAPI.h"

namespace ae
{
    GPUHandle OpenGLRenderAPI::CreateVertexDescription(const BufferLayout& layout)
    {
        GPUHandle handle;
        glGenVertexArrays(1, &handle);
        glBindVertexArray(handle);
        const auto& elements = layout.GetElements();
        unsigned int offset = 0;
        for (size_t i = 0; i < elements.size(); i++)
        {
            const auto& element = elements[i];
            glEnableVertexAttribArray(i);
            glVertexAttribPointer(i, element.count, BufferType(element.type), element.normalized ? GL_TRUE : GL_FALSE, layout.GetStride(), (void*)offset);
            offset += element.count * SizeOfType(element.type);
        }
        // unbind?
        return handle;
    }
    GPUHandle OpenGLRenderAPI::CreateVertexBuffer(uint32_t size, const void *data)
    {
        GPUHandle handle;
        glGenBuffers(1, &handle);
        glBindBuffer(GL_ARRAY_BUFFER, handle);
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW); // hoist STATIC_DRAW to arguments?
        return handle;
    }
    GPUHandle OpenGLRenderAPI::CreateIndexBuffer(uint32_t size, const void *data)
    {
        GPUHandle handle;
        glGenBuffers(1, &handle);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW); // hoist STATIC_DRAW to arguments?
        return handle;
    }
    void OpenGLRenderAPI::BindVertexDescription(GPUHandle handle)
    {
        glBindVertexArray(handle);
    }
    void OpenGLRenderAPI::BindVertexBuffer(GPUHandle handle)
    {
        glBindBuffer(GL_ARRAY_BUFFER, handle);
    }
    void OpenGLRenderAPI::BindIndexBuffer(GPUHandle handle)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle);
    }
    GPUHandle OpenGLRenderAPI::CreateSahder(const std::string &vertexSrc, const std::string &fragmentSrc)
    {
        return GPUHandle();
    }
    void OpenGLRenderAPI::BindShader(GPUHandle handle)
    {
    }
    void OpenGLRenderAPI::SetUniformInt(GPUHandle handle, const std::string &name, int value)
    {
    }
    void OpenGLRenderAPI::SetUniformFloat(GPUHandle handle, const std::string &name, float value)
    {
    }
    void OpenGLRenderAPI::SetUniformVec3(GPUHandle handle, const std::string &name, const float *vec3)
    {
    }
    void OpenGLRenderAPI::SetUniformMat4(GPUHandle handle, const std::string &name, const float *mat4)
    {
    }
} // namespace ae
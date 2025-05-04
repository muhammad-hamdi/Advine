#include "OpenGLRenderAPI.h"

#include <iostream>

namespace Engine
{
    void OpenGLRenderAPI::Clear()
    {
        // TODO: make this adjustable
        GLCall(glClearColor(0.1f, 0.1f, 0.1f, 0.1f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    }

    void OpenGLRenderAPI::SetViewport(int x, int y, int width, int height)
    {
        GLCall(glViewport(x, y, width, height));
    }

    void OpenGLRenderAPI::SetDepth(bool enable)
    {
        if(enable)
        {
            GLCall(glEnable(GL_DEPTH_TEST));
        }
        else
        {
            GLCall(glDisable(GL_DEPTH_TEST));
        }
    }

    GPUHandle OpenGLRenderAPI::CreateVertexDescription(const BufferLayout &layout)
    {
        GPUHandle handle;
        GLCall(glGenVertexArrays(1, &handle));
        GLCall(glBindVertexArray(handle));
        const auto& elements = layout.GetElements();
        unsigned int offset = 0;
        for (size_t i = 0; i < elements.size(); i++)
        {
            const auto& element = elements[i];
            GLCall(glEnableVertexAttribArray(i));
            GLCall(glVertexAttribPointer(i, element.count, BufferType(element.type), element.normalized ? GL_TRUE : GL_FALSE, layout.GetStride(), (void*)offset));
            offset += element.count * BufferElementFormat::SizeOfType(element.type);
        }
        // unbind?
        return handle;
    }

    GPUHandle OpenGLRenderAPI::CreateVertexBuffer(uint32_t size, const void *data)
    {
        GPUHandle handle;
        GLCall(glGenBuffers(1, &handle));
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, handle));
        GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW)); // hoist STATIC_DRAW to arguments?
        return handle;
    }

    GPUHandle OpenGLRenderAPI::CreateIndexBuffer(uint32_t size, const void *data)
    {
        GPUHandle handle;
        GLCall(glGenBuffers(1, &handle));
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle));
        GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW)); // hoist STATIC_DRAW to arguments?
        return handle;
    }

    void OpenGLRenderAPI::BindVertexDescription(GPUHandle handle)
    {
        GLCall(glBindVertexArray(handle));
    }

    void OpenGLRenderAPI::BindVertexBuffer(GPUHandle handle)
    {
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, handle));
    }

    void OpenGLRenderAPI::BindIndexBuffer(GPUHandle handle)
    {
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle));
    }

    GPUHandle OpenGLRenderAPI::CreateFramebuffer()
    {
        GPUHandle fbHandle;
        glGenFramebuffers(1, &fbHandle);
        return fbHandle;
    }

    void OpenGLRenderAPI::BindFramebuffer(GPUHandle handle)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, handle);
    }

    void OpenGLRenderAPI::CheckShaderCompilation(GLuint shader, const std::string& shaderType) {
        GLint success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetShaderInfoLog(shader, 512, nullptr, infoLog);
            std::cerr << shaderType << " shader compilation failed:\n" << infoLog << std::endl;
        }
    }

    void OpenGLRenderAPI::CheckProgramLinking(GLuint shaderProgram) {
        GLint success;
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
            std::cerr << "Program linking failed:\n" << infoLog << std::endl;
        }
    }

    GPUHandle OpenGLRenderAPI::CreateShader(const std::string &vertexSrc, const std::string &fragmentSrc)
    {
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        const char* vertexCStr = vertexSrc.c_str();
        glShaderSource(vertexShader, 1, &vertexCStr, nullptr);
        glCompileShader(vertexShader);
        CheckShaderCompilation(vertexShader, "Vertex");

        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        const char* fragmentCStr = fragmentSrc.c_str();
        glShaderSource(fragmentShader, 1, &fragmentCStr, nullptr);
        glCompileShader(fragmentShader);
        CheckShaderCompilation(fragmentShader, "Fragment");

        GPUHandle shaderProgram = glCreateProgram();

        // Attach shaders
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);

        // Link program
        glLinkProgram(shaderProgram);
        CheckProgramLinking(shaderProgram);

        // Clean up shaders as they're no longer needed after linking
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return shaderProgram;
    }

    void OpenGLRenderAPI::BindShader(GPUHandle handle)
    {
        glUseProgram(handle);
    }

    void OpenGLRenderAPI::SetUniformInt(GPUHandle handle, const std::string &name, int value)
    {
        GLint loc;
        if(locations.find(name) != locations.end()) {
            loc = locations[name];
        } else {
            loc = glGetUniformLocation(handle, name.c_str());
        }
        glUniform1i(loc, value);
    }

    void OpenGLRenderAPI::SetUniformFloat(GPUHandle handle, const std::string &name, float value)
    {
        GLint loc;
        if(locations.find(name) != locations.end()) {
            loc = locations[name];
        } else {
            loc = glGetUniformLocation(handle, name.c_str());
        }
        glUniform1f(loc, value);
    }

    void OpenGLRenderAPI::SetUniformVec3(GPUHandle handle, const std::string &name, const float *vec3)
    {
        GLint loc;
        if(locations.find(name) != locations.end()) {
            loc = locations[name];
        } else {
            loc = glGetUniformLocation(handle, name.c_str());
        }
        glUniform3fv(loc, 1, vec3);
    }

    void OpenGLRenderAPI::SetUniformMat4(GPUHandle handle, const std::string &name, const float *mat4)
    {
        GLint loc;
        if(locations.find(name) != locations.end()) {
            loc = locations[name];
        } else {
            loc = glGetUniformLocation(handle, name.c_str());
        }
        glUniformMatrix4fv(loc, 1, GL_FALSE, mat4);
    }

    GPUHandle OpenGLRenderAPI::CreateUniformBuffer(uint32_t size)
    {
        return GPUHandle();
    }

    void OpenGLRenderAPI::UpdateBuffer(GPUHandle handle, const void *data, uint32_t size, uint32_t offset)
    {
    }

    void OpenGLRenderAPI::BindUniformBufferBlockIndex(GPUHandle shaderHandle, const std::string &name, uint32_t bindingIndex)
    {
    }

    GPUHandle OpenGLRenderAPI::CreateTexture2D(uint32_t width, uint32_t height, const void *data, int channels)
    {
        GPUHandle textureHandle;
        glGenTextures(1, &textureHandle);
        glBindTexture(GL_TEXTURE_2D, textureHandle);

        // Texture parameters, should be modifiable
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); 
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


        GLenum format = GL_RGB;
        if (channels == 1) format = GL_RED;
        else if (channels == 4) format = GL_RGBA;

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        return textureHandle;
    }

    GPUHandle OpenGLRenderAPI::CreateTextureRGB(uint32_t width, uint32_t height)
    {
        GPUHandle texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        return texture;
    }

    GPUHandle OpenGLRenderAPI::CreateTextureDepth(uint32_t width, uint32_t height)
    {
        GPUHandle texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        return texture;
    }

    void OpenGLRenderAPI::BindTexture(GPUHandle handle, uint32_t slot)
    {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, handle);
    }

    void OpenGLRenderAPI::DrawIndexed(uint32_t indexCount)
    {
        GLCall(glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0));
    }
} // namespace ae
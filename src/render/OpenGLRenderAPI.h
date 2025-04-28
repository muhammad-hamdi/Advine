#pragma once
#include "RenderAPI.h"

#include "glad/glad.h"
#include <unordered_map>

namespace Engine
{
    class OpenGLRenderAPI : public RenderAPI
    {
        GPUHandle CreateVertexDescription(const BufferLayout& layout);
        GPUHandle CreateVertexBuffer(uint32_t size, const void* data);
        GPUHandle CreateIndexBuffer(uint32_t size, const void* data);

        void BindVertexDescription(GPUHandle handle);
        void BindVertexBuffer(GPUHandle handle);
        void BindIndexBuffer(GPUHandle handle);

        GPUHandle CreateFramebuffer();

        GPUHandle CreateSahder(const std::string& vertexSrc, const std::string& fragmentSrc);
        void BindShader(GPUHandle handle);
        void SetUniformInt(GPUHandle handle, const std::string& name, int value);
        void SetUniformFloat(GPUHandle handle, const std::string& name, float value);
        void SetUniformVec3(GPUHandle handle, const std::string& name, const float* vec3);
        void SetUniformMat4(GPUHandle handle, const std::string& name, const float* mat4);

        GPUHandle CreateTexture2D(uint32_t width, uint32_t height, const void* data, int channels = 4);
        GPUHandle CreateTextureRGB(uint32_t width, uint32_t height);
        GPUHandle CreateTextureDepth(uint32_t width, uint32_t height);
        void BindTexture(GPUHandle handle, uint32_t slot = 0);

        void DrawIndexed(uint32_t indexCount);
    private:
        uint32_t BufferType(BufferDataType type) {
            switch (type)
            {
                case BufferDataType::Float32: return GL_FLOAT;
                case BufferDataType::UInt32:  return GL_UNSIGNED_INT;
                case BufferDataType::UInt8:   return GL_UNSIGNED_BYTE;
                // TODO: add rest later
            }
            // AE_ASSERT(false); TODO: add engine pch with such utils
            return 0;
        }

        void CheckShaderCompilation(GLuint shader, const std::string &shaderType);
        void CheckProgramLinking(GLuint shaderProgram);

    private:
        std::unordered_map<std::string, GLint> locations;
    };
} // namespace ae

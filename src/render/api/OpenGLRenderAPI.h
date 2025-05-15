#pragma once
#include "RenderAPI.h"

#include "glad/glad.h"
#include <unordered_map>
#include <iostream>

namespace Engine
{
#ifdef _WIN32
#define DEBUG __debugbreak
#else
#include <signal.h>
#define DEBUG raise(SIGTRAP)
#endif

#define ASSERT(x) if(!(x)) DEBUG();

#define GLCall(x)   GLClearError(); \
                    x; \
                    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

    static void GLClearError() {
        while(glGetError() != GL_NO_ERROR);
    }

    static bool GLLogCall(const char *function, const char *file, int line) {
        while(GLenum error = glGetError()) {
            std::cout << "[OpenGL Error] (" << error << "): " << function << " " << file << ":" << line << std::endl;
            return false;
        }
        return true;
    }

    class OpenGLRenderAPI : public RenderAPI
    {
    public:
        OpenGLRenderAPI(): locations(30) {
            // viewport setting
            // depth enable
        }
        ~OpenGLRenderAPI(){}

        void Clear();
        void ClearDepth();
        void SetViewport(int x, int y, int width, int height);
        void SetDepth(bool enable);

        GPUHandle CreateVertexDescription(const BufferLayout& layout);
        GPUHandle CreateVertexBuffer(uint32_t size, const void* data);
        GPUHandle CreateIndexBuffer(uint32_t size, const void* data);

        void BindVertexDescription(GPUHandle handle);
        void BindVertexBuffer(GPUHandle handle);
        void BindIndexBuffer(GPUHandle handle);

        GPUHandle CreateFramebuffer();
        void BindFramebuffer(GPUHandle handle);
        void AttachDepthBuffer(GPUHandle framebufferHandle, GPUHandle depthBufferHandle);

        GPUHandle CreateShader(const std::string& vertexSrc, const std::string& fragmentSrc);
        void BindShader(GPUHandle handle);
        void SetUniformInt(GPUHandle handle, const std::string& name, int value);
        void SetUniformFloat(GPUHandle handle, const std::string& name, float value);
        void SetUniformVec3(GPUHandle handle, const std::string& name, const float* vec3);
        void SetUniformMat4(GPUHandle handle, const std::string& name, const float* mat4);

        GPUHandle CreateUniformBuffer(uint32_t size);
        void UpdateBuffer(GPUHandle handle, const void* data, uint32_t size, uint32_t offset = 0);
        void BindUniformBufferBlockIndex(GPUHandle shaderHandle, const std::string& name, uint32_t bindingIndex);

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
        std::vector<std::unordered_map<std::string, GLint>> locations;
    };
} // namespace ae

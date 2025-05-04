#pragma once

#include <memory>
#include <vector>
#include <string>

#include "BufferLayout.h"

namespace Engine
{
    using GPUHandle = uint32_t;

    enum class RenderBackend {
        OpenGL,
        Vulkan // To Implement Later
    };

    class RenderAPI
    {
    public:
        // === Buffers ===

        /// @brief specify the layout of vertex data (e.g. VertexArrayObject in OpenGL)
        /// @param layout 
        /// @return GPUHandle
        virtual GPUHandle CreateVertexDescription(const BufferLayout& layout) = 0;
        /// @brief Upload vertex data to GPU and get a handle to it
        /// @param size 
        /// @param data 
        /// @return GPUHandle
        virtual GPUHandle CreateVertexBuffer(uint32_t size, const void* data) = 0;
        /// @brief Upload index array data to GPU and get a handle to it
        /// @param size 
        /// @param data 
        /// @return GPUHandle
        virtual GPUHandle CreateIndexBuffer(uint32_t size, const void* data) = 0;

        virtual void BindVertexDescription(GPUHandle handle) = 0;
        virtual void BindVertexBuffer(GPUHandle handle) = 0;
        virtual void BindIndexBuffer(GPUHandle handle) = 0;

        // === Framebuffers ===
        virtual GPUHandle CreateFramebuffer() = 0;
        virtual void BindFramebuffer(GPUHandle handle) = 0;

        // === Shaders ===
        virtual GPUHandle CreateShader(const std::string& vertexSrc, const std::string& fragmentSrc) = 0;
        virtual void BindShader(GPUHandle handle) = 0;
        virtual void SetUniformInt(GPUHandle handle, const std::string& name, int value) = 0;
        virtual void SetUniformFloat(GPUHandle handle, const std::string& name, float value) = 0;
        virtual void SetUniformVec3(GPUHandle handle, const std::string& name, const float* vec3) = 0;
        virtual void SetUniformMat4(GPUHandle handle, const std::string& name, const float* mat4) = 0;
        // TODO: add more as needed

        // === UBOs ===
        virtual GPUHandle CreateUniformBuffer(uint32_t size) = 0;
        virtual void UpdateBuffer(GPUHandle handle, const void* data, uint32_t size, uint32_t offset = 0) = 0;
        virtual void BindUniformBufferBlockIndex(GPUHandle shaderHandle, const std::string& name, uint32_t bindingIndex) = 0;

        // === Textures ===
        virtual GPUHandle CreateTexture2D(uint32_t width, uint32_t height, const void* data, int channels = 4) = 0;
        virtual GPUHandle CreateTextureRGB(uint32_t width, uint32_t height) = 0;
        virtual GPUHandle CreateTextureDepth(uint32_t width, uint32_t height) = 0;
        virtual void BindTexture(GPUHandle handle, uint32_t slot = 0) = 0;

        virtual void DrawIndexed(uint32_t indexCount) = 0;

        virtual ~RenderAPI() = default;

    };
} // namespace ae

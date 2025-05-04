#pragma once
#include "api/RenderAPI.h"

namespace Engine
{
    struct NTexture
    {
        GPUHandle handle;
        int width, height;
        int channels;
        std::string filepath;
    };
} // namespace Engine

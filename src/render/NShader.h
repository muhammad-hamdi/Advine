#pragma once
#include "render/api/RenderAPI.h"

namespace Engine
{
    struct NShader
    {
        GPUHandle handle;
        std::string vertexPath;
        std::string fragmentPath;
        // TODO: unified shader files
    };
} // namespace Engine

#pragma once

#include <utility>

namespace Engine
{
    class NRenderer;
    class Context {
    private:
        NRenderer* renderer = nullptr;
        int windowWidth, windowHeight;
    public:
        inline NRenderer* GetRenderer() { return renderer; }
        inline void SetRenderer(NRenderer* ren) { renderer = ren; }

        inline std::pair<int, int> GetWindowDims() { return {windowWidth, windowHeight}; }
        inline void SetWindowDims(int width, int height) { windowWidth = width, windowHeight = height; }

        static Context& Get() {
            static Context instance;
            return instance;
        }
    };
} // namespace Engine

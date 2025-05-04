#pragma once

class NRenderer;

namespace Engine
{
    class Context {
    private:
        NRenderer* renderer = nullptr;
    public:
        inline NRenderer* GetRenderer() { return renderer; }
        inline void SetRenderer(NRenderer* ren) { renderer = ren; }

        static Context& Get() {
            static Context instance;
            return instance;
        }
    };
} // namespace Engine

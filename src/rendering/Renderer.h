#pragma once
#include "glad/glad.h"
#include "glm/glm.hpp"

#include <iostream>
#include <vector>

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

#include "LightData.h"

namespace Engine {
    class Mesh;
    class Material;
    class Entity;
    class Scene;

    class Renderer {
    public:
        static void RenderScene(Scene& scene);
        static void RenderEntity(Entity* entity);
        static void DrawMesh(Mesh* mesh, Material* material, const glm::mat4& modelMatrix);

        static glm::mat4 GetViewMatrix();
        static glm::mat4 GetProjectionMatrix();
        static glm::vec3 GetCameraPosition();

        static void SetViewProjection(glm::mat4 view, glm::mat4 projection, glm::vec3 camPosition);
    private:

        inline static glm::mat4 viewMatrix;
        inline static glm::mat4 projectionMatrix;
        inline static glm::vec3 cameraPosition;
        inline static std::vector<LightData> lightsToRender;
    };
}
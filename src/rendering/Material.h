#pragma once

#include "render/NShader.h"
#include "render/NTexture.h"

#include <glm/glm.hpp>

#include <vector>
#include <unordered_map>

namespace Engine {
    /*
    // TODO: offline material assets (e.g. "shiny_material.mat")
    // structure in scene data
    "components": [
      {
        "name": "meshRenderer",
        "materialOverrides": {
          "0": "assets/materials/wood.mat",
          "2": "assets/materials/shiny_material.mat"
        }
      }
    ]
    // 13/5/25: looking at this structure I'm not sure it's good enough, will have to iterate when I revisit this part
    */

    class Material {
    private:
        NShader* nShader;
        std::unordered_map<std::string, NTexture*> nTextures;
    public:
        std::string name;

        // TODO: material props when no texture, and base ambience, also pass shininess for specular exponent
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
        float shininess;

        bool isLit = true;

        Material(const std::string& name, NShader* shader) : name(name), nShader(shader) {}

        inline GPUHandle GetShaderHandle() const { return nShader->handle; }
        inline void SetShader(NShader* newShader) { nShader = newShader; }

        inline std::unordered_map<std::string, NTexture*> GetTextures() const { return nTextures; }
        inline void AddTexture(const std::string& name, NTexture* tex) { nTextures[name] = tex; }
    };
}
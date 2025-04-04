#include "Model.h"
#include "Mesh.h"
#include "Material.h"
#include "Texture.h"

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <tiny_gltf.h>
#include <iostream>

void Model::LoadGLTF(const std::string& path) {
    tinygltf::TinyGLTF loader;
    tinygltf::Model gltfModel;
    std::string err, warn;

    bool ret = loader.LoadASCIIFromFile(&gltfModel, &err, &warn, path);

    if (!warn.empty()) std::cout << "GLTF warning: " << warn << std::endl;
    if (!err.empty()) std::cerr << "GLTF error: " << err << std::endl;
    if (!ret) {
        std::cerr << "Failed to load glTF: " << path << std::endl;
        return;
    }

    for (const auto& mesh : gltfModel.meshes) {
        for (const auto& primitive : mesh.primitives) {
            std::vector<Vertex> vertices;
            std::vector<unsigned int> indices;

            const float* posBuffer = nullptr;
            const float* normBuffer = nullptr;
            const float* texCoordBuffer = nullptr;

            // POSITION
            if (primitive.attributes.find("POSITION") != primitive.attributes.end()) {
                const auto& accessor = gltfModel.accessors[primitive.attributes.at("POSITION")];
                const auto& bufferView = gltfModel.bufferViews[accessor.bufferView];
                posBuffer = reinterpret_cast<const float*>(
                    &gltfModel.buffers[bufferView.buffer].data[bufferView.byteOffset + accessor.byteOffset]);
            }

            // NORMAL
            if (primitive.attributes.find("NORMAL") != primitive.attributes.end()) {
                const auto& accessor = gltfModel.accessors[primitive.attributes.at("NORMAL")];
                const auto& bufferView = gltfModel.bufferViews[accessor.bufferView];
                normBuffer = reinterpret_cast<const float*>(
                    &gltfModel.buffers[bufferView.buffer].data[bufferView.byteOffset + accessor.byteOffset]);
            }

            // TEXCOORD_0
            if (primitive.attributes.find("TEXCOORD_0") != primitive.attributes.end()) {
                const auto& accessor = gltfModel.accessors[primitive.attributes.at("TEXCOORD_0")];
                const auto& bufferView = gltfModel.bufferViews[accessor.bufferView];
                texCoordBuffer = reinterpret_cast<const float*>(
                    &gltfModel.buffers[bufferView.buffer].data[bufferView.byteOffset + accessor.byteOffset]);
            }

            // Vertices
            const auto& posAccessor = gltfModel.accessors[primitive.attributes.at("POSITION")];
            for (size_t i = 0; i < posAccessor.count; i++) {
                Vertex vertex;

                vertex.position = glm::vec3(
                    posBuffer[i * 3 + 0],
                    posBuffer[i * 3 + 1],
                    posBuffer[i * 3 + 2]
                );

                if (normBuffer) {
                    vertex.normal = glm::vec3(
                        normBuffer[i * 3 + 0],
                        normBuffer[i * 3 + 1],
                        normBuffer[i * 3 + 2]
                    );
                } else {
                    vertex.normal = glm::vec3(0.0f);
                }

                if (texCoordBuffer) {
                    vertex.texCoords = glm::vec2(
                        texCoordBuffer[i * 2 + 0],
                        texCoordBuffer[i * 2 + 1]
                    );
                } else {
                    vertex.texCoords = glm::vec2(0.0f);
                }

                vertices.push_back(vertex);
            }

            // Indices
            const auto& indexAccessor = gltfModel.accessors[primitive.indices];
            const auto& bufferView = gltfModel.bufferViews[indexAccessor.bufferView];
            const auto& buffer = gltfModel.buffers[bufferView.buffer];

            const void* dataPtr = &buffer.data[bufferView.byteOffset + indexAccessor.byteOffset];

            for (size_t i = 0; i < indexAccessor.count; i++) {
                unsigned int index = 0;
                switch (indexAccessor.componentType) {
                    case TINYGLTF_PARAMETER_TYPE_UNSIGNED_BYTE:
                        index = ((uint8_t*)dataPtr)[i]; break;
                    case TINYGLTF_PARAMETER_TYPE_UNSIGNED_SHORT:
                        index = ((uint16_t*)dataPtr)[i]; break;
                    case TINYGLTF_PARAMETER_TYPE_UNSIGNED_INT:
                        index = ((uint32_t*)dataPtr)[i]; break;
                }
                indices.push_back(index);
            }

            // TODO: Load material and textures properly
            Material* mat = new Material(nullptr, nullptr); // Placeholder
            meshes.push_back(Mesh(vertices, indices, mat));
        }
    }
}

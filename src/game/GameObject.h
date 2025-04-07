#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include "core/Camera.h"
#include "graphics/Model.h"

#include <glm/glm.hpp>

#include <unordered_map>

class GameObject {
public:
    static int objectCount;

    std::string name;
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);

    std::vector<GameObject*> children;

    Model* model = nullptr;
    Material* globalMaterial = nullptr;
    std::unordered_map<Mesh*, Material*> materialOverrides;

    bool isWorldSpace = false;

    void SetWorldSpace(bool flag) {
        isWorldSpace = flag;
    }

    GameObject();

    void Update(float deltaTime);
    void Render(const Camera& camera, glm::mat4 globalTransform);

    std::string GetName() const;
    std::vector<GameObject*> GetChildren() const;

    void SetModel(Model* model);
    void SetMaterial(Material* material);
    void AddMaterialOverride(Mesh* mesh, Material* material);

    Material* GetMaterialForMesh(Mesh* mesh);

    void SetPosition(const glm::vec3& position);
    void SetRotation(const glm::vec3& rotation);
    void SetScale(const glm::vec3& scale);
    glm::mat4 GetTransform() const;
    glm::vec3 GetPosition() const;
    glm::vec3 GetRotation() const;
    glm::vec3 GetScale() const;

};

#endif

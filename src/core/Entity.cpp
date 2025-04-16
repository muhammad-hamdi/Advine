#include "Entity.h"

glm::mat4 Entity::GetWorldMatrix() const
{
    if (parent) {
        return parent->transform.GetLocalModelMatrix() * transform.GetLocalModelMatrix();
    } else {
        return transform.GetLocalModelMatrix();
    }
}

glm::vec3 Entity::GetWorldPosition() const
{
    return glm::vec3(GetWorldMatrix()[3]);
}

void Entity::Update(float deltaTime)
{
    for (const auto& comp : components) {
        comp.get()->Update(deltaTime);
    }
}

bool Entity::HasOverrides() const {
    return transform.positionOverridden ||
           transform.rotationOverridden ||
           transform.scaleOverridden ||
           std::any_of(components.begin(), components.end(),
                       [](const std::unique_ptr<Component>& c) { return c->IsOverridden(); });
}


void Entity::GatherOverrides(std::vector<json>& overridesOut, const std::vector<int>& path) const {
    if (HasOverrides() && !path.empty()) {
        json override;

        override["path"] = path;

        if(HasTransformOverride()) {
            auto t = transform;
            override["position"] = {t.position.x, t.position.y, t.position.z};
            override["rotation"] = {t.eulerRotation.x, t.eulerRotation.y, t.eulerRotation.z};
            override["scale"]    = {t.scale.x, t.scale.y, t.scale.z};
        }


        json comps = json::array();
        for (auto& component : components) {
            if(component->IsOverridden()) {
                comps.push_back(component->Serialize());
            }
        }

        if (!comps.empty())
            override["components"] = comps;

        overridesOut.push_back(override);
    }

    // Recurse into children
    for (size_t i = 0; i < children.size(); ++i) {
        std::vector<int> childPath = path;
        childPath.push_back(static_cast<int>(i));
        if(children[i]->source == 0) {
            children[i]->GatherOverrides(overridesOut, childPath);
        }
    }
}

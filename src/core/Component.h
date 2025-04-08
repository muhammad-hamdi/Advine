#pragma once
class Entity;

class Component {
public:
    virtual ~Component() = default;
    Entity* owner = nullptr;
};

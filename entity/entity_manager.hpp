#ifndef ENTITY_MANAGER_HPP
#define ENTITY_MANAGER_HPP

#include "entity.hpp"
#include "component.hpp"

#include <unordered_map>
#include <vector>
#include <memory>
#include <typeindex>
#include <type_traits>
#include <iostream>

class EntityManager {
public:
    static EntityManager& getInstance();

    void addEntity(Entity entity);

    template <typename T>
    void addComponent(Entity::ID entityID, std::shared_ptr<T> component);

    template <typename T>
    std::shared_ptr<T> getComponent(Entity::ID entityID);

    template <typename... Components>
    std::vector<Entity::ID> getEntitiesWithComponents();
    
    const std::vector<Entity>& getEntities() const;
    EntityManager(const EntityManager&) = delete;
    EntityManager& operator=(const EntityManager&) = delete;
    ~EntityManager() = default;

private:
    EntityManager();

    std::vector<Entity> entities;
    std::unordered_map<Entity::ID, std::unordered_map<std::type_index, std::shared_ptr<Component>>> components;
};

template <typename T>
void EntityManager::addComponent(Entity::ID entityID, std::shared_ptr<T> component) {
    components[entityID][std::type_index(typeid(T))] = component;
}

template <typename T>
std::shared_ptr<T> EntityManager::getComponent(Entity::ID entityID) {
    auto entityComponents = components.find(entityID);
    if (entityComponents != components.end()) {
        auto componentIt = entityComponents->second.find(std::type_index(typeid(T)));
        if (componentIt != entityComponents->second.end()) {
            return std::static_pointer_cast<T>(componentIt->second);
        }
    }
    return nullptr;
}

template <typename... Components>
std::vector<Entity::ID> EntityManager::getEntitiesWithComponents() {
    std::vector<Entity::ID> matchingEntities;

    for (const auto& [entityID, componentMap] : components) {
        bool hasAll = (... && (componentMap.find(std::type_index(typeid(Components))) != componentMap.end()));
        if (hasAll) {
            matchingEntities.push_back(entityID);
        }
    }

    return matchingEntities;
}

#endif // ENTITY_MANAGER_HPP
